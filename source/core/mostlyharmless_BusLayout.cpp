//
// Created by Syl Morrison on 03/11/2024.
//
#include <mostly_harmless/core/mostlyharmless_BusLayout.h>
namespace mostly_harmless::core {
    BusLayout::BusLayout(std::vector<AudioBus>&& inputs, std::vector<AudioBus>&& outputs) : m_inputPorts(std::move(inputs)),
                                                                                            m_outputPorts(std::move(outputs)) {
    }

    bool BusLayout::satisfiesClapConfig(const clap_audio_port_configuration_request* configs, std::uint32_t requestCount) {
        std::span<const clap_audio_port_configuration_request> configView{ configs, requestCount };
        std::uint32_t numInputs{ 0 }, numOutputs{ 0 };
        std::for_each(configView.begin(), configView.end(), [&numInputs, &numOutputs](const clap_audio_port_configuration_request& config) -> void {
            if (config.is_input) {
                ++numInputs;
            } else {
                ++numOutputs;
            }
            return;
        });
        if (numInputs != m_inputPorts.size() || numOutputs != m_outputPorts.size()) {
            return false;
        }
        std::vector<AudioBus*> usedInputPorts, usedOutputPorts;
        for (const auto& config : configView) {
            const auto isInput = config.is_input;
            const auto channelCount = config.channel_count;
            auto& vecToQuery = isInput ? m_inputPorts : m_outputPorts;
            auto& usedPorts = isInput ? usedInputPorts : usedOutputPorts;
            auto it = std::find_if(vecToQuery.begin(), vecToQuery.end(), [&usedPorts, channelCount](const AudioBus& port) -> bool {
                const auto alreadyUsed = std::find(usedPorts.begin(), usedPorts.end(), &port) != usedPorts.end();
                const auto containsChannelCount = std::find(port.supportedNumChannels.begin(), port.supportedNumChannels.end(), channelCount) != port.supportedNumChannels.end();
                return !alreadyUsed && containsChannelCount;
            });
            if (it == vecToQuery.end()) {
                // No port available, bail.
                return false;
            }
            usedPorts.emplace_back(&(*it));
        }
        return true;
    }

} // namespace mostly_harmless::core