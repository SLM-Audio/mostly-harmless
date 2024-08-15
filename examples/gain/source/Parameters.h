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

        // Evil reinterpret cast - only works because every element is the same type
        std::span<mostly_harmless::Parameter<float>*> toView() {
            auto** thisAsArray = reinterpret_cast<mostly_harmless::Parameter<float>**>(this);
            std::span<mostly_harmless::Parameter<float>*> paramView{ thisAsArray, static_cast<size_t>(ParamIds::kNumParams) };
            return paramView;
        }
    };

} // namespace examples::gain
#endif // PARAMETERS_H
