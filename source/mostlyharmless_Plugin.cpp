#include "clap/helpers/plugin.hxx"
#include <mostly_harmless/mostlyharmless_BusConfig.h>
#include <mostly_harmless/mostlyharmless_Plugin.h>
#include <sstream>
#include <string>
#include <iomanip>

namespace mostly_harmless {
    template <marvin::FloatType SampleType>
    Plugin<SampleType>::Plugin(const clap_host* host, std::vector<Parameter<SampleType>>&& params) : clap::helpers::Plugin<clap::helpers::MisbehaviourHandler::Terminate, clap::helpers::CheckingLevel::Maximal>(&getDescriptor(), host),
                                                                                                     m_indexedParams(std::move(params)),
                                                                                                     m_procToGuiQueue(100),
                                                                                                     m_guiToProcQueue(100){
        for (auto& p : m_indexedParams) {
            m_idParams.emplace(p.pid, &p);
        }
        auto guiDispatchCallback = [this]() -> void {
            auto messageThreadCallback = [this]() -> void {
              while(auto nextEvent = m_procToGuiQueue.tryPop()) {
                  if(!m_editor) continue;
                  m_editor->onParamEvent(*nextEvent);
              }
            };
            runOnMainThread(std::move(messageThreadCallback));
        };
        m_guiDispatchThread.action = std::move(guiDispatchCallback);
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
        InputEventContext context{ processContext->in_events };
        if (processContext->audio_outputs_count == 0) {
            return CLAP_PROCESS_SLEEP;
        }
        handleGuiEvents(processContext->out_events);
        // If our input data != our output data, do a copy..
        const auto* inputData = processContext->audio_inputs;
        auto* outputData = processContext->audio_outputs;
        SampleType **inDataPtr, **outDataPtr;
        if constexpr (std::same_as<float, SampleType>) {
            inDataPtr = inputData->data32;
            outDataPtr = outputData->data32;
        } else {
            inDataPtr = inputData->data64;
            outDataPtr = outputData->data64;
        }
        for (std::uint32_t i = 0; i < outputData->channel_count; ++i) {
            // Check if we're out of range..
            if (i >= inputData->channel_count) {
                const auto inIndex = inputData->channel_count - 1;
                std::memcpy(outDataPtr[i], inDataPtr[inIndex], sizeof(SampleType) * processContext->frames_count);
            } else {
                if (inDataPtr[i] == outDataPtr[i]) continue;
                std::memcpy(outDataPtr[i], inDataPtr[i], sizeof(SampleType) * processContext->frames_count);
            }
        }
        // Okay cool - that's taken care of, now make a BufferView..
        marvin::containers::BufferView<SampleType> bufferView{ outDataPtr, outputData->channel_count, processContext->frames_count };
        // and then call the virtual process function..
        process(bufferView, context);
        return CLAP_PROCESS_SLEEP;
    }

    template<marvin::FloatType SampleType>
    void Plugin<SampleType>::paramsFlush(const clap_input_events* in, const clap_output_events* /*out*/) noexcept { // TODO: OUTPUT!
        InputEventContext inContext{ in };
        flushParams(inContext);
    }

    template <marvin::FloatType SampleType>
    void Plugin<SampleType>::pollEventQueue(size_t currentSample, InputEventContext context) noexcept {
        while (context.next() && context.next()->time == currentSample) {
            handleEvent(context.next());
            ++context;
        }
    }

    template<marvin::FloatType SampleType>
    void Plugin<SampleType>::pollEventQueue(mostly_harmless::InputEventContext context) noexcept {
        while(context.next()) {
            handleEvent(context.next());
            ++context;
        }
    }

