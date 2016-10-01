project(aaetherengine)
cmake_minimum_required(VERSION 2.8)



#load modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
find_package(aae_utils REQUIRED)



#include directories of the project
include_directories(${CMAKE_SOURCE_DIR}/aae_utils)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -nostartfiles -nostdlib -fno-exceptions")



if (CMAKE_SYSTEM_NAME STREQUAL Linux)
aae_add_source(asm/x64/linux/aae_main.s)
endif()



#add all source files here
aae_add_source(main.c)
#-------------------------



#create target
set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)
add_executable(${PROJECT_NAME} ${SOURCES})



#things to do if platform is linux
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
find_library(AAE_ALLOCATOR_LIB NAMES libaae_allocator.so PATHS ${CMAKE_BINARY_DIR}/lib)
target_link_libraries (${PROJECT_NAME} ${AAE_ALLOCATOR_LIB})
add_definitions(-DAAE_LINUX_PLATFORM)
endif()



#have some flag defined for corresponding builds
if (CMAKE_BUILD_TYPE STREQUAL Debug)
add_definitions(-DAAE_DEBUG_BUILD)
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
add_definitions(-DAAE_RELEASE_BUILD)
endif() 
