#pragma once
#include "Types.h"

namespace jigson {
  namespace mapping {
    class IMappingProfile {
    public:
      virtual ~IMappingProfile() = default;
      virtual void map(const json_payload& json, void* obj) const = 0;
    };
  }
}
