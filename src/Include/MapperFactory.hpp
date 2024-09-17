#pragma once
#include <format>
#include "Mapper.hpp"
#include "MappingProfile.hpp"

namespace jigson {
    namespace mapping {
        class MapperFactory {
            Mapper mapper_;

        public:
            static MapperFactory &instance() {
                static MapperFactory instance;
                return instance;
            }

            /**
             * Get the mapper instance.
             *
             * @return The mapper instance.
             */
            Mapper &mapper() {
                return mapper_;
            }

            /**
             * Register a mapping profile for the mapper to use.
             *
             * @tparam T The type of the object the json object will be mapped to.
             */
            template<typename T>
            void register_profile() {
                auto profile = std::make_unique<MappingProfile<T> >();
                configure_profile(*profile);
                mapper_.register_profile(std::move(profile));
            }

        private:
            MapperFactory() = default;

            ~MapperFactory() = default;

            MapperFactory(const MapperFactory &) = delete;

            MapperFactory &operator=(const MapperFactory &) = delete;

            /**
             * A function for enabling specialized configuration.
             *
             * @tparam T The type of object we want to configure.
             * @param profile The profile for the given class.
             */
            template<typename T>
            void configure_profile(MappingProfile<T> &profile) {
                throw std::runtime_error(std::format("Mapping Profile does not exist for: {}", typeid(T).name()));
            }
        };
    }
}
