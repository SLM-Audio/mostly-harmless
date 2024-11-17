#include "gain_Engine.h"

Engine::Engine(SharedState* sharedState) : m_sharedState(sharedState) {
}

void Engine::initialise(mostly_harmless::core::InitContext context) noexcept {
    auto paramView = m_sharedState->getParamView();
    m_paramUpdateRate = static_cast<int>(m_paramUpdateRateSeconds * context.sampleRate);
    m_smoothedGain.reset(m_paramUpdateRate);
    m_smoothedGain.setCurrentAndTargetValue(paramView.gainParam->value);
}

void Engine::process(mostly_harmless::core::ProcessContext context) noexcept {
    auto buffer = context.buffer;
    [[maybe_unused]] const auto* const* read = buffer.getArrayOfReadPointers();
    [[maybe_unused]] auto* const* write = buffer.getArrayOfWritePointers();
    for (size_t sample = 0; sample < buffer.getNumSamples(); ++sample) {
        checkParameters();
        const auto gain = m_smoothedGain();
        for (size_t channel = 0; channel < buffer.getNumChannels(); ++channel) {
            write[channel][sample] *= gain;
        }
    }
}

void Engine::reset() noexcept {
    m_smoothedGain.reset(m_paramUpdateRate);
}

void Engine::handleNoteOn(std::uint16_t /*portIndex*/, std::uint8_t /*channel*/, std::uint8_t /*note*/, double /*velocity*/) {
}

void Engine::handleNoteOff(std::uint16_t /*portIndex*/, std::uint8_t /*channel*/, std::uint8_t /*note*/, double /*velocity*/) {
}

void Engine::checkParameters() noexcept {
    if (m_samplesUntilParamUpdate == 0) {
        auto paramView = m_sharedState->getParamView();
        m_smoothedGain.setTargetValue(paramView.gainParam->value);
        m_samplesUntilParamUpdate = m_paramUpdateRate;
    }
    --m_samplesUntilParamUpdate;
}