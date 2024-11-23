//
// Created by Syl Morrison on 17/11/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_UTILS_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_UTILS_H
#include <concepts>
#include <type_traits>
#include <string_view>
namespace mostly_harmless::utils {
    enum class HashType {
        Default,
        DJB2
    };
    template <typename T>
    concept UnsignedIntType = std::is_integral_v<T> && std::is_unsigned_v<T>;

    namespace {
        template <UnsignedIntType T>
        constexpr T djb2(std::string_view toHash) noexcept {
            auto hash = 5381UL;
            auto* data = toHash.data();
            while (auto c = *data++) {
                hash = ((hash << 5) + hash) + c;
            }
            return static_cast<T>(hash);
        }
    } // namespace

    template <UnsignedIntType T, HashType Algorithm = HashType::Default>
    constexpr static T hash(std::string_view toHash) noexcept {
        switch (Algorithm) {
            case HashType::DJB2: [[fallthrough]];
            default:
                return djb2<T>(toHash);
        }
    }

} // namespace mostly_harmless::utils
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_UTILS_H
