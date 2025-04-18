cmake_minimum_required(VERSION 3.10)

project(NikiScript VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

option(BUILD_SHARED "Whether make it shared or static" OFF)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

	option(BUILD_64BITS "x64 or x86" ON)
	if(BUILD_64BITS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
	endif()
endif()


file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

if(BUILD_SHARED)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)
	add_library(NikiScript SHARED ${SOURCES})
	set_target_properties(NikiScript PROPERTIES VERSION ${PROJECT_VERSION} POSITION_INDEPENDENT_CODE ON)
else()
    add_library(NikiScript STATIC ${SOURCES})
	set_target_properties(NikiScript PROPERTIES VERSION ${PROJECT_VERSION})
endif()

set_target_properties(NikiScript PROPERTIES PREFIX "")

if(MSVC)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(NIKISCRIPT_SUFFIX "-d")
	elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		set(NIKISCRIPT_SUFFIX "-rd")
	elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
		set(NIKISCRIPT_SUFFIX "-min")
	endif()

	if(CMAKE_SIZEOF_VOID_P STREQUAL 8)
		set(NIKISCRIPT_SUFFIX "64${NIKISCRIPT_SUFFIX}")
	endif()

	if(BUILD_SHARED)
		set_target_properties(NikiScript PROPERTIES SUFFIX "${NIKISCRIPT_SUFFIX}.dll")
	else()
		set_target_properties(NikiScript PROPERTIES SUFFIX "${NIKISCRIPT_SUFFIX}.lib")
	endif()
endif()

target_include_directories(NikiScript PRIVATE ${PROJECT_SOURCE_DIR}/include/NikiScript)

add_subdirectory(${PROJECT_SOURCE_DIR}/example)