cmake_minimum_required(VERSION 3.10)

project(jigson)

include(GNUInstallDirs)

# Find the fmt library
find_package(fmt CONFIG REQUIRED)

# Add your library
add_library(jigson jigson.cpp)

if (BUILD_SHARED_LIBS AND MSVC)
    target_compile_definitions(jigson PRIVATE MYLIB_EXPORTS)
endif()

# Link your library to nlohmann_json
target_link_libraries(jigson PRIVATE nlohmann_json::nlohmann_json)


# Add include directories
target_include_directories(jigson PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>   # for headers when building
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>  # for client in install mode
)
# Install the library and its headers
install(TARGETS jigson
        EXPORT jigson_targets
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

install(FILES jigson.h DESTINATION include)


# Generate and install *-targets.cmake 
install(EXPORT jigson_targets
        FILE jigson-targets.cmake
        NAMESPACE jigson::
        DESTINATION share/jigson)

# Generate the config file in the current binary dir (this ensures it's not placed directly in source)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/jigson-config.cmake"
        "include(CMakeFindDependencyMacro)\n"
        "find_dependency(nlohmann_json CONFIG REQUIRED)\n"
        "include(\"\${CMAKE_CURRENT_LIST_DIR}/jigson-targets.cmake\")\n"
)

# Install the generated config file
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/jigson-config.cmake"
        DESTINATION share/jigson)