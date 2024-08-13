#ifndef GAIN_H
#define GAIN_H
#include "Parameters.h"
#include <mostly_harmless/gui/mostlyharmless_IEditor.h>
#include <mostly_harmless/mostlyharmless_Plugin.h>
#include <marvin/utils/marvin_SmoothedValue.h>
#include "marvin/containers/marvin_BufferView.h"

namespace examples::gain {
    class Gain : public mostly_harmless::Plugin<float> {
    public:
        explicit Gain(const clap_host* host);
        ~Gain() noexcept override = default;
        void initialise(double sampleRate, std::uint32_t minFrames, std::uint32_t maxFrames) noexcept override;
        void process(marvin::containers::BufferView<float> buffer, mostly_harmless::events::InputEventContext eventContext) noexcept override;
        void flushParams(mostly_harmless::events::InputEventContext eventContext) noexcept override;
        void reset() noexcept override;
        std::unique_ptr<mostly_harmless::gui::IEditor> createEditor() noexcept override;

    private:
        void checkParameters();
        Parameters m_params;
        // KR = 100 at 44.1
        constexpr static auto m_paramUpdateRateSeconds{ 100.0 / 44100.0 };
        int m_paramUpdateRate{};
        int m_samplesUntilParamUpdate{ 0 };

        marvin::utils::SmoothedValue<float, marvin::utils::SmoothingType::Linear> m_smoothedGain{};
    };
} // namespace examples::gain
#endif