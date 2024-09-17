#include <string>
#include <vector>
#include <gtest\gtest.h>

#include "..\Include\Jigson.hpp"
using namespace jigson;

// Example class to map JSON to
class Address {
public:
    std::string line1;
    std::string line2;
};

class Person {
public:
    std::string name;
    int age{0};
    bool is_adult{false};
    std::vector<std::string> hobbies{};
    Address address{};

    void set_age(int age) {
        this->age = age;
        this->is_adult = age >= 18;
    }
};

class Singleton {
public:
    static Singleton &get_instance() {
        static Singleton instance;
        return instance;
    }

    std::string str;
    int num;

private:
    Singleton() = default;

    ~Singleton() = default;

    Singleton(const Singleton &) = delete;

    Singleton &operator=(const Singleton &) = delete;
};

struct ComplexStruct {
    std::string str;
};

class InvalidObject {
public:
    ComplexStruct complex_type;
};

// Specialization for Address class
template<>
void mapping::MapperFactory::configure_profile<Address>(mapping::MappingProfile<Address> &profile) {
    profile
            .add_mapping("line1", &Address::line1)
            .add_mapping("line2", &Address::line2);
}

// Specialization for Person class
template<>
void mapping::MapperFactory::configure_profile<Person>(mapping::MappingProfile<Person> &profile) {
    profile
            .add_mapping("name", &Person::name)
            .add_mapping("age", &Person::age, [](Person &p, const json_payload &j) {
                const auto age = j.get<int>();
                p.age = age;
                p.is_adult = age >= 18;
            })
            .add_mapping("hobbies", &Person::hobbies)
            .add_mapping("address", &Person::address, [](Person &p, const json_payload &j) {
                p.address = mapping::get_mapper().map<Address>(j);
            });
}

// Specialization for Singleton class
template<>
void mapping::MapperFactory::configure_profile<Singleton>(mapping::MappingProfile<Singleton> &profile) {
    profile
            .add_mapping("str", &Singleton::str)
            .add_mapping("num", &Singleton::num);
}

// Specialization for Complex class
template<>
void mapping::MapperFactory::configure_profile<InvalidObject>(mapping::MappingProfile<InvalidObject> &profile) {
    profile.add_mapping("complex", &InvalidObject::complex_type);
}

// Test fixture
class JigsonTest : public ::testing::Test {
protected:
    void SetUp() override {
        mapping::MapperFactory::instance().register_profile<Address>();
        mapping::MapperFactory::instance().register_profile<Person>();
        mapping::MapperFactory::instance().register_profile<Singleton>();
        mapping::MapperFactory::instance().register_profile<InvalidObject>();
    }
};

TEST_F(JigsonTest, GIVEN_BasicJsonObject_WHEN_MapCalled_THEN_ExpectBasicObjectToBeMapped) {
    const json_payload json = {
        {"name", "John Doe"},
        {"age", 30},
        {"hobbies", {"reading", "swimming"}}
    };

    auto &mapper = mapping::get_mapper();
    const Person person = mapper.map<Person>(json);

    EXPECT_EQ(person.name, "John Doe");
    EXPECT_EQ(person.age, 30);
    EXPECT_EQ(person.hobbies.size(), 2);
    EXPECT_EQ(person.hobbies[0], "reading");
    EXPECT_EQ(person.hobbies[1], "swimming");
}

TEST_F(JigsonTest, GIVEN_InvalidTypeInJsonObject_WHEN_MapToCalled_THEN_ExpectRuntimeError) {
    const json_payload json = {
        {"complex", "I should fail hard"}
    };

    auto &mapper = mapping::get_mapper();

    // Expect a runtime_error when trying to map the invalid type
    EXPECT_THROW({
        try {
            const auto invalid = mapper.map<InvalidObject>(json);
        } catch (const std::runtime_error& e) {
            // Check if the error message contains the expected text
            EXPECT_STREQ(e.what(), "Invalid mapping provided for property complex");
            throw;
        }
    }, std::runtime_error);
}