    template<marvin::FloatType SampleType>
    void Plugin<SampleType>::handleGuiEvents(const clap_output_events_t* outputQueue) noexcept {
        using EventType = events::GuiToProcParamEvent::Type;
        while(auto guiEvent = m_guiToProcQueue.tryPop()) {
            [[maybe_unused]] const auto [type, id, value] = *guiEvent;
            switch(type) {
                case EventType::Begin: [[fallthrough]];
                case EventType::End: {
                    auto ev = clap_event_param_gesture();
                    ev.header.size = sizeof(clap_event_param_gesture);
                    ev.header.type = type == EventType::Begin ? static_cast<std::uint16_t>(CLAP_EVENT_PARAM_GESTURE_BEGIN) : static_cast<std::uint16_t>(CLAP_EVENT_PARAM_GESTURE_END);
                    ev.header.time = 0;
                    ev.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
                    ev.header.flags = 0;
                    ev.param_id = id;
                    outputQueue->try_push(outputQueue, &ev.header);
                    // then push this into the output queue.
                    break;
                }
                case EventType::Adjust: {
                    auto* param = m_idParams.at(id);
                    param->value = static_cast<SampleType>(value);

                    // And then tell the host..
                    auto ev = clap_event_param_value();
                    ev.header.size = sizeof(clap_event_param_value);
                    ev.header.type = static_cast<std::uint16_t>(CLAP_EVENT_PARAM_VALUE);
                    ev.header.time = 0; // clap-saw-demo has an ominous "for now..." here, so TODO: find out why
                    ev.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
                    ev.header.flags = 0;
                    ev.param_id = id;
                    ev.value = value;
                    outputQueue->try_push(outputQueue, &ev.header);
                    // Then push this into the output queue.

                }
                default: break;
            }
        }
    }

