cmake_minimum_required(VERSION 2.8.8)
project(libfoo)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(PCAP REQUIRED)
include_directories(${PCAP_INCLUDE_DIR})

add_library(foo SHARED
   src/lib.cpp
   src/lib.h
)

target_link_libraries(foo ${PCAP_LIBRARIES})

install(
   TARGETS foo
   EXPORT libfooTargets
   LIBRARY DESTINATION lib
)

include(CMakePackageConfigHelpers)

set(CONF_INCLUDE_DIRS "include")
configure_package_config_file(
   cmake/libfooConfig.cmake.in
   "${PROJECT_BINARY_DIR}/libfooConfig.cmake"
   INSTALL_DESTINATION CMake
   PATH_VARS CONF_INCLUDE_DIRS
)

export(
   TARGETS foo
   FILE "${PROJECT_BINARY_DIR}/libfooTargets.cmake"
)

export(PACKAGE libfoo)

install(
   FILES
   "${PROJECT_BINARY_DIR}/libfooConfig.cmake"
   DESTINATION CMake
)

install(
   EXPORT libfooTargets
   DESTINATION CMake
)
