# - Find the Oculus Rift SDK
# This module defines the following variables:
#  OVR_INCLUDE_DIRS - include directories for OVR
#  OVR_LIBRARIES - libraries to link against OVR
#  OVR_FOUND - true if OVR has been found and can be used
#  OVR_DEFINITIONS - defines OVR_ENABLED

find_path(OVR_INCLUDE_DIR OVR.h
  PATH_SUFFIXES "LibOVR/Include"
  PATHS ${OVR_SDK_ROOT})

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  if(WIN32)
		if(MINGW)
      set(OVR_PATH_SUFFIX "LibOVR/Lib/MinGW/Release/w64")
		else()
      set(OVR_PATH_SUFFIX "LibOVR/Lib/x64")
		endif()
  elseif(APPLE)
    set(OVR_PATH_SUFFIX "LibOVR/Lib/Mac/Release/")
  else()
    set(OVR_PATH_SUFFIX "LibOVR/Lib/Linux/Release/x86_64")
  endif()
else()
  if(WIN32)
		if(MINGW)
      set(OVR_PATH_SUFFIX "LibOVR/Lib/MinGW/Release/w32")
		else()
      set(OVR_PATH_SUFFIX "LibOVR/Lib/Win32")
		endif()
	else()
    set(OVR_PATH_SUFFIX "LibOVR/Lib/Linux/Release/i386")
  endif()
endif()

find_library(OVR_LIBRARY NAMES OVR ovr libOVR libovr
	PATH_SUFFIXES ${OVR_PATH_SUFFIX}
  PATHS ${OVR_SDK_ROOT})

set(OVR_INCLUDE_DIRS ${OVR_INCLUDE_DIR})
set(OVR_LIBRARIES ${OVR_LIBRARY} )
set(OVR_DEFINITIONS "-DOVR_ENABLED")

find_package(Threads REQUIRED)
set(OVR_LIBRARIES ${OVR_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
if(APPLE)
  find_package(OpenGL REQUIRED)
  find_library(COCOA_LIBRARY NAMES Cocoa)
  find_library(IOKIT_LIBRARY NAMES IOKit)
  set(OVR_LIBRARIES ${OVR_LIBRARIES} ${OPENGL_LIBRARIES} ${COCOA_LIBRARY} ${IOKIT_LIBRARY})
elseif(LINUX)
	find_package(X11 REQUIRED)
  find_package(Xinerama REQUIRED)
	find_package(UDev REQUIRED)
  set(OVR_INCLUDE_DIRS ${OVR_INCLUDES} ${XINERAMA_INCLUDE_DIR} ${X11_INCLUDE_DIR})
	set(OVR_LIBRARIES ${OVR_LIBRARIES} ${XINERAMA_LIBRARIES} ${UDEV_LIBRARIES} ${X11_LIBRARIES})
endif()

find_package_handle_standard_args(OVR REQUIRED_VARS OVR_INCLUDE_DIR OVR_LIBRARY)

mark_as_advanced(OVR_INCLUDE_DIR OVR_LIBRARY)
