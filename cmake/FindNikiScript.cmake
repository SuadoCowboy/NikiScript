# FindNikiScript.cmake
# Usage:
#   find_package(NikiScript REQUIRED)
#   target_link_libraries(MyTarget PRIVATE ${NIKISCRIPT_LIB})
#   target_include_directories(MyTarget PRIVATE ${NIKISCRIPT_INCLUDE})

find_path(NIKISCRIPT_INCLUDE
	NAMES NikiScript.h
	PATHS ${CMAKE_CURRENT_LIST_DIR}/../include/NikiScript
		  ${CMAKE_SOURCE_DIR}/include/NikiScript
		  $ENV{NIKISCRIPT_DIR}/include/NikiScript
)

if (WIN32)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(NIKISCRIPT_SUFFIX "-d")
	elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		set(NIKISCRIPT_SUFFIX "-rd")
	elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
		set(NIKISCRIPT_SUFFIX "-min")
	else()
		set(NIKISCRIPT_SUFFIX "")
	endif()

	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(_lib_path "lib/win64")
		set(NIKISCRIPT_SUFFIX "64${NIKISCRIPT_SUFFIX}")
	else()
		set(_lib_path "lib/win32")
	endif()

	find_library(NIKISCRIPT_LIB
		NAMES "NikiScript${NIKISCRIPT_SUFFIX}"
		PATHS ${CMAKE_SOURCE_DIR}/${_lib_path}
			  $ENV{NIKISCRIPT_DIR}
		NO_DEFAULT_PATH
	)

elseif(UNIX)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(NIKISCRIPT_SUFFIX "-d")
	elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		set(NIKISCRIPT_SUFFIX "-rd")
	elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
		set(NIKISCRIPT_SUFFIX "-min")
	else()
		set(NIKISCRIPT_SUFFIX "")
	endif()

	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(_lib_path "lib/linux64")
		set(NIKISCRIPT_SUFFIX "64${NIKISCRIPT_SUFFIX}")
	else()
		set(_lib_path "lib/linux32")
	endif()

	find_library(NIKISCRIPT_LIB
		NAMES "NikiScript${NIKISCRIPT_SUFFIX}"
		PATHS /usr/local/lib /usr/lib
				${CMAKE_SOURCE_DIR}/${_lib_path}
				$ENV{NIKISCRIPT_DIR}/lib
		NO_DEFAULT_PATH
	)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NikiScript REQUIRED_VARS NIKISCRIPT_LIB NIKISCRIPT_INCLUDE)