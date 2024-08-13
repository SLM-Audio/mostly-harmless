#ifndef GAIN_H
#define GAIN_H
#include <mostly_harmless/gui/mostlyharmless_IEditor.h>
#include <mostly_harmless/mostlyharmless_Plugin.h>
#include "marvin/containers/marvin_BufferView.h"

namespace examples::gain {
    enum ParamIds : std::uint32_t {
        kGain,
        kNumParams
    };

    struct Parameters final {
        mostly_harmless::Parameter<float>* gainParam{ nullptr };
    };
    class Gain : public mostly_harmless::Plugin<float> {
    public:
        explicit Gain(const clap_host* host);
        ~Gain() noexcept override = default;
        void initialise(double sampleRate, std::uint32_t minFrames, std::uint32_t maxFrames) noexcept override;
        void process(marvin::containers::BufferView<float> buffer, mostly_harmless::InputEventContext eventContext) noexcept override;
        void flushParams(mostly_harmless::InputEventContext eventContext) noexcept override;
        void reset() noexcept override;
        std::unique_ptr<mostly_harmless::gui::IEditor> createEditor() noexcept override;

    private:
        Parameters m_params;
    };
} // namespace examples::gain
#endif