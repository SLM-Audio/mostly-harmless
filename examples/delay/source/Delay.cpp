//
// Created by Syl on 19/08/2024.
//

#include "Delay.h"
#include <marvin/library/marvin_Literals.h>
namespace examples::delay {
    template <marvin::FloatType SampleType>
    void Delay<SampleType>::intialise(double sampleRate, Parameters params) noexcept {
        m_sampleRate = sampleRate;
        m_paramUpdateRate = static_cast<int>(m_paramUpdateRateSeconds * sampleRate);
        m_smoothedDelayTime.reset(m_paramUpdateRate);
        m_smoothedDelayTime.setCurrentAndTargetValue(static_cast<SampleType>(params.delayTimeParam->value));
        m_smoothedFeedback.reset(m_paramUpdateRate);
        m_smoothedFeedback.setCurrentAndTargetValue(static_cast<SampleType>(params.feedbackParam->value));
        m_smoothedDryWet.reset(m_paramUpdateRate);
        m_smoothedDryWet.setCurrentAndTargetValue(static_cast<SampleType>(params.dryWetParam->value));
    }

    template <marvin::FloatType SampleType>
    void Delay<SampleType>::process(marvin::containers::BufferView<SampleType> buffer, Parameters params) noexcept {
        using namespace marvin::literals;
        const auto* const* read = buffer.getArrayOfReadPointers();
        auto* const* write = buffer.getArrayOfWritePointers();
        for (auto sample = 0_sz; sample < buffer.getNumSamples(); ++sample) {
            checkParameters(params);
            const auto delaySamples = m_smoothedDelayTime() * static_cast<SampleType>(m_sampleRate);
            const auto feedback = m_smoothedFeedback();
            const auto dryWet = m_smoothedDryWet();
            for (auto channel = 0_sz; channel < buffer.getNumChannels(); ++channel) {
                const auto delayed = m_delayLines[channel].popSample(delaySamples);
                const auto feedbackSample = delayed * feedback;
                m_delayLines[channel].pushSample(read[channel][sample] + feedbackSample);
                write[channel][sample] = (delayed * dryWet) + (read[channel][sample] * (static_cast<SampleType>(1.0) - dryWet));
            }
        }
    }

    template <marvin::FloatType SampleType>
    void Delay<SampleType>::reset() noexcept {
        for (auto& d : m_delayLines) {
            d.reset();
        }
    }

    template <marvin::FloatType SampleType>
    void Delay<SampleType>::checkParameters(examples::delay::Parameters params) noexcept {
        if (m_samplesUntilParamUpdate == 0) {
            m_smoothedDelayTime.setTargetValue(static_cast<SampleType>(params.delayTimeParam->value));
            m_smoothedFeedback.setTargetValue(static_cast<SampleType>(params.feedbackParam->value));
            m_smoothedDryWet.setTargetValue(static_cast<SampleType>(params.dryWetParam->value));
            m_samplesUntilParamUpdate = m_paramUpdateRate;
        }
        --m_samplesUntilParamUpdate;
    }
}; // namespace examples::delay