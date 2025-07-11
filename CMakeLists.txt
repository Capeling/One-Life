cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(OneLife VERSION 1.0.0)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp ${OS_SOURCES})
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if(DEFINED ENV{GITHUB_ACTIONS})
# Enable LTO for smaller binaries
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
target_compile_definitions(${PROJECT_NAME} PRIVATE GITHUB_ACTIONS=1)
message(STATUS "GitHub Actions build detected")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE src)

# Debug macro (Taken from EclipseMenu)
if (NOT DEFINED ENV{GITHUB_ACTIONS})
  target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
      $<$<CONFIG:Debug>:DEBUG_BUILD>
      $<$<CONFIG:RelWithDebInfo>:DEBUG_BUILD>
  )
endif()

execute_process(
	COMMAND git describe --always --dirty
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE ONELIFE_COMMIT_HASH
	OUTPUT_STRIP_TRAILING_WHITESPACE
	ERROR_QUIET
)

if (NOT ONELIFE_COMMIT_HASH)
	set(ONELIFE_COMMIT_HASH "(?)")
endif()

configure_file(src/BuildConstants.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/BuildConstants.hpp)

if (NOT DEFINED ENV{GEODE_SDK})
message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)


setup_geode_mod(${PROJECT_NAME})