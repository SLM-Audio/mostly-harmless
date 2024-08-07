#ifndef MOSTLYHARMLESS_PLUGIN_H
#define MOSTLYHARMLESS_PLUGIN_H
#include "clap/plugin-features.h"
#include "mostlyharmless_Descriptor.h"
#include "clap/helpers/checking-level.hh"
#include "clap/helpers/misbehaviour-handler.hh"
#include "clap/helpers/plugin.hh"
#include <marvin/library/marvin_Concepts.h>
#include <marvin/containers/marvin_BufferView.h>
#include <fstream>
#include <iostream>
#define MOSTLYHARMLESS_REGISTER(ProcessorType) \
namespace mostly_harmless::entry { \
    const clap_plugin* clap_create_plugin(const clap_plugin_factory* f, const clap_host* h, const char* id) { \
        auto* p = new ProcessorType(h); \
        return p->clapPlugin(); \
    } \
}

namespace mostly_harmless { 
    template <marvin::FloatType SampleType>
    class Plugin : public clap::helpers::Plugin<clap::helpers::MisbehaviourHandler::Terminate, clap::helpers::CheckingLevel::Maximal> { 
    public: 
        explicit Plugin(const clap_host* host);
        ~Plugin() noexcept = default;
        virtual void initialise(double sampleRate, std::uint32_t minFrameCount, std::uint32_t maxFrameCount) noexcept = 0;
        virtual void process(marvin::containers::BufferView<SampleType> buffer) noexcept = 0;
    protected:
        bool activate(double sampleRate, std::uint32_t minFrameCount, std::uint32_t maxFrameCount) noexcept override;
        clap_process_status process(const clap_process* processContext) noexcept override;
        [[nodiscard]] bool implementsAudioPorts() const noexcept override;
        [[nodiscard]] std::uint32_t audioPortsCount(bool isInput) const noexcept override;
        [[nodiscard]] bool audioPortsInfo(std::uint32_t index, bool isInput, clap_audio_port_info* info) const noexcept override;
        [[nodiscard]] bool implementsNotePorts() const noexcept override;
        [[nodiscard]] std::uint32_t notePortsCount(bool isInput) const noexcept override;
        [[nodiscard]] bool notePortsInfo(std::uint32_t index, bool isInput, clap_note_port_info* info) const noexcept override;

    };
}
#endif