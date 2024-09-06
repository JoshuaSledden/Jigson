#pragma once
#include "Mapper.hpp"
#include "MappingProfile.hpp"

namespace jigson {
  namespace mapping {
    class MapperFactory {
      Mapper mapper_;

    public:
      static MapperFactory& instance() {
        static MapperFactory instance;
        return instance;
      }

      /// <summary>
      /// Get the mapper instance.
      /// </summary>
      /// <returns>The mapper instance.</returns>
      Mapper& mapper() {
        return mapper_;
      }

      /// <summary>
      /// Register a mapping profile for the mapper to use.
      /// </summary>
      /// <typeparam name="T">The type of the object the json object will be mapped to.</typeparam>
      template<typename T>
      void register_profile() {
        auto profile = std::make_unique<MappingProfile<T>>();
        configure_profile(*profile);
        mapper_.register_profile(std::move(profile));
      }

    private:
      MapperFactory() = default;
      ~MapperFactory() = default;
      MapperFactory(const MapperFactory&) = delete;
      MapperFactory& operator=(const MapperFactory&) = delete;

      /// <summary>
      /// A function for enabling specialized configuration.
      /// </summary>
      /// <typeparam name="T">The type of object we want to configure.</typeparam>
      /// <param name="profile">The profile for the given class.</param>
      template<typename T>
      void configure_profile(MappingProfile<T>& profile) {
        throw std::runtime_error(std::format("Mapping Profile does not exist for: {}", typeid(T).name()));
      }
    };
  }
}