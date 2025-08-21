add_executable(BasicExample ${PROJECT_SOURCE_DIR}/examples/BasicExample/src/Main.cpp)
if (TARGET BasicExample)
	file(COPY ${PROJECT_SOURCE_DIR}/examples/BasicExample/data/cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
endif()

target_include_directories(BasicExample PRIVATE ${PROJECT_SOURCE_DIR}/include)
if (BUILD_SHARED)
	target_compile_definitions(BasicExample PRIVATE NS_IMPORT=1)
endif()
target_link_libraries(BasicExample NikiScript)

add_executable(JobSystem ${PROJECT_SOURCE_DIR}/examples/JobSystem/src/Main.cpp)

target_include_directories(JobSystem PRIVATE ${PROJECT_SOURCE_DIR}/include)
if (BUILD_SHARED)
	target_compile_definitions(JobSystem PRIVATE NS_IMPORT=1)
endif()
target_link_libraries(JobSystem NikiScript)