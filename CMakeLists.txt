#project name and version
project(aaetherengine)
cmake_minimum_required(VERSION 2.8)



#load modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
find_package(aae_utils REQUIRED)



#include directories of the project
include_directories(${CMAKE_SOURCE_DIR}/aae_utils)
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -nostartfiles -nostdlib -fno-exceptions -fno-asynchronous-unwind-tables")



#tell the compiler which architecture the build is for
if (AAE_CPU_ARCHITECTURE STREQUAL aae_32bit)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
elseif(AAE_CPU_ARCHITECTURE STREQUAL aae_64bit)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
endif()



#add sources required for linux build
if (CMAKE_SYSTEM_NAME STREQUAL Linux)
aae_add_source(build/license.o)
if (AAE_CPU_ARCHITECTURE STREQUAL aae_32bit)
aae_add_source(asm/x86/linux/aae_main.s)
elseif(AAE_CPU_ARCHITECTURE STREQUAL aae_64bit)
aae_add_source(asm/x64/linux/aae_main.s)
endif()
endif()



#add all source files here
aae_add_source(aae_utils/assert/aae_assert.c)
aae_add_source(aae_utils/string/aae_string.c)
aae_add_source(aae_utils/error/aae_error.c)
aae_add_source(aae_utils/aae_allocators.c)
aae_add_source(aae_main.c)
#-------------------------



#create target
set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)
add_executable(${PROJECT_NAME} ${SOURCES})



#things to do if platform is linux
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
find_library(AAE_ALLOCATOR_LIB NAMES libaae_allocator.so PATHS ${CMAKE_BINARY_DIR}/lib)
if (AAE_MULTITHREADED STREQUAL Yes)
find_package(Threads)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
add_definitions(-DAAE_MULTITHREADED)
endif()
target_link_libraries (${PROJECT_NAME} ${AAE_ALLOCATOR_LIB})
add_definitions(-DAAE_LINUX_PLATFORM)
endif()



#have some flag defined for corresponding builds
if (CMAKE_BUILD_TYPE STREQUAL Debug)
add_definitions(-DAAE_DEBUG_BUILD)
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
add_definitions(-DAAE_RELEASE_BUILD)
endif()



#have some flah defined for corresponding architecture
if (AAE_CPU_ARCHITECTURE STREQUAL aae_32bit)
add_definitions(-DAAE_32BIT_BUILD)
elseif(AAE_CPU_ARCHITECTURE STREQUAL aae_64bit)
add_definitions(-DAAE_64BIT_BUILD)
endif()





