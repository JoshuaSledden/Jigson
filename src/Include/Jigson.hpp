#pragma once
#include "Mapper.hpp"
#include "MapperFactory.hpp"
#include "Types.h"

namespace jigson {
    namespace mapping {
        /**
         * Get the global mapper instance.
         *
         * @return Reference to the global Mapper instance.
         */
        static Mapper &get_mapper() {
            return MapperFactory::instance().mapper();
        }

        /**
         * Map a JSON payload to an object of type T.
         *
         * @tparam T The type of object to map to.
         * @param json The JSON payload to map from.
         * @return An instance of T populated with data from the JSON.
         */
        template<typename T>
        static T map(const json_payload &json) {
            return MapperFactory::instance().mapper().map<T>(json);
        }

        /**
         * Map a JSON string to an object of type T.
         *
         * @tparam T The type of object to map to.
         * @param content The JSON string to map from.
         * @return An instance of T populated with data from the JSON string.
         */
        template<typename T>
        static T map(const std::string &content) {
            const auto json_data = nlohmann::json::parse(content);
            return MapperFactory::instance().mapper().map<T>(json_data);
        }

        /**
         * Map a JSON payload to a pre-existing object of type T.
         *
         * @tparam T The type of the object being mapped to.
         * @param json The JSON payload to map from.
         * @param object The pre-existing object to map into.
         */
        template<typename T>
        static void map_to(const json_payload &json, T &object) {
            MapperFactory::instance().mapper().map_to<T>(json, object);
        }

        /**
         * Map a JSON string to a pre-existing object of type T.
         *
         * @tparam T The type of the object being mapped to.
         * @param content The JSON string to map from.
         * @param object The pre-existing object to map into.
         */
        template<typename T>
        static void map_to(const std::string &content, T &object) {
            const auto json_data = nlohmann::json::parse(content);
            MapperFactory::instance().mapper().map_to<T>(json_data, object);
        }

        /**
         * Register a mapping profile for type T.
         *
         * @tparam T The type for which to register a mapping profile.
         */
        template<typename T>
        static void register_profile() {
            MapperFactory::instance().register_profile<T>();
        }
    }
}
