cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(NikiScript VERSION 0.0.1 LANGUAGES CXX)

option(BUILD_64BITS "x64 or x86" ON)
option(BUILD_SHARED "Whether make it shared or static" ON)

if (BUILD_64BITS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
endif()

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

if(BUILD_SHARED)
    add_library(NikiScript SHARED ${SOURCES})
    set_target_properties(NikiScript PROPERTIES VERSION ${PROJECT_VERSION} POSITION_INDEPENDENT_CODE ON)
else()
    add_library(NikiScript STATIC ${SOURCES})
endif()

target_include_directories(NikiScript PRIVATE ${PROJECT_SOURCE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/example)