    template <marvin::FloatType SampleType>
    void Plugin<SampleType>::handleEvent(const clap_event_header_t* event) noexcept {
        if (event->space_id != CLAP_CORE_EVENT_SPACE_ID) return;
        switch (event->type) {
            case CLAP_EVENT_PARAM_VALUE: {
                const auto v = reinterpret_cast<const clap_event_param_value*>(event);
                const auto id = v->param_id;
                const auto paramValue = v->value;
                auto* param = static_cast<Parameter<SampleType>*>(v->cookie);
                if(!param) [[unlikely]] {
                    param = m_idParams.at(id);
                }
                param->value = static_cast<SampleType>(paramValue);
                // Also inform the UI
                m_procToGuiQueue.tryPush({id, paramValue});
                break;
            }
            case CLAP_EVENT_NOTE_ON: {
                const auto* noteEv = reinterpret_cast<const clap_event_note*>(event);
                handleNoteOn(noteEv->port_index, static_cast<std::uint8_t>(noteEv->channel), static_cast<std::uint8_t>(noteEv->key), noteEv->velocity);
                break;
            }
            case CLAP_EVENT_NOTE_OFF: {
                const auto* noteEv = reinterpret_cast<const clap_event_note*>(event);
                handleNoteOff(noteEv->port_index, static_cast<std::uint8_t>(noteEv->channel), static_cast<std::uint8_t>(noteEv->key), noteEv->velocity);
                break;
            }
            case CLAP_EVENT_MIDI: {
                // SSSS CCCC
                // 0PPP PPPP
                // 0VVV VVVV
                const auto* midiEvent = reinterpret_cast<const clap_event_midi*>(event);
                std::uint8_t message = midiEvent->data[0] & 0xF0; // SSSS 0000 - Message will be << 4
                std::uint8_t channel = midiEvent->data[0] & 0x0F; // 0000 CCCC
                std::uint8_t note = midiEvent->data[1];           // 0PPP PPPP
                std::uint8_t velocity = midiEvent->data[2];       // 0VVV VVVV
                const auto fpVelocity = static_cast<double>(velocity) / 127.0;
                switch (message) {
                    case 0x90: {
                        handleNoteOn(midiEvent->port_index, channel, note, fpVelocity);
                        break;
                    }
                    case 0x80: {
                        handleNoteOff(midiEvent->port_index, channel, note, fpVelocity);
                        break;
                    }
                    default: break; // TODO
                }
            }

            case CLAP_EVENT_PARAM_MOD: [[fallthrough]]; // TODO
            default: break;
        }
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
        return static_cast<std::uint32_t>(m_indexedParams.size());
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::paramsInfo(std::uint32_t paramIndex, clap_param_info* info) const noexcept {
        if (paramIndex >= m_indexedParams.size()) return false;
        auto& param = m_indexedParams[paramIndex];
        info->id = param.pid;
        info->flags = param.flags;
        strncpy(info->name, param.name.c_str(), CLAP_NAME_SIZE);
        strncpy(info->module, param.category.c_str(), CLAP_NAME_SIZE);
        const auto [min, max] = param.range;
        info->min_value = min;
        info->max_value = max;
        info->default_value = param.defaultValue;
        info->cookie = (void*)&param;
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
    bool Plugin<SampleType>::audioPortsInfo(std::uint32_t /*index*/, bool isInput, clap_audio_port_info* info) const noexcept {
        const auto audioBusConfig = getAudioBusConfig();
        if (audioBusConfig == BusConfig::InputOutput || (isInput && audioBusConfig == BusConfig::InputOnly) || (!isInput && audioBusConfig == BusConfig::OutputOnly)) {
            info->id = 0;
            info->in_place_pair = std::numeric_limits<std::uint32_t>::max();
            strncpy(info->name, "main", sizeof(info->name));
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
    bool Plugin<SampleType>::notePortsInfo(std::uint32_t /*index*/, bool isInput, clap_note_port_info* info) const noexcept {
        const auto noteBusConfig = getNoteBusConfig();
        if (noteBusConfig == BusConfig::InputOutput || (isInput && noteBusConfig == BusConfig::InputOnly) || (!isInput && noteBusConfig == BusConfig::OutputOnly)) {
            info->id = 1;
            info->supported_dialects = CLAP_NOTE_DIALECT_MIDI | CLAP_NOTE_DIALECT_CLAP;
            info->preferred_dialect = CLAP_NOTE_DIALECT_CLAP;
            strncpy(info->name, "NoteInput", CLAP_NAME_SIZE);
            return true;
        }
        return false;
    }

    //===========================================================================================
    // Here be gui

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::implementsGui() const noexcept {
        return true;
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::guiIsApiSupported(const char* api, bool isFloating) noexcept {
        if (isFloating) return false;
#if defined(MOSTLY_HARMLESS_WINDOWS)
        return strcmp(api, CLAP_WINDOW_API_WIN32) == 0;
#elif defined(MOSTLY_HARMLESS_MACOS)
        return strcmp(api, CLAP_WINDOW_API_COCOA) == 0;
#else
        static_assert(false);
#endif
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::guiCreate(const char* /*api*/, bool /*isFloating*/) noexcept {
        m_editor = createEditor();
        m_editor->setGuiToProcQueue(&m_guiToProcQueue);
        m_editor->initialise();
        m_guiDispatchThread.run(1);
        return true;
    }

    template <marvin::FloatType SampleType>
    void Plugin<SampleType>::guiDestroy() noexcept {
        m_guiDispatchThread.stop();
        m_editor.reset();
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::guiSetParent(const clap_window* window) noexcept {
#if defined(MOSTLY_HARMLESS_WINDOWS)
        m_editor->setParent(window->win32);
#elif defined(MOSTLY_HARMLESS_MACOS)
        m_editor->setParent(window->cocoa);
#else
        static_assert(false);
#endif
        return true;
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::guiSetScale(double /*scale*/) noexcept {
        return false; // TODO
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::guiCanResize() const noexcept {
        return false; // TODO
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::guiAdjustSize(std::uint32_t* /*width*/, std::uint32_t* /*height*/) noexcept {
        return false; // TODO
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::guiSetSize(std::uint32_t width, std::uint32_t height) noexcept {
        m_editor->setSize(width, height);
        return true;
    }

    template <marvin::FloatType SampleType>
    bool Plugin<SampleType>::guiGetSize(std::uint32_t* width, std::uint32_t* height) noexcept {
        m_editor->getSize(width, height);
        return true;
    }

    template class Plugin<float>;
    template class Plugin<double>;

} // namespace mostly_harmless