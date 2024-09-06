
# Jigson

Jigson is a header-only C++ library that facilitates mapping between JSON data and C++ class objects. The name is a play on "Jigsaw" and "JSON", reflecting its purpose of piecing class objects together from JSON data.

## Features

- Header-only library for easy integration
- Depends on nlohmann-json for JSON parsing
- Supports C++20
- Provides intuitive mapping between JSON and C++ objects

## Requirements

- C++20 compatible compiler
- nlohmann-json library

## Installation

### Manual Installation

1. Clone the repository:

    git clone[https://github.com/JoshuaSledden/Jigson.git](https://github.com/JoshuaSledden/Jigson.git)

2. Copy the `Jigson.hpp` file to your project's include directory.
3. Make sure you have nlohmann-json installed and properly linked in your project.

### Installation via vcpkg

You can install Jigson using vcpkg:

`vcpkg install jigson`

## Usage

Include the Jigson header in your C++ file:

```cpp
#include <jigson.hpp>
```


For detailed usage examples, please refer to the `examples` directory in this repository.## Examples

The `examples` directory contains sample code demonstrating how to use Jigson for various JSON mapping scenarios. These examples include:1. Simple object mapping

1. Nested object mapping
2. Object mapping with lists
3. Mapping with conditionals based on other field values

## Tests

The `tests` directory contains unit tests for Jigson. These tests use the Google Test framework and cover various aspects of the library's functionality.To run the tests:1. Make sure you have Google Test installed

1. Build the test project
2. Run the resulting test executable

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](https://www.perplexity.ai/search/LICENSE) file for details.

This README provides an overview of Jigson, its features, installation methods (both manual and via vcpkg), basic usage, and information about the examples and tests. You may want to adjust some details based on your specific repository structure or add more sections as needed.
