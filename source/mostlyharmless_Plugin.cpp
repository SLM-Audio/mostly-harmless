#include "clap/events.h"
#include "clap/ext/note-ports.h"
#include "clap/ext/params.h"
#include "clap/helpers/plugin.hxx"
#include "clap/process.h"
#include "clap/string-sizes.h"
#include "marvin/containers/marvin_BufferView.h"
#include "marvin/library/marvin_Concepts.h"
#include "mostlyharmless_BusConfig.h"
#include "mostlyharmless_EventContext.h"
#include "mostlyharmless_Parameters.h"
#include <iomanip>
#include <limits>
#include <mostlyharmless_Descriptor.h>
#include <mostlyharmless_Plugin.h>
#include <sstream>
namespace mostly_harmless {
    template <marvin::FloatType SampleType>
    Plugin<SampleType>::Plugin(const clap_host* host, std::vector<Parameter<SampleType>>&& params) : clap::helpers::Plugin<clap::helpers::MisbehaviourHandler::Terminate, clap::helpers::CheckingLevel::Maximal>(&getDescriptor(), host),
                                                                                                     m_indexedParams(std::move(params)) {
        for (auto& p : m_indexedParams) {
            m_idParams.emplace(p.pid, &p);
        }
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::activate(double sampleRate, std::uint32_t minFrameCount, std::uint32_t maxFrameCount) noexcept {
        initialise(sampleRate, minFrameCount, maxFrameCount);
        return true;
    }

    template <marvin::FloatType SampleType>
    Parameter<SampleType>* Plugin<SampleType>::getParameter(clap_id id) noexcept {
        assert(m_idParams.find(id) != m_idParams.end());
        return m_idParams.at(id);
    }

    template <marvin::FloatType SampleType>
    clap_process_status Plugin<SampleType>::process(const clap_process* processContext) noexcept {
        EventContext context{ processContext->in_events };
        if (processContext->audio_outputs_count == 0) {
            return CLAP_PROCESS_SLEEP;
        }
        return CLAP_PROCESS_CONTINUE;
    }

    template <marvin::FloatType SampleType>
    void Plugin<SampleType>::pollEventQueue(size_t currentSample, EventContext context) noexcept {
        while (context.next() && context.next()->time == currentSample) {
            handleEvent(context.next());
            ++context;
        }
    }

    template <marvin::FloatType SampleType>
    void Plugin<SampleType>::handleEvent(const clap_event_header_t* event) noexcept {
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::implementsParams() const noexcept {
        return true;
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::isValidParamId(clap_id id) const noexcept {
        return m_idParams.find(id) != m_idParams.end();
    }

    template <marvin::FloatType SampleType>
    std::uint32_t Plugin<SampleType>::paramsCount() const noexcept {
        return m_indexedParams.size();
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::paramsInfo(std::uint32_t paramIndex, clap_param_info* info) const noexcept {
        if (paramIndex >= m_indexedParams.size()) return false;
        const auto& param = m_indexedParams[paramIndex];
        info->id = param.pid;
        info->flags = param.flags;
        strncpy_s(info->name, param.name.c_str(), CLAP_NAME_SIZE);
        strncpy_s(info->module, param.category.c_str(), CLAP_NAME_SIZE);
        const auto [min, max] = param.range;
        info->min_value = min;
        info->max_value = max;
        info->default_value = param.defaultValue;
        return true;
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::paramsValue(clap_id id, double* value) noexcept {
        if (m_idParams.find(id) == m_idParams.end()) return false;
        *value = static_cast<double>(m_idParams.at(id)->value);
        return true;
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::paramsValueToText(clap_id id, double value, char* display, std::uint32_t size) noexcept {
        if (m_idParams.find(id) == m_idParams.end()) return false;
        std::string res;
        auto* const param = m_idParams.at(id);
        if (param->valueToString) {
            res = param->valueToString(*param, value);
        } else {
            std::ostringstream outStream;
            outStream << std::setprecision(6) << value;
            res = outStream.str();
        }
        strncpy(display, res.c_str(), size);
        display[size - 1] = '\0';
        return true;
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::implementsAudioPorts() const noexcept {
        const auto audioBusConfig = getAudioBusConfig();
        return audioBusConfig != BusConfig::None;
    }

    template <marvin::FloatType SampleType>
    std::uint32_t Plugin<SampleType>::audioPortsCount(bool isInput) const noexcept {
        const auto audioBusConfig = getAudioBusConfig();
        switch (audioBusConfig) {
            case BusConfig::None: {
                return 0;
            }
            case BusConfig::InputOutput: {
                return 1;
            }
            case mostly_harmless::BusConfig::InputOnly: {
                return isInput ? 1 : 0;
            }
            case mostly_harmless::BusConfig::OutputOnly: {
                return isInput ? 0 : 1;
            }
            default: return 0;
        }
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::audioPortsInfo(std::uint32_t index, bool isInput, clap_audio_port_info* info) const noexcept {
        const auto audioBusConfig = getAudioBusConfig();
        if (audioBusConfig == BusConfig::InputOutput || (isInput && audioBusConfig == BusConfig::InputOnly) || (!isInput && audioBusConfig == BusConfig::OutputOnly)) {
            info->id = 0;
            info->in_place_pair = std::numeric_limits<std::uint32_t>::max();
            strncpy_s(info->name, "main", sizeof(info->name));
            info->flags = CLAP_AUDIO_PORT_IS_MAIN;
            info->channel_count = 2;
            info->port_type = CLAP_PORT_STEREO;
            return true;
        }
        return false;
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::implementsNotePorts() const noexcept {
        const auto noteBusConfig = getNoteBusConfig();
        return noteBusConfig != BusConfig::None;
    }

    template <marvin::FloatType SampleType>
    std::uint32_t Plugin<SampleType>::notePortsCount(bool isInput) const noexcept {
        const auto noteBusConfig = getNoteBusConfig();
        switch (noteBusConfig) {
            case BusConfig::None: {
                return 0;
            }
            case BusConfig::InputOutput: {
                return 1;
            }
            case BusConfig::InputOnly: {
                return isInput ? 1 : 0;
            }
            case BusConfig::OutputOnly: {
                return isInput ? 0 : 1;
            }
            default: return 0;
        }
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::notePortsInfo(std::uint32_t index, bool isInput, clap_note_port_info* info) const noexcept {
        const auto noteBusConfig = getNoteBusConfig();
        if (noteBusConfig == BusConfig::InputOutput || (isInput && noteBusConfig == BusConfig::InputOnly) || (!isInput && noteBusConfig == BusConfig::OutputOnly)) {
            info->id = 1;
            info->supported_dialects = CLAP_NOTE_DIALECT_MIDI | CLAP_NOTE_DIALECT_CLAP;
            info->preferred_dialect = CLAP_NOTE_DIALECT_CLAP;
            strncpy_s(info->name, "NoteInput", CLAP_NAME_SIZE);
            return true;
        }
        return false;
    }

    template class Plugin<float>;
    template class Plugin<double>;
} // namespace mostly_harmless