#include "synth_Engine.h"
#include <mostly_harmless/utils/mostlyharmless_Macros.h>
#include <fmt/core.h>

Engine::Engine(SharedState* sharedState) : m_sharedState(sharedState) {
}

void Engine::initialise(mostly_harmless::core::InitContext context) noexcept {
    for (auto& v : m_voices) {
        v.initialise(context.sampleRate);
    }
}

void Engine::process(mostly_harmless::core::ProcessContext context) noexcept {
    auto buffer = context.buffer;
    [[maybe_unused]] const auto* const* read = buffer.getArrayOfReadPointers();
    [[maybe_unused]] auto* const* write = buffer.getArrayOfWritePointers();
    for (size_t channel = 0; channel < buffer.getNumChannels(); ++channel) {
        for (size_t sample = 0; sample < buffer.getNumSamples(); ++sample) {
            auto output = 0.0f;
            for (auto& v : m_voices) {
                output += v(channel);
            }
            write[channel][sample] = output;
        }
    }
}

void Engine::reset() noexcept {
    for (auto& v : m_voices) {
        v.reset();
    }
}

void Engine::handleNoteOn(std::uint16_t portIndex, std::uint8_t channel, std::uint8_t note, double velocity) {
    auto paramView{ m_sharedState->getParamView() };
    m_voices[note].noteOn(note, velocity, paramView);
}

void Engine::handleNoteOff(std::uint16_t portIndex, std::uint8_t channel, std::uint8_t note, double velocity) {
    auto paramView{ m_sharedState->getParamView() };
    m_voices[note].noteOff(note, paramView);
}