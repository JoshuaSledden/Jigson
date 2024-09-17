#ifndef MAPPING_WITH_CONDITIONALS_HPP
#define MAPPING_WITH_CONDITIONALS_HPP

#include <string>
#include <iostream>
#include "..\Include\Jigson.hpp"
using namespace jigson;

// Class to represent a person with a conditional json_payload
class Person {
public: // Make attributes public for direct access
  std::string name;
  int age;
  bool is_adult; // Conditional json_payload based on age
};

// Specialization for Person class with conditional logic
template <>
void mapping::MapperFactory::configure_profile<Person>(mapping::MappingProfile<Person>& profile) {
  profile
      .add_mapping("name", &Person::name)
      .add_mapping("age", &Person::age, [](Person &p, const json_payload &j) {
              const auto age = j.get<int>();
              p.age = age;
              p.is_adult = age >= 18;
          });
}

// Mock main function
int main() {
  // Register the profile for Person class
  mapping::MapperFactory::instance().register_profile<Person>();

  // Create a JSON object with age that will affect the isAdult json_payload
  const json_payload json = {
      {"name", "Chris"},
      {"age", 17} // Not an adult
  };

  // Create mapper and map JSON to Person object
  auto& mapper = mapping::get_mapper();
  Person person = mapper.map<Person>(json);

  // Alternatively you can just call
  person = mapping::map<Person>(json);

  // Output the mapped values and conditional json_payload
  std::cout << "Name: " << person.name << std::endl;
  std::cout << "Age: " << person.age << std::endl;
  std::cout << "Is Adult: " << (person.is_adult ? "Yes" : "No") << std::endl;

  return 0;
}

#endif // MAPPING_WITH_CONDITIONALS_HPP