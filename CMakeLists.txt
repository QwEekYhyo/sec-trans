cmake_minimum_required(VERSION 3.10)
project(sec-trans VERSION 0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
link_directories(lib)

add_executable(test_server src/test_server.c)
target_link_libraries(test_server server)

add_executable(test_client src/test_client.c)
target_link_libraries(test_client client)
