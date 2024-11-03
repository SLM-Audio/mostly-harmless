//
// Created by Syl Morrison on 03/11/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_BUSLAYOUT_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_BUSLAYOUT_H
#include <clap/ext/configurable-audio-ports.h>
#include <span>
#include <vector>

namespace mostly_harmless::core {
    struct AudioBus {
        bool input;
        std::vector<std::uint32_t> supportedNumChannels;
    };

    class BusLayout final {
    public:
        explicit BusLayout(std::vector<AudioBus>&& inputs, std::vector<AudioBus>&& outputs);
        [[nodiscard]] bool satisfiesClapConfig(const clap_audio_port_configuration_request* configs, std::uint32_t requestCount);

    private:
        std::vector<AudioBus> m_inputPorts;
        std::vector<AudioBus> m_outputPorts;
    };
} // namespace mostly_harmless::core

#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_BUSLAYOUT_H
