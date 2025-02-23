//
// Created by Syl Morrison on 23/02/2025.
//

#include "synth_Voice.h"
static std::random_device s_rd{};
float mtof(std::uint8_t midi) {
    const auto exponent = static_cast<float>(midi - 69) / 12.0f;
    const auto power = std::pow(2.0f, exponent);
    return power * 440.0f;
}

SynthVoice::SynthVoice() : m_osc(s_rd) {
    m_osc.setShape(marvin::dsp::oscillators::MultiOscillator<float, marvin::dsp::oscillators::Bandlimiting::On>::Shape::Sine);
}
void SynthVoice::initialise(double sampleRate) {
    m_osc.initialise(sampleRate);
    m_envelope.intialise(sampleRate);
    m_envelope.setAttack(0.5f);
    m_envelope.setRelease(0.5f);
}

void SynthVoice::noteOn(std::uint8_t note, double velocity, Parameters paramView) {
    if (!m_envelope.isSmoothing()) {
        m_osc.reset();
    }
    const auto frequency = mtof(note);
    m_osc.setFrequency(frequency);
    m_envelope.setAttack(paramView.attackParam->value);
    m_envelope.noteOn();
}

void SynthVoice::noteOff(std::uint8_t note, Parameters paramView) {
    m_envelope.setRelease(paramView.releaseParam->value);
    m_envelope.noteOff();
}

float SynthVoice::operator()() noexcept {

    constexpr static auto gain = 0.1;
    return m_envelope(m_osc() * gain);
}

void SynthVoice::reset() noexcept {
    m_osc.reset();
}


void StereoVoice::initialise(double sampleRate) {
    for (auto& v : m_voices) {
        v.initialise(sampleRate);
    }
}

void StereoVoice::noteOn(std::uint8_t note, double velocity, Parameters paramView) {
    for (auto& v : m_voices) {
        v.noteOn(note, velocity, paramView);
    }
}

void StereoVoice::noteOff(std::uint8_t note, Parameters paramView) {
    for (auto& v : m_voices) {
        v.noteOff(note, paramView);
    }
}

float StereoVoice::operator()(size_t channel) noexcept {
    return m_voices[channel]();
}

void StereoVoice::reset() {
    for (auto& v : m_voices) {
        v.reset();
    }
}