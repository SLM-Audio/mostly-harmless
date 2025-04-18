//
// Created by Syl Morrison on 20/10/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_CONCEPTS_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_CONCEPTS_H
#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
#include <type_traits>

namespace mostly_harmless {
    template <typename T>
    concept StateType = std::is_base_of_v<core::ISharedState, T>;


    template <typename T>
    concept DatabaseStorageType = std::same_as<T, std::string> || std::is_integral_v<T> || std::is_floating_point_v<T>;
} // namespace mostly_harmless
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_CONCEPTS_H
