#ifndef GAIN_ENGINE_H
#define GAIN_ENGINE_H

#include "gain_SharedState.h"
#include <mostly_harmless/core/mostlyharmless_IEngine.h>
#include <marvin/utils/marvin_SmoothedValue.h>

class Engine final : public mostly_harmless::core::IEngine {
public:
    Engine(SharedState* sharedState);
    void initialise(mostly_harmless::core::InitContext context) noexcept override;
    void process(mostly_harmless::core::ProcessContext context) noexcept override;
    void reset() noexcept override;

    void handleNoteOn(std::uint16_t portIndex, std::uint8_t channel, std::uint8_t note, double velocity) override;
    void handleNoteOff(std::uint16_t portIndex, std::uint8_t channel, std::uint8_t note, double velocity) override;

private:
    void checkParameters() noexcept;
    SharedState* m_sharedState{ nullptr };
    constexpr static auto m_paramUpdateRateSeconds{ 400.0 / 44100.0 };
    int m_paramUpdateRate;
    int m_samplesUntilParamUpdate{ 0 };
    marvin::utils::SmoothedValue<float, marvin::utils::SmoothingType::Linear> m_smoothedGain;
};

#endif // GAIN_ENGINE_H
