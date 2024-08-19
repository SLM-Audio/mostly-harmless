//
// Created by Syl on 19/08/2024.
//

#ifndef MOSTLYHARMLESS_DELAY_H
#define MOSTLYHARMLESS_DELAY_H
#include <Parameters.h>
#include <marvin/containers/marvin_BufferView.h>
#include <marvin/dsp/marvin_DelayLine.h>
#include <marvin/utils/marvin_SmoothedValue.h>
#include <array>
namespace examples::delay {
    template <marvin::FloatType SampleType>
    class Delay final {
    public:
        void intialise(double sampleRate, Parameters params) noexcept;
        void process(marvin::containers::BufferView<SampleType> buffer, Parameters params) noexcept;
        void reset() noexcept;

    private:
        void checkParameters(Parameters params) noexcept;
        double m_sampleRate{};
        constexpr static auto m_paramUpdateRateSeconds{ 100.0 / 44100.0 };
        int m_paramUpdateRate{ 0 };
        int m_samplesUntilParamUpdate{ 0 };
        std::array<marvin::dsp::DelayLine<SampleType, marvin::dsp::DelayLineInterpolationType::Linear>, 2> m_delayLines;
        marvin::utils::SmoothedValue<SampleType, marvin::utils::SmoothingType::Linear> m_smoothedDelayTime, m_smoothedFeedback, m_smoothedDryWet;
    };
} // namespace examples::delay
#endif // MOSTLYHARMLESS_DELAY_H
