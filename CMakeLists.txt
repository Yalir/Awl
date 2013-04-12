
cmake_minimum_required (VERSION 2.8)


# set a default build type if none was provided
# this has to be done before the project() instruction!
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project (Awl)

set(BUILD_SAMPLES TRUE CACHE BOOLEAN "Choose whether to build the sample programs")

# detect the OS
if(${CMAKE_HOST_WIN32})
	set(WINDOWS 1)
elseif(${CMAKE_HOST_APPLE})
	set(MACOSX 1)
elseif(${CMAKE_HOST_UNIX})
	set(LINUX 1)
else()
	message(FATAL_ERROR "Unsupported operating system")
	return()
endif()

# Set some env vars
set (LIB_NAME Awl)
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (VERSION_PATCH 1)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
#	add_definitions("-O2")
endif()


file(GLOB SOURCE_FILES RELATIVE ${PROJECT_SOURCE_DIR} "src/Awl/*.cpp")
file(GLOB UNIX_SOURCE_FILES RELATIVE ${PROJECT_SOURCE_DIR} "src/Awl/Unix/*.cpp")

# Define the source files sfe should compile
include_directories("include" "src")
add_library (${LIB_NAME} SHARED ${SOURCE_FILES} ${UNIX_SOURCE_FILES})



set_target_properties(${LIB_NAME} PROPERTIES
					  LINK_FLAGS "-Wl,-rpath='$ORIGIN'"
					  SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.0"
					  VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")


if (${BUILD_SAMPLES})
	add_subdirectory(test)
endif()


