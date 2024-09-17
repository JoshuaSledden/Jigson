#ifndef OBJECT_MAPPING_WITH_LISTS_HPP
#define OBJECT_MAPPING_WITH_LISTS_HPP

#include <string>
#include <vector>
#include <iostream>
#include "..\Include\Jigson.hpp"
using namespace jigson;

// Class to represent a person with a list of hobbies
class Person {
public: // Make attributes public for direct access
  std::string name;
  int age;
  std::vector<std::string> hobbies;
};

// Specialization for Person class with a list of hobbies
template <>
inline void mapping::MapperFactory::configure_profile<Person>(mapping::MappingProfile<Person>& profile) {
  profile
    .add_mapping("name", &Person::name)
    .add_mapping("age", &Person::age)
    .add_mapping("hobbies", &Person::hobbies);
}

// Mock main function
int main() {
  // Register the profile for Person class
  mapping::MapperFactory::instance().register_profile<Person>();

  // Create a JSON object with a list of hobbies
  const json_payload json = {
      {"name", "Alice"},
      {"age", 25},
      {"hobbies", {"painting", "hiking", "cooking"}}
  };

  // Create mapper and map JSON to Person object
  auto& mapper = mapping::get_mapper();
  Person person = mapper.map<Person>(json);

  // Alternatively you can just call
  person = mapping::map<Person>(json);

  // Output the mapped values, including the list of hobbies
  std::cout << "Name: " << person.name << std::endl;
  std::cout << "Age: " << person.age << std::endl;
  std::cout << "Hobbies: ";
  for (const auto& hobby : person.hobbies) {
    std::cout << hobby << " ";
  }
  std::cout << std::endl;

  return 0;
}

#endif // OBJECT_MAPPING_WITH_LISTS_HPP