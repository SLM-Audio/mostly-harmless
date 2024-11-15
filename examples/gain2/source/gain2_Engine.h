//
// Created by Syl Morrison on 20/10/2024.
//

#ifndef MOSTLYHARMLESS_GAIN2_ENGINE_H
#define MOSTLYHARMLESS_GAIN2_ENGINE_H
#include "gain2_SharedState.h"
#include <mostly_harmless/core/mostlyharmless_IEngine.h>
#include <marvin/utils/marvin_SmoothedValue.h>
namespace gain2 {
    class Engine : public mostly_harmless::core::IEngine {
    public:
        explicit Engine(SharedState* sharedState);
        void initialise(mostly_harmless::core::InitContext context) override;
        void process(mostly_harmless::core::ProcessContext context) override;
        void reset() noexcept override;

    private:
        void checkParameters() noexcept;
        constexpr static auto m_paramUpdateRateSeconds{ 100.0 / 44100.0 };
        int m_paramUpdateRate;
        int m_samplesUntilParamUpdate{ 0 };
        SharedState* m_sharedState{ nullptr };
        marvin::utils::SmoothedValue<float, marvin::utils::SmoothingType::Linear> m_smoothedGain;
    };

} // namespace gain2

#endif // MOSTLYHARMLESS_GAIN2_ENGINE_H