TEST_F(JigsonTest, GIVEN_BasicJsonObject_WHEN_MapCalledWithMissingFields_THEN_MissingFieldsAreConstructedWithDefaultValues) {
    const json_payload json = {
        {"name", "Jane Doe"}
        // age and hobbies are missing
    };

    auto &mapper = mapping::get_mapper();
    const Person person = mapper.map<Person>(json);

    EXPECT_EQ(person.name, "Jane Doe");
    EXPECT_EQ(person.age, 0); // Assuming int default constructs to 0
    EXPECT_TRUE(person.hobbies.empty());
}

TEST_F(JigsonTest, GIVEN_BasicJsonObject_WHEN_MapCalledWithAdditionalFields_THEN_MissingFieldsAreIgnored) {
    const json_payload json = {
        {"name", "Bob Smith"},
        {"age", 25},
        {"hobbies", {"coding"}},
        {"extra", "This field is not in the Person class"}
    };

    auto &mapper = mapping::get_mapper();
    const Person person = mapper.map<Person>(json);

    EXPECT_EQ(person.name, "Bob Smith");
    EXPECT_EQ(person.age, 25);
    EXPECT_EQ(person.hobbies.size(), 1);
    EXPECT_EQ(person.hobbies[0], "coding");
    // The "extra" field should be ignored without causing any issues
}

TEST_F(JigsonTest, GIVEN_JsonObjectWithNestedAddress_WHEN_MapCalled_THEN_ExpectNestedObjectToBeMapped) {
    const json_payload json = {
        {"name", "John Doe"},
        {"age", 30},
        {"hobbies", {"reading", "swimming"}},
        {
            "address",
            {
                {"line1", "123 Main St"},
                {"line2", "Apt 4B"}
            }
        }
    };

    auto &mapper = mapping::get_mapper();
    const Person person = mapper.map<Person>(json);

    EXPECT_EQ(person.name, "John Doe");
    EXPECT_EQ(person.age, 30);
    EXPECT_EQ(person.hobbies.size(), 2);
    EXPECT_EQ(person.hobbies[0], "reading");
    EXPECT_EQ(person.hobbies[1], "swimming");
    EXPECT_EQ(person.address.line1, "123 Main St");
    EXPECT_EQ(person.address.line2, "Apt 4B");
}

TEST_F(JigsonTest, GIVEN_BasicJsonObject_WHEN_MapToCalled_THEN_ExpectSingletonToBeMapped) {
    const json_payload json = {
        {"str", "John Doe"},
        {"num", 30}
    };

    auto &mapper = mapping::get_mapper();
    mapper.map_to<Singleton>(json, Singleton::get_instance());

    EXPECT_EQ(Singleton::get_instance().str, "John Doe");
    EXPECT_EQ(Singleton::get_instance().num, 30);
}

class JigsonConditionalTests : public testing::TestWithParam<std::tuple<int, bool> > {
protected:
    void SetUp() override {
        mapping::MapperFactory::instance().register_profile<Address>();
        mapping::MapperFactory::instance().register_profile<Person>();
    }
};

// Test multiple input cases
TEST_P(JigsonConditionalTests, GIVEN_JsonObjectHasConditionalMapping_WHEN_MapCalled_THEN_ExpectConditionalMappingToBeCorrect) {
    int age, expected_is_adult;
    std::tie(age, expected_is_adult) = GetParam();

    const json_payload json = {
        {"name", "John Doe"},
        {"age", age}
    };

    auto &mapper = mapping::get_mapper();
    const Person person = mapper.map<Person>(json);

    EXPECT_EQ(person.age, age);
    EXPECT_EQ(person.is_adult, expected_is_adult);
}

INSTANTIATE_TEST_SUITE_P(
    IsAdultTests,
    JigsonConditionalTests,
    testing::Values(
        std::make_tuple(18, true),
        std::make_tuple(10, false),
        std::make_tuple(21, true),
        std::make_tuple(1, false)
    )
);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
