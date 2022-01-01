cmake_minimum_required(VERSION 3.0)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(Cex2DexKernel)
set(SELF_NAME "cex2dex")
include("${VITASDK}/share/vita.cmake" REQUIRED)
include("${VITASDK}/share/vita.self_sign.cmake" OPTIONAL RESULT_VARIABLE self_sign_module_path)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3 -fno-inline")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

enable_language(C ASM)

add_custom_command(
  OUTPUT cmep_payload.c
  COMMAND cp ../../payload/build/mep_payload.bin ./cmep_payload.bin
  COMMAND xxd -i ./cmep_payload.bin > cmep_payload.c
)

add_executable(${PROJECT_NAME}
  src/main.c
  src/console_id.c
  src/idstorage.c
  cmep_payload.c
)

target_link_libraries(${PROJECT_NAME}
  SceIdStorageForDriver_stub
  SceIofilemgrForDriver_stub
  SceDebugForDriver_stub
  SceKernelUtilsForDriver_stub
  SceSysclibForDriver_stub
  SceSysmemForDriver_stub
  SceSysrootForKernel_stub
  SceModulemgrForDriver_stub
  SceThreadmgrForDriver_stub
  SceSblAIMgrForDriver_stub
  CmepMgrForKernel_stub
  taihenModuleUtils_stub
  taihenForKernel_stub
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES LINK_FLAGS "-nostdlib"
  COMPILE_FLAGS "-D__PSP2_KERNEL__"
)

vita_create_self(${SELF_NAME}.skprx ${PROJECT_NAME}
  CONFIG exports.yml
  UNSAFE
  STRIPPED
  REL_OPTIMIZE
)

if(NOT ${self_sign_module_path} MATCHES "NOTFOUND")
  vita_self_sign(${SELF_NAME}.skprx
    SDK_TYPE 0
    SDK_VERSION 0x3600000
    SELF_TYPE REF00D_KERNEL
    ATTR ./config_attributes.txt
    CAPS ./config_capability.txt
  )
endif()

vita_create_stubs(stubs_${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/exports.yml KERNEL)
