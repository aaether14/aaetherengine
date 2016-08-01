project(aaetherengine)
cmake_minimum_required(VERSION 2.8)


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
find_package(aae_utils REQUIRED)


aae_add_source(main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})


if(CMAKE_SYSTEM_NAME STREQUAL Linux)
target_link_libraries (${PROJECT_NAME} -lGL -lX11)
endif(CMAKE_SYSTEM_NAME STREQUAL Linux)
