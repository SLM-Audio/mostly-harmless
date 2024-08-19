//
// Created by Syl on 19/08/2024.
//

#ifndef MOSTLYHARMLESS_DELAYEXAMPLE_H
#define MOSTLYHARMLESS_DELAYEXAMPLE_H
#include <Delay.h>
#include <mostly_harmless/mostlyharmless_Plugin.h>


namespace examples::delay {
    class Plugin : public mostly_harmless::Plugin<float> {
    public:
        explicit Plugin(const clap_host* host);
        void initialise(double sampleRate, std::uint32_t minFrames, std::uint32_t maxFrames) noexcept override;
        void process(marvin::containers::BufferView<float> buffer, mostly_harmless::events::InputEventContext context) noexcept override;
        void flushParams(mostly_harmless::events::InputEventContext context) noexcept override;
        void reset() noexcept override;
        [[nodiscard]] std::unique_ptr<mostly_harmless::gui::IEditor> createEditor() noexcept override;

    private:
        void loadState(std::string_view loadedState) override;
        void saveState(std::ostringstream& stream) override;
        Parameters m_parameters;
        Delay<float> m_delay;
    };
} // namespace examples::delay
#endif // MOSTLYHARMLESS_PLUGIN_H
