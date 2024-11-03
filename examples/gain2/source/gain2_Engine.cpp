//
// Created by Syl Morrison on 20/10/2024.
//
#include "gain2_Engine.h"
#include <marvin/library/marvin_Literals.h>
namespace gain2 {
    Engine::Engine(SharedState* sharedState) : m_sharedState(sharedState) {
    }

    void Engine::initialise(mostly_harmless::core::InitContext initContext) noexcept {
        m_paramUpdateRate = static_cast<int>(m_paramUpdateRateSeconds * initContext.sampleRate);
        const auto paramView = m_sharedState->getParamView();
        m_smoothedGain.reset(m_paramUpdateRate);
        m_smoothedGain.setCurrentAndTargetValue(paramView.gainParam->value);
    }

    void Engine::process(mostly_harmless::core::ProcessContext context) noexcept {
        using namespace marvin::literals;
        const auto* const* read = context.buffer.getArrayOfReadPointers();
        auto* const* write = context.buffer.getArrayOfWritePointers();
        for (auto sample = 0_sz; sample < context.buffer.getNumSamples(); ++sample) {
            checkParameters();
            const auto gain = m_smoothedGain();
            for (auto channel = 0_sz; channel < context.buffer.getNumChannels(); ++channel) {
                write[channel][sample] = read[channel][sample] * gain;
            }
        }
    }

    void Engine::reset() noexcept {
        m_smoothedGain.reset(m_paramUpdateRate);
    }

    void Engine::checkParameters() noexcept {
        const auto paramView = m_sharedState->getParamView();
        if (m_samplesUntilParamUpdate == 0) {
            const auto gain = paramView.gainParam->value;
            m_smoothedGain.setTargetValue(gain);
            m_samplesUntilParamUpdate = m_paramUpdateRate;
        }
        --m_samplesUntilParamUpdate;
    }
} // namespace gain2
