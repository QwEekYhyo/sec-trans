cmake_minimum_required(VERSION 3.10)
project(sec-trans VERSION 0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(COMMON_SOURCES src/message.c)
include_directories(include)
link_directories(lib)

add_executable(sectrans-server src/server.c ${COMMON_SOURCES})
target_link_libraries(sectrans-server server client)

add_executable(test_client src/test_client.c ${COMMON_SOURCES})
target_link_libraries(test_client server client)
