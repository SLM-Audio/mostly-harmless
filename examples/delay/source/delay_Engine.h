#ifndef DELAY_ENGINE_H
#define DELAY_ENGINE_H

#include "delay_SharedState.h"
#include <mostly_harmless/core/mostlyharmless_IEngine.h>

class Engine final : public mostly_harmless::core::IEngine {
public:
    Engine(SharedState* sharedState);
    void initialise(mostly_harmless::core::InitContext context) noexcept override;
    void process(mostly_harmless::core::ProcessContext context) noexcept override;
    void reset() noexcept override;

    void handleNoteOn(std::uint16_t portIndex, std::uint8_t channel, std::uint8_t note, double velocity) override;
    void handleNoteOff(std::uint16_t portIndex, std::uint8_t channel, std::uint8_t note, double velocity) override;

private:
    SharedState* m_sharedState{ nullptr };
};

#endif // DELAY_ENGINE_H
