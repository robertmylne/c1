cmake_minimum_required(VERSION 3.6)
project(assignment1)

set(SOURCE_FILES server.c client.c playground.c)
add_executable(assignment1 ${SOURCE_FILES})