cmake_minimum_required(VERSION 3.29)
project(Jigson)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Jigson src/main.cpp)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(Jigson PRIVATE nlohmann_json::nlohmann_json)
