#ifndef SIMPLE_OBJECT_MAPPING_HPP
#define SIMPLE_OBJECT_MAPPING_HPP

#include <string>
#include <iostream>

#include "..\Include\Jigson.hpp"
using namespace jigson;

// Simple class to map JSON to
class Person {
public: // Make attributes public for direct access
  std::string name;
  int age;
};

// Specialization for Person class
// This tells the mapper how to map JSON fields to Person attributes
template <>
inline void mapping::MapperFactory::configure_profile<Person>(mapping::MappingProfile<Person>& profile) {
  profile
    .add_mapping("name", &Person::name)
    .add_mapping("age", &Person::age);
}

int main() {
  // Register the profile for Person class
  mapping::MapperFactory::instance().register_profile<Person>();

  // Create a JSON object with person data
  const json_payload json = {
      {"name", "John Doe"},
      {"age", 30}
  };

  // Create mapper and map JSON to Person object
  auto& mapper = mapping::get_mapper();
  Person person = mapper.map<Person>(json);

  // Alternatively you can just call
  person = mapping::map<Person>(json);

  // Output the mapped values
  std::cout << "Name: " << person.name << std::endl;
  std::cout << "Age: " << person.age << std::endl;

  return 0;
}

#endif // SIMPLE_OBJECT_MAPPING_HPP