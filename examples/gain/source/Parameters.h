//
// Created by Syl on 13/08/2024.
//

#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <mostly_harmless/mostlyharmless_Parameters.h>
#include <cstdint>
#include <array>
namespace examples::gain {
    enum ParamIds : std::uint32_t {
        kGain,
        kNumParams
    };

    struct Parameters final {
        mostly_harmless::Parameter<float>* gainParam{ nullptr };

        auto asArray() {
            std::array<mostly_harmless::Parameter<float>*, static_cast<size_t>(kNumParams)> res;
            std::memcpy(res.data(), this, sizeof(Parameters));
            return res;
        }
    };

} // namespace examples::gain
#endif // PARAMETERS_H
