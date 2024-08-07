#include "Gain.h"
#include "clap/ext/params.h"
#include "marvin/containers/marvin_BufferView.h"
#include "mostlyharmless_Plugin.h"
#include <iostream>
namespace mostly_harmless::examples {
    std::vector<Parameter<float>> createParameters() {
        return {
            Parameter<float>{ ParamIds::kGain, "Gain", "Plugin", { 0.0f, 1.0f }, 1.0f, CLAP_PARAM_IS_AUTOMATABLE }
        };
    }

    Gain::Gain(const clap_host* host) : Plugin<float>(host, createParameters()) {
    }

    void Gain::initialise(double, std::uint32_t, std::uint32_t) noexcept {
    }

    void Gain::process(marvin::containers::BufferView<float> buffer) noexcept {
    }


} // namespace mostly_harmless::examples
MOSTLYHARMLESS_REGISTER(mostly_harmless::examples::Gain)