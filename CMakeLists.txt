cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(NikiScript VERSION 0.0.1 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

if(BUILD_SHARED_LIBS)
    add_library(NikiScript SHARED ${SOURCES})
    set_target_properties(NikiScript PROPERTIES VERSION ${PROJECT_VERSION} POSITION_INDEPENDENT_CODE 1)
else()
    add_library(NikiScript STATIC ${SOURCES})
endif()

target_include_directories(NikiScript PRIVATE ${PROJECT_SOURCE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/example)
