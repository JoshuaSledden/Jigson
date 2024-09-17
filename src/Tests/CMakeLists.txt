cmake_minimum_required(VERSION 3.26)
project(JigsonTests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

# Integrate vcpkg
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "")
endif()

include(GoogleTest)

add_executable(JigsonTests Jigson.Tests.cpp)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(JigsonTests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

gtest_discover_tests(JigsonTests)

# Set output name
set_target_properties(JigsonTests PROPERTIES OUTPUT_NAME "JigsonTests")

# Link dependency packages
target_link_libraries(JigsonTests INTERFACE nlohmann_json::nlohmann_json)