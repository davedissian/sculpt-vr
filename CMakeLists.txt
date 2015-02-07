cmake_minimum_required(VERSION 2.8)
project(sculptvr)

include(FindPkgConfig)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

# Enable C++11
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    # If on OS X, use the libc++ standard library which is much more c++11 complete
    if(APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        if(XCODE)
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
        endif()
    endif()
endif()

# Dependencies
set(OVR_SDK_ROOT "${CMAKE_SOURCE_DIR}/dependencies/OculusSDK")
set(LEAP_SDK_ROOT "${CMAKE_SOURCE_DIR}/dependencies/LeapSDK")
find_package(OVR REQUIRED)
find_package(Leap REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${OVR_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${LEAP_INCLUDE_DIRS})

# Sources
set(SRC
  main.cc
  mesh.cc
  shader.cc
  volume.cc)

if (LINUX)
  set(PLATFORM_LIBS X11 GL rt Xrandr GLEW)
else (APPLE)
  set(PLATFORM_LIBS SDL2)
endif()

# Executable
add_executable(sculptvr ${SRC})
target_link_libraries(
  sculptvr 
  ${OVR_LIBRARIES} 
  ${SDL2_LIBRARIES} 
  ${LEAP_LIBRARIES}
  ${PLATFORM_LIBS})
