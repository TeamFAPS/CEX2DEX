cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{MEPSDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{MEPSDK}/mep-elf/share/mep-toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define MEPSDK to point to your SDK path!")
  endif()
endif()

project(mep_payload C ASM)
#include("${MEPSDK}/mep-elf/share/mep-toolchain.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-delete-null-pointer-checks -nostdlib -fno-optimize-sibling-calls -Os -std=gnu99 -Wall -Wextra -fno-inline")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_CURRENT_SOURCE_DIR}/linker.x -nostartfiles -nodefaultlibs -nostdlib")

include_directories(
  ./
)

add_executable(mep_payload
  main.c
)

target_link_libraries(mep_payload
)

add_custom_command(TARGET mep_payload
  COMMAND ${CMAKE_OBJCOPY} -S -O binary mep_payload mep_payload.bin
  DEPENDS mep_payload
)

add_custom_target(mep_payload_binary DEPENDS mep_payload)