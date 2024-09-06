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
    static void register_profile() {
      MapperFactory::instance().register_profile<T>();
    }
  }
}
