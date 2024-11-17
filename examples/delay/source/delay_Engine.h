#ifndef DELAY_ENGINE_H
#define DELAY_ENGINE_H

#include "delay_SharedState.h"
#include <mostly_harmless/core/mostlyharmless_IEngine.h>
#include <marvin/dsp/marvin_DelayLine.h>
#include <marvin/utils/marvin_SmoothedValue.h>
#include <array>

class Engine final : public mostly_harmless::core::IEngine {
public:
    explicit Engine(SharedState* sharedState);
    void initialise(mostly_harmless::core::InitContext context) noexcept override;
    void process(mostly_harmless::core::ProcessContext context) noexcept override;
    void reset() noexcept override;

    void handleNoteOn(std::uint16_t portIndex, std::uint8_t channel, std::uint8_t note, double velocity) override;
    void handleNoteOff(std::uint16_t portIndex, std::uint8_t channel, std::uint8_t note, double velocity) override;

private:
    void checkParameters() noexcept;
    struct ParamSmoother final {
        marvin::utils::SmoothedValue<float, marvin::utils::SmoothingType::Linear> smoothedTime;
        marvin::utils::SmoothedValue<float, marvin::utils::SmoothingType::Linear> smoothedFeedback;
        marvin::utils::SmoothedValue<float, marvin::utils::SmoothingType::Linear> smoothedDryWet;
        int paramUpdateRate;
        int samplesUntilParamUpdate{ 0 };
    } m_smoother;
    SharedState* m_sharedState{ nullptr };
    mostly_harmless::core::InitContext m_initContext;
    std::array<marvin::dsp::DelayLine<float, marvin::dsp::DelayLineInterpolationType::Linear>, 2> m_delayLines;
    constexpr static auto m_paramUpdateRateSeconds{ 100.0 / 44100.0 };
};

#endif // DELAY_ENGINE_H
