//
// Created by Syl on 19/08/2024.
//

#ifndef MOSTLYHARMLESS_DELAYPARAMETERS_H
#define MOSTLYHARMLESS_DELAYPARAMETERS_H
#include <mostly_harmless/mostlyharmless_Parameters.h>
#include <cstdint>
namespace examples::delay {
    enum ParamIds : std::uint32_t {
        kDelayTime,
        kFeedback,
        kDryWet,
        kNumParams
    };

    struct Parameters {
        mostly_harmless::Parameter<float>* delayTimeParam{ nullptr };
        mostly_harmless::Parameter<float>* feedbackParam{ nullptr };
        mostly_harmless::Parameter<float>* dryWetParam{ nullptr };
    };
} // namespace examples::delay
#endif // MOSTLYHARMLESS_PARAMETERS_H
