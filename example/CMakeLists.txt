add_executable(Example ${PROJECT_SOURCE_DIR}/example/src/example.cpp)

file(COPY ${PROJECT_SOURCE_DIR}/example/data/cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

target_include_directories(Example PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(Example PRIVATE NS_IMPORT=1)
target_link_libraries(Example NikiScript)