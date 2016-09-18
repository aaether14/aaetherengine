project(aaetherengine)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
cmake_minimum_required(VERSION 2.8)


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
find_package(aae_utils REQUIRED)


include_directories(${CMAKE_SOURCE_DIR}/aae_utils)


#add all sources file here
aae_add_source(main.c)
#-------------------------


set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)
add_executable(${PROJECT_NAME} ${SOURCES})




if(CMAKE_SYSTEM_NAME STREQUAL Linux)
find_library(AAE_ALLOCATOR_LIB NAMES libaae_allocator.so PATHS ${CMAKE_BINARY_DIR}/lib)
target_link_libraries (${PROJECT_NAME} -lGL -lX11 ${AAE_ALLOCATOR_LIB})
endif(CMAKE_SYSTEM_NAME STREQUAL Linux)
