//
// Created by Syl Morrison on 23/02/2025.
//

#include "synth_Envelope.h"
#include <marvin/math/marvin_Conversions.h>


void Envelope::intialise(double sampleRate) {
    m_sampleRate = sampleRate;
    m_gain.setCurrentAndTargetValue(0.0f);
}

void Envelope::setAttack(float newAttackSeconds) {
    const auto attackSamples = marvin::math::secondsToSamples(newAttackSeconds, m_sampleRate);
    m_attackPhaseSamples = attackSamples;
}

void Envelope::setRelease(float newReleaseSeconds) {
    const auto releaseSamples = marvin::math::secondsToSamples(newReleaseSeconds, m_sampleRate);
    m_releasePhaseSamples = releaseSamples;
}

void Envelope::noteOn() {
    m_gain.reset(m_attackPhaseSamples, false);
    m_gain.setTargetValue(1.0f);
}

void Envelope::noteOff() {
    m_gain.reset(m_releasePhaseSamples, false);
    m_gain.setTargetValue(0.0f);
}

float Envelope::operator()(float x) noexcept {
    const auto nextGain = m_gain();
    return x * nextGain;
}

void Envelope::reset() {
}

bool Envelope::isSmoothing() const noexcept {
    return m_gain.isSmoothing();
}