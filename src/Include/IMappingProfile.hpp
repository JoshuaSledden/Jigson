#pragma once
#include "Types.h"

namespace jigson {
    namespace mapping {
        /**
         * Interface for mapping profiles.
         *
         * This interface defines the contract for mapping JSON payloads to objects.
         * Concrete implementations of this interface should provide specific mapping logic
         * for different object types.
         */
        class IMappingProfile {
        public:
            /**
             * Virtual destructor to ensure proper cleanup of derived classes.
             */
            virtual ~IMappingProfile() = default;

            /**
             * Map a JSON payload to an object.
             *
             * @param json The JSON payload to map from.
             * @param obj Pointer to the object to map into. The actual type is determined by the concrete implementation.
             */
            virtual void map(const json_payload &json, void *obj) const = 0;
        };
    }
}
