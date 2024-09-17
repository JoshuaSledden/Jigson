#pragma once
#include <concepts>
#include <string>
#include <vector>
#include <type_traits>
#include "Types.h"

namespace jigson {
    namespace mapping {
        template<typename T>
        concept DirectlyMappableType =
                std::is_same_v<T, std::string> ||
                std::is_arithmetic_v<T> ||
                std::is_same_v<T, std::vector<std::string> > ||
                (std::is_same_v<std::remove_cvref_t<T>, std::vector<typename T::value_type> > &&
                 (std::is_arithmetic_v<typename T::value_type> ||
                  std::is_same_v<typename T::value_type, std::string>));

        template<typename T>
        class MappingProfile : public IMappingProfile {
            using MappingFunction = std::function<void(T &, const json_payload &)>;

            std::unordered_map<std::string, MappingFunction> mappings_;

        public:
            /**
             *  Add a function for mapping a specific key in a json object.
             *
             * @tparam PropertyType Type of the member being mapped to
             * @param json_key The key of the json field.
             * @param member_ptr A pointer to the member variable being mapped to
             * @param converter An optional custom converter method for more complex mapping scenarios
             */
            template<typename PropertyType>
            auto &add_mapping(const std::string &json_key, PropertyType T::*member_ptr, MappingFunction converter = nullptr) {
                if (converter) {
                    mappings_[json_key] = std::move(converter);
                } else {
                    mappings_[json_key] = [member_ptr, json_key](T &obj, const json_payload &j) {
                        if constexpr (DirectlyMappableType<PropertyType>) {
                            obj.*member_ptr = j.get<PropertyType>();
                        } else {
                            throw std::runtime_error("Invalid mapping provided for property " + json_key);
                        }
                    };
                }
                return *this;
            }

            /**
             * Add a function for mapping a specific key in a json object.
             *
             * @param json_key The key of the json field.
             * @param func The function for mapping that key to a value.
             */
            auto &add_mapping(const std::string &json_key, MappingFunction func) {
                mappings_[json_key] = std::move(func);
                return *this;
            }

            /**
             * Perform the mapping action on a json payload to an object.
             *
             * @param json The json payload we wish to map
             * @param object The void object we're mapping the json content to.
             */
            void map(const json_payload &json, void *object) const override {
                T *typed_object = static_cast<T *>(object);

                for (const auto &[key, func]: mappings_) {
                    if (json.contains(key)) {
                        func(*typed_object, json[key]);
                    }
                }
            }
        };
    }
}
