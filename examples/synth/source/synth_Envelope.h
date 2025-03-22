//
// Created by Syl Morrison on 23/02/2025.
//

#ifndef MOSTLYHARMLESS_SYNTH_ENVELOPE_H
#define MOSTLYHARMLESS_SYNTH_ENVELOPE_H
#include "synth_Parameters.h"
#include <marvin/utils/marvin_SmoothedValue.h>
class Envelope final {
public:
    void intialise(double sampleRate);
    void setAttack(float newAttackSeconds);
    void setRelease(float newReleaseSeconds);
    void noteOn();
    void noteOff();
    [[nodiscard]] float operator()(float x) noexcept;
    void reset();
    [[nodiscard]] bool isSmoothing() const noexcept;

private:
    double m_sampleRate{ 0 };
    int m_attackPhaseSamples{ 0 };
    int m_releasePhaseSamples{ 0 };
    marvin::utils::SmoothedValue<float, marvin::utils::SmoothingType::Linear> m_gain;
};
#endif // MOSTLYHARMLESS_SYNTH_ENVELOPE_H
