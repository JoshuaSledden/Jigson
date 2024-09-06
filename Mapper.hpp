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
      Mapper(const Mapper&) = delete;
      Mapper& operator=(const Mapper&) = delete;
      Mapper(Mapper&&) = default;
      Mapper& operator=(Mapper&&) = default;

      /// <summary>
      /// Register a mapping profile that the mapper can use.
      /// </summary>
      /// <typeparam name="T">The type of the object the JSON payload will be mapped to.</typeparam>
      /// <param name="profile">The mapping profile being registered.</param>
      template<typename T>
      void register_profile(std::unique_ptr<MappingProfile<T>> profile) {
        profiles_[typeid(T).name()] = std::move(profile);
      }

      /// <summary>
      /// Map a JSON payload to an object of type T
      /// </summary>
      /// <typeparam name="T">The object type which we are mapping to.</typeparam>
      /// <param name="json">The raw JSON payload.</param>
      /// <returns>An instance of type T with fields populated from the mapped JSON payload.</returns>
      template<typename T>
      T map(const json_payload& json) {
        T object;

        const auto found_profile = profiles_.find(typeid(T).name());
        if (found_profile != profiles_.end()) {
          found_profile->second->map(json, &object);
        }

        return object;
      }
    };
  }
}
