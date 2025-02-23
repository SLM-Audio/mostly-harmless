//
// Created by Syl Morrison on 23/02/2025.
//

#ifndef MOSTLYHARMLESS_SYNTH_VOICE_H
#define MOSTLYHARMLESS_SYNTH_VOICE_H
#include "synth_Envelope.h"
#include <marvin/dsp/oscillators/marvin_Oscillator.h>
class SynthVoice final {
public:
    SynthVoice();
    void initialise(double sampleRate);
    void noteOn(std::uint8_t note, double velocity, Parameters paramView);
    void noteOff(std::uint8_t note, Parameters paramView);
    [[nodiscard]] float operator()() noexcept;
    void reset() noexcept;

private:
    marvin::dsp::oscillators::MultiOscillator<float, marvin::dsp::oscillators::Bandlimiting::On> m_osc;
    Envelope m_envelope;
};

struct StereoVoice final {
public:
    void initialise(double sampleRate);
    void noteOn(std::uint8_t note, double velocity, Parameters paramView);
    void noteOff(std::uint8_t note, Parameters paramView);
    [[nodiscard]] float operator()(size_t channel) noexcept;
    void reset();

private:
    std::array<SynthVoice, 2> m_voices;
};
#endif // MOSTLYHARMLESS_SYNTH_VOICE_H
