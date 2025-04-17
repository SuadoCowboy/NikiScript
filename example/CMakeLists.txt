add_executable(Example ${PROJECT_SOURCE_DIR}/example/src/example.cpp)

file(COPY ${PROJECT_SOURCE_DIR}/example/cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(Example PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(Example NikiScript)