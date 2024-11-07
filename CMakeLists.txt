cmake_minimum_required(VERSION 3.16)

project(tcp_server)

set(CMAKE_CXX_STANDARD 11)

    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic "-std=c++11")

add_library(${PROJECT_NAME} 
  src/tcp_server.h 
  src/tcp_server.cpp)

add_executable(server server.cpp)

target_link_libraries(server ${PROJECT_NAME})

add_executable(server_echo example/server_echo.cpp)
target_link_libraries(server_echo ${PROJECT_NAME})



