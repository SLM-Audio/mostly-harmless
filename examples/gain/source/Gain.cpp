#include "Gain.h"
#include "clap/ext/params.h"
#include "marvin/containers/marvin_BufferView.h"
#include "mostlyharmless_EventContext.h"
#include "mostlyharmless_Plugin.h"
#include "marvin/library/marvin_Literals.h"
#include <iostream>
namespace mostly_harmless::examples {
    std::vector<Parameter<float>> createParameters() {
        return {
            Parameter<float>{ ParamIds::kGain, "Gain", "Plugin", { 0.0f, 1.0f }, 1.0f, CLAP_PARAM_IS_AUTOMATABLE }
        };
    }

    Gain::Gain(const clap_host* host) : Plugin<float>(host, createParameters()) {
        m_params.gainParam = getParameter(ParamIds::kGain);
    }

    void Gain::initialise(double, std::uint32_t, std::uint32_t) noexcept {
    }

    void Gain::process(marvin::containers::BufferView<float> buffer, EventContext eventContext) noexcept {
        using namespace marvin::literals;
        const auto* const* read = buffer.getArrayOfReadPointers();
        auto* const* write = buffer.getArrayOfWritePointers();
        for (auto sample = 0_sz; sample < buffer.getNumSamples(); ++sample) {
            pollEventQueue(sample, eventContext);
            const auto gain = m_params.gainParam->value;
            for (auto channel = 0_sz; channel < buffer.getNumChannels(); ++channel) {
                write[channel][sample] = read[channel][sample] * gain;
            }
        }
    }


} // namespace mostly_harmless::examples
MOSTLYHARMLESS_REGISTER(mostly_harmless::examples::Gain)