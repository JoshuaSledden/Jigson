#include <iostream>
#include "Include\Jigson.hpp"
using namespace jigson;

// An example class we want to map to
class TestClass {
public:
    std::string name;
    int value;
};

// Specialized mapping profile configuration.
// Good practice would be to put this in it's own header file or inside the same file as the TestClass header file.
template<>
inline void mapping::MapperFactory::configure_profile<TestClass>(MappingProfile<TestClass>& profile) {
    // Map JSON key "name" to TestClass.name
    profile.add_mapping("name", [](TestClass& test_class, const json_payload& json) {
      test_class.name = json.get<std::string>();
    });

    // Map JSON key "value" to TestClass.value
    profile.add_mapping("value", [](TestClass& test_class, const json_payload& json) {
      test_class.value = json.get<int>();
    });
}

int main() {
    // Register a mapping profile for a class,
    mapping::register_profile<TestClass>();

    // Example JSON object (Can also parse a file)
    json_payload json = {
        {"name", "Hello, Jigson!"},
        {"value", 42}
    };

    // Map JSON to the class type
    const auto [name, value] = mapping::map<TestClass>(json);

    std::cout << name << ". You are " << value << " Years Old." << std::endl;
    return 0;
}