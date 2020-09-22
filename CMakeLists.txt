cmake_minimum_required(VERSION 3.5)

project(MsgLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(shared shared.cpp shared.hpp)

add_executable(client client.cpp)
add_executable(server server.cpp)

target_link_libraries(client shared)
target_link_libraries(server shared)
