cmake_minimum_required(VERSION 3.0)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(CEX2DEX)
include("${VITASDK}/share/vita.cmake" REQUIRED)
include("${VITASDK}/share/vita.self_sign.cmake" OPTIONAL RESULT_VARIABLE self_sign_module_path)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3 -fno-inline")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")
set(VITA_MAKE_FSELF_FLAGS "${VITA_MAKE_FSELF_FLAGS} -a 0x2808000000000001")

enable_language(C ASM)

add_executable(${PROJECT_NAME}
  src/main.c
  src/graphics.c
  src/font.c
  src/screen_clear.S
)

target_link_libraries(${PROJECT_NAME}
  SceAppMgr_stub
  SceCtrl_stub
  SceDisplay_stub
  ScePower_stub
  SceLibKernel_stub
  SceKernelThreadMgr_stub
  SceSysmem_stub
  SceVshBridge_stub
  taihen_stub
  ${CMAKE_CURRENT_BINARY_DIR}/../../build/Cex2Dex-Kernel/libCex2DexKernel_stub.a
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES LINK_FLAGS "-nostdlib"
  COMPILE_FLAGS "-D__PSP2_USER__"
)

vita_create_self(eboot.bin ${PROJECT_NAME}
  CONFIG exports.yml
  STRIPPED
  REL_OPTIMIZE
)

if(NOT ${self_sign_module_path} MATCHES "NOTFOUND")
  vita_self_sign(eboot.bin
    SDK_TYPE 0
    SDK_VERSION 0x3600000
    SELF_TYPE REF00D_USER
    ATTR ./config_attributes.txt
    CAPS ./config_capability.txt
  )
endif()
