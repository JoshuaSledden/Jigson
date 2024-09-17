#pragma once
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "IMappingProfile.hpp"
#include "MappingProfile.hpp"
#include "Types.h"

namespace jigson {
    namespace mapping {
        class Mapper {
            std::unordered_map<std::string, std::unique_ptr<IMappingProfile>> profiles_;

        public:
            Mapper() = default;

            Mapper(const Mapper &) = delete;

            Mapper &operator=(const Mapper &) = delete;

            Mapper(Mapper &&) = default;

            Mapper &operator=(Mapper &&) = default;

            /**
             * Register a mapping profile that the mapper can use.
             *
             * @tparam T The type of the object the JSON payload will be mapped to.
             * @param profile The mapping profile being registered.
             */
            template<typename T>
            void register_profile(std::unique_ptr<MappingProfile<T>> profile) {
                profiles_[typeid(T).name()] = std::move(profile);
            }

            /**
             * Map a JSON payload to an object of type T
             *
             * @tparam T The object type which we are mapping to.
             * @param json The raw JSON payload.
             * @return An instance of type T with fields populated from the mapped JSON payload.
             */
            template<typename T>
            T map(const json_payload &json) {
                T object;

                const auto found_profile = profiles_.find(typeid(T).name());
                if (found_profile != profiles_.end()) {
                    found_profile->second->map(json, &object);
                }

                return object;
            }

            /**
             * Map a JSON payload to a pre-existing instance of type T
             *
             * @tparam T The object type which we are mapping to.
             * @param json The raw JSON payload.
             * @param object The pre-existing instance of T to which we are mapping.
             */
            template<typename T>
            void map_to(const json_payload &json, T &object) {
                const auto found_profile = profiles_.find(typeid(T).name());

                if (found_profile != profiles_.end()) {
                    found_profile->second->map(json, &object);
                }
            }
        };
    }
}
