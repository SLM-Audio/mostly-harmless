#include "delay_Engine.h"

Engine::Engine(SharedState* sharedState) : m_sharedState(sharedState) {
}

void Engine::initialise(mostly_harmless::core::InitContext /*context*/) noexcept {
}

void Engine::process(mostly_harmless::core::ProcessContext context) noexcept {
    auto buffer = context.buffer;
    [[maybe_unused]] const auto* const* read = buffer.getArrayOfReadPointers();
    [[maybe_unused]] auto* const* write = buffer.getArrayOfWritePointers();
    for (size_t channel = 0; channel < buffer.getNumChannels(); ++channel) {
        for (size_t sample = 0; sample < buffer.getNumSamples(); ++sample) {
        }
    }
}

void Engine::reset() noexcept {
}

void Engine::handleNoteOn(std::uint16_t /*portIndex*/, std::uint8_t /*channel*/, std::uint8_t /*note*/, double /*velocity*/) {
}

void Engine::handleNoteOff(std::uint16_t /*portIndex*/, std::uint8_t /*channel*/, std::uint8_t /*note*/, double /*velocity*/) {
}