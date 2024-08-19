//
// Created by Syl on 19/08/2024.
//

#include "Plugin.h"
#include <mostly_harmless/audio/mostlyharmless_AudioHelpers.h>

namespace examples::delay {
    std::vector<mostly_harmless::Parameter<float>> createParams() {
        return {};
    }

    Plugin::Plugin(const clap_host* host) : mostly_harmless::Plugin<float>(host, createParams()) {
    }

    void Plugin::initialise(double sampleRate, std::uint32_t minFrames, std::uint32_t maxFrames) noexcept {
        m_delay.intialise(sampleRate);
    }

    void Plugin::process(marvin::containers::BufferView<float> buffer, mostly_harmless::events::InputEventContext context) noexcept {
        auto onEvent = [this](const clap_event_header* event) -> void {
            handleEvent(event);
        };

        auto onAudio = [this](marvin::containers::BufferView<float> buffer) -> void {
            m_delay.process(buffer);
        };

        mostly_harmless::runBlockDispatch(buffer, context, std::move(onEvent), std::forward<std::function<void(marvin::containers::BufferView<float>)>>(std::move(onAudio)));
    }

    void Plugin::flushParams(mostly_harmless::events::InputEventContext context) noexcept {
    }

    void Plugin::reset() noexcept {
        m_delay.reset();
    }

    std::unique_ptr<mostly_harmless::gui::IEditor> Plugin::createEditor() noexcept {
        return nullptr;
    }

    void Plugin::loadState(std::string_view loadedState) {
    }

    void Plugin::saveState(std::ostringstream& stream) {
    }
} // namespace examples::delay