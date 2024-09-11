#pragma once
#include "Types.h"

namespace jigson {
  namespace mapping {
    template<typename T>
    class MappingProfile : public IMappingProfile {
      using MappingFunction = std::function<void(T&, const json_payload&)>;

      std::unordered_map<std::string, MappingFunction> mappings_;

    public:
      /// <summary>
      /// Add a function for mapping a specific key in a json object.
      /// </summary>
      /// <param name="json_key">The key of the json field.</param>
      /// <param name="func">The function for mapping that key to a value.</param>
      void add_mapping(const std::string& json_key, MappingFunction func) {
        mappings_[json_key] = std::move(func);
      }

      /// <summary>
      /// Perform the mapping action on a json payload to an object.
      /// </summary>
      /// <param name="json">The json payload we wish to map</param>
      /// <param name="object">The void object we're mapping the json content to.</param>
      void map(const json_payload& json, void* object) const override {
        T* typed_object = static_cast<T*>(object);

        for (const auto& [key, func] : mappings_) {
          if (json.contains(key)) {
            func(*typed_object, json[key]);
          }
        }
      }
    };
  }
}
