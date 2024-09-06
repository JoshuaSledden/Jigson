#ifndef NESTED_OBJECT_MAPPING_HPP
#define NESTED_OBJECT_MAPPING_HPP

#include <string>
#include <iostream>
#include "..\jigson.hpp"
using namespace jigson;

// Address class for nested object
class Address {
public: // Make attributes public for direct access
  std::string street;
  std::string city;
};

// Person class containing Address
class Person {
public: // Make attributes public for direct access
  std::string name;
  int age;
  Address address;
};

// Specialization for Address class
template <>
inline void mapping::MapperFactory::configure_profile<Address>(mapping::MappingProfile<Address>& profile) {
  profile.add_mapping("street", [](Address& a, const json_payload& j) { a.street = j.get<std::string>(); });
  profile.add_mapping("city", [](Address& a, const json_payload& j) { a.city = j.get<std::string>(); });
}

// Specialization for Person class with nested Address
template <>
inline void mapping::MapperFactory::configure_profile<Person>(mapping::MappingProfile<Person>& profile) {
  profile.add_mapping("name", [](Person& p, const json_payload& j) { p.name = j.get<std::string>(); });
  profile.add_mapping("age", [](Person& p, const json_payload& j) { p.age = j.get<int>(); });
  // Map the nested Address object
  profile.add_mapping("address", [](Person& p, const json_payload& j) {
    p.address = mapping::get_mapper().map<Address>(j);
    });
}

// Mock main function
int main() {
  // Register profiles for both Address and Person classes
  mapping::MapperFactory::instance().register_profile<Address>();
  mapping::MapperFactory::instance().register_profile<Person>();

  // Create a JSON object with nested address data
  const json_payload json = {
      {"name", "Jane Doe"},
      {"age", 28},
      {"address", {
          {"street", "456 Park Ave"},
          {"city", "New York"}
      }}
  };

  // Create mapper and map JSON to Person object
  auto& mapper = mapping::get_mapper();
  Person person = mapper.map<Person>(json);

  // Alternatively you can just call
  person = mapping::map<Person>(json);

  // Output the mapped values, including nested address
  std::cout << "Name: " << person.name << std::endl;
  std::cout << "Age: " << person.age << std::endl;
  std::cout << "Address: " << person.address.street << ", " << person.address.city << std::endl;

  return 0;
}

#endif // NESTED_OBJECT_MAPPING_HPP