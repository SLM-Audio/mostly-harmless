#include "delay_Engine.h"
#include <marvin/math/marvin_Conversions.h>

Engine::Engine(SharedState* sharedState) : m_sharedState(sharedState) {
}

void Engine::initialise(mostly_harmless::core::InitContext context) noexcept {
    m_initContext = context;
    auto params = m_sharedState->getParamView();
    for (auto& d : m_delayLines) {
        const auto maxDelaySamples = static_cast<int>(marvin::math::secondsToSamples(params.timeParam->range.max, context.sampleRate)) + 1;
        d.initialise(context.sampleRate);
        d.setMaximumDelayInSamples(maxDelaySamples);
    }
    const auto paramUpdateRateSamples = static_cast<int>(m_paramUpdateRateSeconds * context.sampleRate);
    m_smoother.paramUpdateRate = paramUpdateRateSamples;
    m_smoother.smoothedTime.reset(paramUpdateRateSamples);
    const auto delaySamples = marvin::math::secondsToSamples(params.timeParam->value, context.sampleRate);
    m_smoother.smoothedTime.setCurrentAndTargetValue(delaySamples);
    m_smoother.smoothedFeedback.reset(paramUpdateRateSamples);
    m_smoother.smoothedFeedback.setCurrentAndTargetValue(params.feedbackParam->value);
    m_smoother.smoothedDryWet.reset(paramUpdateRateSamples);
    m_smoother.smoothedDryWet.setCurrentAndTargetValue(params.dryWetParam->value);
}

void Engine::process(mostly_harmless::core::ProcessContext context) noexcept {
    auto buffer = context.buffer;
    [[maybe_unused]] const auto* const* read = buffer.getArrayOfReadPointers();
    [[maybe_unused]] auto* const* write = buffer.getArrayOfWritePointers();
    for (size_t sample = 0; sample < buffer.getNumSamples(); ++sample) {
        checkParameters();
        const auto timeSeconds = m_smoother.smoothedTime();
        const auto timeSamples = marvin::math::secondsToSamples(timeSeconds, m_initContext.sampleRate);
        const auto feedback = m_smoother.smoothedFeedback();
        const auto dryWet = m_smoother.smoothedDryWet();
        for (size_t channel = 0; channel < buffer.getNumChannels(); ++channel) {
            auto& delayLine = m_delayLines[channel];
            const auto delayed = delayLine.popSample(timeSamples);
            const auto delayInput = read[channel][sample] + (delayed * feedback);
            delayLine.pushSample(delayInput);
            write[channel][sample] = (delayed * dryWet) + (read[channel][sample] * (1.0f - dryWet));
        }
    }
}

void Engine::reset() noexcept {
    for (auto& d : m_delayLines) {
        d.reset();
    }
}

void Engine::handleNoteOn(std::uint16_t /*portIndex*/, std::uint8_t /*channel*/, std::uint8_t /*note*/, double /*velocity*/) {
}

void Engine::handleNoteOff(std::uint16_t /*portIndex*/, std::uint8_t /*channel*/, std::uint8_t /*note*/, double /*velocity*/) {
}

void Engine::checkParameters() noexcept {
    if (m_smoother.samplesUntilParamUpdate == 0) {
        auto paramView = m_sharedState->getParamView();
        m_smoother.smoothedTime.setTargetValue(paramView.timeParam->value);
        m_smoother.smoothedFeedback.setTargetValue(paramView.feedbackParam->value);
        m_smoother.smoothedDryWet.setTargetValue(paramView.dryWetParam->value);
        m_smoother.samplesUntilParamUpdate = m_smoother.paramUpdateRate;
    }
    --m_smoother.samplesUntilParamUpdate;
}