cmake_minimum_required(VERSION 3.26)
project(Jigson LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

# Integrate vcpkg
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "")
endif()

# Find the nlohmann_json package
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(Jigson src/Main.cpp)

# Set output name
set_target_properties(Jigson PROPERTIES OUTPUT_NAME "Jigson")

# Link dependency packages
target_link_libraries(Jigson PRIVATE nlohmann_json::nlohmann_json)