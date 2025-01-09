cmake_minimum_required(VERSION 3.10)
project(sec-trans VERSION 0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SERVER_SOURCES src/server.c src/file.c)
set(COMMON_SOURCES src/message.c)
include_directories(include)
link_directories(lib)

add_executable(sectrans-server ${SERVER_SOURCES} ${COMMON_SOURCES})
target_link_libraries(sectrans-server server client)

add_executable(sectrans-client src/client.c ${COMMON_SOURCES})
target_link_libraries(sectrans-client server client)
