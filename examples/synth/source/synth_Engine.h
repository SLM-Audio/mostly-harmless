#ifndef SYNTH_ENGINE_H
#define SYNTH_ENGINE_H

#include "synth_SharedState.h"
#include "synth_Voice.h"
#include <mostly_harmless/core/mostlyharmless_IEngine.h>

class Engine final : public mostly_harmless::core::IEngine {
public:
    explicit Engine(SharedState* sharedState);
    void initialise(mostly_harmless::core::InitContext context) noexcept override;
    void process(mostly_harmless::core::ProcessContext context) noexcept override;
    void reset() noexcept override;

    void handleNoteOn(std::uint16_t portIndex, std::uint8_t channel, std::uint8_t note, double velocity) override;
    void handleNoteOff(std::uint16_t portIndex, std::uint8_t channel, std::uint8_t note, double velocity) override;

private:
    SharedState* m_sharedState{ nullptr };
    std::array<StereoVoice, 128> m_voices;
};

#endif // SYNTH_ENGINE_H
