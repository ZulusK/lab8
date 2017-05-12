cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")

file(GLOB LAB_SOURCES src/*.cpp include/*.h)
include_directories(include)
project(lab8)
add_executable(${CMAKE_PROJECT_NAME} ${LAB_SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} jansson)
target_link_libraries(${CMAKE_PROJECT_NAME} progbase-cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} progbase)
target_link_libraries(${CMAKE_PROJECT_NAME} ssl)
target_link_libraries(${CMAKE_PROJECT_NAME} pthread)