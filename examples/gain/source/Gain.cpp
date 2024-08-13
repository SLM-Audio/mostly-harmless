#include "Gain.h"
#include "GainEditor.h"
#include "clap/ext/params.h"
#include "marvin/containers/marvin_BufferView.h"
#include "marvin/library/marvin_Literals.h"
#include <iostream>
namespace examples::gain {
    std::vector<mostly_harmless::Parameter<float>> createParameters() {
        return {
            mostly_harmless::Parameter<float>{ ParamIds::kGain, "Gain", "Plugin", { 0.0f, 1.0f }, 1.0f, CLAP_PARAM_IS_AUTOMATABLE }
        };
    }

    Gain::Gain(const clap_host* host) : Plugin<float>(host, createParameters()) {
        m_params.gainParam = getParameter(ParamIds::kGain);
    }

    void Gain::initialise(double sampleRate, std::uint32_t, std::uint32_t) noexcept {
        m_paramUpdateRate = static_cast<int>(m_paramUpdateRateSeconds * sampleRate);
        m_smoothedGain.reset(m_paramUpdateRate);
        m_smoothedGain.setCurrentAndTargetValue(m_params.gainParam->defaultValue);
    }

    void Gain::process(marvin::containers::BufferView<float> buffer, mostly_harmless::events::InputEventContext eventContext) noexcept {
        using namespace marvin::literals;
        const auto* const* read = buffer.getArrayOfReadPointers();
        auto* const* write = buffer.getArrayOfWritePointers();
        for (auto sample = 0_sz; sample < buffer.getNumSamples(); ++sample) {
            pollEventQueue(sample, eventContext);
            checkParameters();
            const auto gain = m_smoothedGain();
            for (auto channel = 0_sz; channel < buffer.getNumChannels(); ++channel) {
                write[channel][sample] = read[channel][sample] * gain;
            }
        }
    }

    void Gain::flushParams(mostly_harmless::events::InputEventContext eventContext) noexcept {
        pollEventQueue(eventContext);
    }

    void Gain::reset() noexcept {
    }

    std::unique_ptr<mostly_harmless::gui::IEditor> Gain::createEditor() noexcept {
        return std::make_unique<GainEditor>(500, 500);
    }

    void Gain::checkParameters() {
        if (m_samplesUntilParamUpdate == 0) {
            m_smoothedGain.setTargetValue(m_params.gainParam->value);
            m_samplesUntilParamUpdate = m_paramUpdateRate;
        }
        --m_samplesUntilParamUpdate;
    }

} // namespace examples::gain
MOSTLYHARMLESS_REGISTER(examples::gain::Gain)