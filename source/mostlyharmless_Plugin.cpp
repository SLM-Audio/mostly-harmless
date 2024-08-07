#include "clap/helpers/plugin.hxx"
#include "clap/process.h"
#include "marvin/containers/marvin_BufferView.h"
#include <limits>
#include <mostlyharmless_Descriptor.h>
#include <mostlyharmless_Plugin.h>
namespace mostly_harmless { 
    template<marvin::FloatType SampleType> 
    Plugin<SampleType>::Plugin(const clap_host* host) : 
    clap::helpers::Plugin<clap::helpers::MisbehaviourHandler::Terminate, clap::helpers::CheckingLevel::Maximal>(&getDescriptor(), host) { 

    }
    template<marvin::FloatType SampleType>
    bool Plugin<SampleType>::activate(double sampleRate, std::uint32_t minFrameCount, std::uint32_t maxFrameCount) noexcept { 
        initialise(sampleRate, minFrameCount, maxFrameCount);
        return true;
    }

    template<marvin::FloatType SampleType> 
    clap_process_status Plugin<SampleType>::process(const clap_process* processContext) noexcept { 
        const auto* audioIn = processContext->audio_inputs;
        const auto inCount = processContext->audio_inputs_count;
        auto* audioOut = processContext->audio_outputs;
        const auto outCount = processContext->audio_outputs;
        SampleType** inPtr{ nullptr }, **outPtr{ nullptr };
        if constexpr(std::same_as<SampleType, float>) { 
            inPtr = audioIn->data32;
            outPtr = audioOut->data32;
        }
        else { 
            inPtr = audioIn->data64;
            outPtr = audioOut->data64;
        }
        if(inPtr != outPtr) { 
            for(std::uint32_t channel = 0; channel < audioIn->channel_count; ++channel) { 
                std::memcpy(outPtr[channel], inPtr[channel], sizeof(SampleType) * processContext->frames_count);
            }
        }
        marvin::containers::BufferView<SampleType> bufferView{ outPtr, static_cast<size_t>(audioOut->channel_count), static_cast<size_t>(processContext->frames_count) };
        process(bufferView);
        return CLAP_PROCESS_CONTINUE;
    }

    template<marvin::FloatType SampleType>
    bool Plugin<SampleType>::implementsAudioPorts() const noexcept { 
        return true; // Todo: midi only? 
    }

    template<marvin::FloatType SampleType> 
    std::uint32_t Plugin<SampleType>::audioPortsCount(bool isInput) const noexcept { 
        return 1; // Todo: Handle output only
    }

    template<marvin::FloatType SampleType> 
    bool Plugin<SampleType>::audioPortsInfo(std::uint32_t index, bool isInput, clap_audio_port_info* info) const noexcept { 
        info->id = 0;
        info->in_place_pair = std::numeric_limits<std::uint32_t>::max();
        strncpy_s(info->name, "main", sizeof(info->name));
        info->flags = CLAP_AUDIO_PORT_IS_MAIN;
        info->channel_count = 2;
        info->port_type = CLAP_PORT_STEREO;
        return true;
    }

    template<marvin::FloatType SampleType> 
    bool Plugin<SampleType>::implementsNotePorts() const noexcept { 
        return false;
    }

    template<marvin::FloatType SampleType> 
    std::uint32_t Plugin<SampleType>::notePortsCount(bool isInput) const noexcept { 
        return 0;
    }

    template<marvin::FloatType SampleType> 
    bool Plugin<SampleType>::notePortsInfo(std::uint32_t index, bool isInput, clap_note_port_info* info) const noexcept { 
        return false;
    }

    template class Plugin<float>;
    template class Plugin<double>;
}