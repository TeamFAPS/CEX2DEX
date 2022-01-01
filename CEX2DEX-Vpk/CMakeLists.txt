cmake_minimum_required(VERSION 3.0)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "CEX2DEX")
set(VITA_TITLEID  "CXDX00001")
set(VITA_VERSION  "01.00")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1 -s CATEGORY=gd -s CONTENT_ID=HB0000-${VITA_TITLEID}_00-0000000000000000")

vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${CMAKE_CURRENT_BINARY_DIR}/../CEX2DEX-Installer/eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE ${CMAKE_CURRENT_BINARY_DIR}/../CEX2DEX-Kernel/cex2dex.skprx cex2dex.skprx
)
