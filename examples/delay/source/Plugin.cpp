//
// Created by Syl on 19/08/2024.
//

#include "Plugin.h"
#include "PluginEditor.h"
#include <mostly_harmless/audio/mostlyharmless_AudioHelpers.h>

namespace examples::delay {
    std::vector<mostly_harmless::Parameter<float>> createParams() {
        return {
            { ParamIds::kDelayTime, "Delay", "plugin/", { 0.01f, 2.0f }, 0.2f, CLAP_PARAM_IS_AUTOMATABLE },
            { ParamIds::kFeedback, "Feedback", "plugin/", { 0.0f, 0.9f }, 0.5f, CLAP_PARAM_IS_AUTOMATABLE },
            { ParamIds::kDryWet, "DryWet", "plugin/", { 0.0f, 1.0f }, 0.5f, CLAP_PARAM_IS_AUTOMATABLE }
        };
    }

    Plugin::Plugin(const clap_host* host) : mostly_harmless::Plugin<float>(host, createParams()) {
        m_parameters.delayTimeParam = getParameter(ParamIds::kDelayTime);
        m_parameters.feedbackParam = getParameter(ParamIds::kFeedback);
        m_parameters.dryWetParam = getParameter(ParamIds::kDryWet);
    }

    void Plugin::initialise(double sampleRate, std::uint32_t /*minFrames*/, std::uint32_t /*maxFrames*/) noexcept {
        m_delay.intialise(sampleRate, m_parameters);
    }

    void Plugin::process(marvin::containers::BufferView<float> buffer, mostly_harmless::events::InputEventContext context) noexcept {
        auto onEvent = [this](const clap_event_header* event) -> void {
            handleEvent(event);
        };

        auto onAudio = [this](marvin::containers::BufferView<float> buffer) -> void {
            m_delay.process(buffer, m_parameters);
        };

        mostly_harmless::runBlockDispatch<float>(buffer, context, std::move(onEvent), std::move(onAudio));
    }

    void Plugin::flushParams(mostly_harmless::events::InputEventContext context) noexcept {
        pollEventQueue(context);
    }

    void Plugin::reset() noexcept {
        m_delay.reset();
    }

    std::unique_ptr<mostly_harmless::gui::IEditor> Plugin::createEditor() noexcept {
        return std::make_unique<PluginEditor>(500, 500);
    }

    void Plugin::loadState(std::string_view /*loadedState*/) {
    }

    void Plugin::saveState(std::ostringstream& /*stream*/) {
    }
} // namespace examples::delay
MOSTLYHARMLESS_REGISTER(examples::delay::Plugin);