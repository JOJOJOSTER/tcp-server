cmake_minimum_required(VERSION 3.16)

project(tcp_server)

set(CMAKE_CXX_STANDARD 11)

# additional warnings
add_compile_options(-Wall -Wextra -Wpedantic "-std=c++11")

add_library(${PROJECT_NAME} 
  src/tcp_server.h 
  src/tcp_server.cpp)

option(SERVER_ECHO "Build SERVER_ECHO" ON)
option(SERVER_HTTP_RESPONDE "Build SERVER_HTTP_RESPONDE" ON)


if(SERVER_ECHO)
  add_executable(server_echo example/server_echo.cpp)
  target_link_libraries(server_echo ${PROJECT_NAME})
endif()

if(SERVER_HTTP_RESPONDE)
  add_executable(server_http_responde example/server_http_responde.cpp)
  target_link_libraries(server_http_responde ${PROJECT_NAME})
endif()
