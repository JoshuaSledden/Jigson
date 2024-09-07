#pragma once
#include "Mapper.hpp"
#include "MapperFactory.hpp"
#include "Types.h"

namespace jigson {
  namespace mapping {
    static Mapper& get_mapper() {
      return MapperFactory::instance().mapper();
    }

    template<typename T>
    static T map(const json_payload& json) {
      return MapperFactory::instance().mapper().map<T>(json);
    }

    template<typename T>
    static T map(const std::string& content) {
      const auto json_data = nlohmann::json::parse(content);
      return MapperFactory::instance().mapper().map<T>(json_data);
    }

    template<typename T>
    static void map_to(const json_payload& json, T& object) {
      MapperFactory::instance().mapper().map_to<T>(json, object);
    }

    template<typename T>
    static void map_to(const std::string& content, T& object) {
      const auto json_data = nlohmann::json::parse(content);
      MapperFactory::instance().mapper().map_to<T>(json_data, object);
    }

    template<typename T>
    static void register_profile() {
      MapperFactory::instance().register_profile<T>();
    }
  }
}
