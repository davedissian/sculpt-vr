cmake_minimum_required(VERSION 2.8)
project(sculptvr)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

# Dependencies
set(OVR_SDK_ROOT "${CMAKE_SOURCE_DIR}/dependencies/OculusSDK")
find_package(OVR REQUIRED)
include_directories(${OVR_INCLUDE_DIRS})

# Sources
set(SRC
  main.cc
  mesh.cc
  shader.cc
  volume.cc)

# Executable
add_executable(sculptvr ${SRC})
target_link_libraries(sculptvr ${OVR_LIBRARIES})
