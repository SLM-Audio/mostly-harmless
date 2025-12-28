//
// Created by Syl Morrison on 20/10/2024.
//
#include "mostly_harmless/utils/mostlyharmless_Visitor.h"


#include <mostly_harmless/mostlyharmless_PluginBase.h>
#include <mostly_harmless/utils/mostlyharmless_Macros.h>
#include <mostly_harmless/audio/mostlyharmless_AudioHelpers.h>
#include <mostly_harmless/utils/mostlyharmless_NoDenormals.h>
#include <mostly_harmless/events/mostlyharmless_MidiEvent.h>
#include <clap/helpers/plugin.hxx>

namespace mostly_harmless::internal {
    PluginBase::PluginBase(const clap_host* host) : clap::helpers::Plugin<clap::helpers::MisbehaviourHandler::Ignore, clap::helpers::CheckingLevel::Maximal>(&getDescriptor(), host) {
        MH_LOG("PROC: Creating plugin instance...");
        m_pluginEntry = core::createPluginEntry();
        core::SharedStateContext context{
            .runOnMainThread = [this](std::function<void(void)>&& toCall) { runOnMainThread(toCall); },
            .requestParamFlush = [this]() {
                if (_host.canUseParams()) {
                    _host.paramsRequestFlush();
                } },
            .requestParamRescan = [this]() -> void {
                _host.paramsRescan(CLAP_PARAM_RESCAN_VALUES);
            },
            .requestGuiResize = [this](std::uint32_t width, std::uint32_t height) -> bool {
                return _host.guiRequestResize(width, height);
            }
        };
        m_state = m_pluginEntry->createState(std::move(context));
        m_engine = m_pluginEntry->createEngine(m_state.get());

        auto guiDispatchCallback = [this]() -> void {
            auto messageThreadCallback = [this]() -> void {
                auto& procToGuiQueue = m_state->getProcToGuiQueue();
                while (auto nextEvent = procToGuiQueue.tryPop()) {
                    if (!m_editor) continue;
                    m_editor->onParamEvent(*nextEvent);
                }
            };
            runOnMainThread(std::move(messageThreadCallback));
        };
        m_guiDispatchThread.action = std::move(guiDispatchCallback);
    }

    bool PluginBase::activate(double sampleRate, uint32_t minFrameCount, uint32_t maxFrameCount) noexcept {
        core::InitContext context{
            .sampleRate = sampleRate,
            .minBufferSize = minFrameCount,
            .maxBufferSize = maxFrameCount
        };
        m_engine->initialise(context);
        return true;
    }

    clap_process_status PluginBase::process(const clap_process* processContext) noexcept {

        [[maybe_unused]] utils::NoDenormals stopDenormals;
        if (processContext->audio_outputs_count == 0) {
            return CLAP_PROCESS_SLEEP;
        }
        events::InputEventContext eventContext{ processContext->in_events };

        handleGuiEvents(processContext->out_events);
        const auto* inputData = processContext->audio_inputs;
        auto* outputData = processContext->audio_outputs;
        if (!outputData) { // TODO: Support no output
            return CLAP_PROCESS_SLEEP;
        }
        auto** outDataPtr = outputData->data32;
        if (inputData) {
            auto** inDataPtr = inputData->data32;
            for (std::uint32_t i = 0; i < outputData->channel_count; ++i) {
                if (i >= inputData->channel_count) {
                    const auto inIndex = inputData->channel_count - 1;
                    std::memcpy(outDataPtr[i], inDataPtr[inIndex], sizeof(float) * processContext->frames_count);
                    continue;
                }
                if (inDataPtr[i] == outDataPtr[i]) {
                    continue;
                }
                std::memcpy(outDataPtr[i], inDataPtr[i], sizeof(float) * processContext->frames_count);
            }
        }
        marvin::containers::BufferView<float> bufferView{ outDataPtr, outputData->channel_count, processContext->frames_count };
        // clang-format off
        runBlockDispatch<float>(bufferView, eventContext,
            [this](const clap_event_header* event) -> void {
                handleEvent(event);
            },
            [this](marvin::containers::BufferView<float> buffer) -> void {
                core::ProcessContext context {
                    .buffer = buffer,
                    .transportState = m_lastTransportState
                };
                m_engine->process(context);
            });
        // clang-format on
        return CLAP_PROCESS_SLEEP;
    }

    void PluginBase::paramsFlush(const clap_input_events* in, const clap_output_events* /*out*/) noexcept {
        events::InputEventContext eventContext{ in };
        while (eventContext.next()) {
            handleEvent(eventContext.next());
            ++eventContext;
        }
    }

    void PluginBase::handleGuiEvents(const clap_output_events_t* outputQueue) noexcept {
        using EventType = events::GuiToProcParamEvent::Type;
        while (auto guiEvent = m_state->getGuiToProcQueue().tryPop()) {
            [[maybe_unused]] const auto [type, id, value] = *guiEvent;
            switch (type) {
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
                    break;
                }
                case EventType::Adjust: {
                    auto* param = m_state->getParameterById(id);
                    param->value = static_cast<float>(value);
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
                }
                default: break;
            }
        }
    }

    void PluginBase::handleEvent(const clap_event_header_t* event) noexcept {
        if (!event) return;
        if (event->space_id != CLAP_CORE_EVENT_SPACE_ID) return;
        switch (event->type) {
            case CLAP_EVENT_PARAM_VALUE: {
                const auto v = reinterpret_cast<const clap_event_param_value*>(event);
                const auto id = v->param_id;
                const auto paramValue = v->value;
                auto* param = static_cast<Parameter<float>*>(v->cookie);
                if (!param) [[unlikely]] {
                    param = m_state->getParameterById(id);
                }
                param->value = static_cast<float>(paramValue);
                m_state->getProcToGuiQueue().tryPush({ id, paramValue });
                break;
            }
            case CLAP_EVENT_NOTE_ON: {
                const auto* noteEv = reinterpret_cast<const clap_event_note*>(event);
                m_engine->handleNoteOn(noteEv->port_index, static_cast<std::uint8_t>(noteEv->channel), static_cast<std::uint8_t>(noteEv->key), noteEv->velocity);
                break;
            }
            case CLAP_EVENT_NOTE_OFF: {
                const auto* noteEv = reinterpret_cast<const clap_event_note*>(event);
                m_engine->handleNoteOff(noteEv->port_index, static_cast<std::uint8_t>(noteEv->channel), static_cast<std::uint8_t>(noteEv->key), noteEv->velocity);
                break;
            }
            case CLAP_EVENT_MIDI: {
                // SSSS CCCC
                // 0PPP PPPP
                // 0VVV VVVV
                const auto* midiEvent = reinterpret_cast<const clap_event_midi*>(event);
                auto res = mostly_harmless::events::midi::parse(midiEvent->data[0], midiEvent->data[1], midiEvent->data[2]);
                if (!res) {
                    return;
                }
                std::visit(utils::Visitor{
                               [this, &midiEvent](events::midi::NoteOff x) {
                                   m_engine->handleNoteOff(midiEvent->port_index, x.channel, x.note, x.velocity);
                               },
                               [this, &midiEvent](events::midi::NoteOn x) {
                                   m_engine->handleNoteOn(midiEvent->port_index, x.channel, x.note, x.velocity);
                               },
                               [this, &midiEvent](events::midi::PolyAftertouch x) {
                                   m_engine->handlePolyAftertouch(midiEvent->port_index, x.channel, x.note, x.pressure);
                               },
                               [this, &midiEvent](events::midi::ControlChange x) {
                                   m_engine->handleControlChange(midiEvent->port_index, x.channel, x.controllerNumber, x.data);
                               },
                               [this, &midiEvent](events::midi::ProgramChange x) {
                                   m_engine->handleProgramChange(midiEvent->port_index, x.channel, x.programNumber);
                               },
                               [this, &midiEvent](events::midi::ChannelAftertouch x) {
                                   m_engine->handleChannelAftertouch(midiEvent->port_index, x.channel, x.pressure);
                               },
                               [this, &midiEvent](events::midi::PitchWheel x) {
                                   m_engine->handlePitchWheel(midiEvent->port_index, x.channel, x.value);
                               } },
                           *res);
            }
            case CLAP_EVENT_TRANSPORT: {
                if (const auto* transportEvent = reinterpret_cast<const clap_event_transport_t*>(event)) {
                    TransportState state{ transportEvent };
                    m_lastTransportState = state;
                }
                break;
            }
            case CLAP_EVENT_PARAM_MOD: [[fallthrough]]; // TODO
            default: break;
        }
    }

    bool PluginBase::implementsParams() const noexcept {
        MH_LOG("PARAM: implementsParams()");
        return true;
    }

    bool PluginBase::isValidParamId(clap_id id) const noexcept {
        auto* paramWithId = m_state->getParameterById(id);
        return paramWithId != nullptr;
    }

    std::uint32_t PluginBase::paramsCount() const noexcept {
        return m_state->getNumParams();
    }

    bool PluginBase::paramsInfo(std::uint32_t paramIndex, clap_param_info* info) const noexcept {
        if (paramIndex >= m_state->getNumParams()) return false;
        auto* param = m_state->getParameterByIndex(paramIndex);
        info->id = param->pid;
        info->flags = param->flags;
        strncpy(info->name, param->name.c_str(), CLAP_NAME_SIZE);
        strncpy(info->module, param->category.c_str(), CLAP_NAME_SIZE);
        const auto [min, max] = param->range;
        info->min_value = min;
        info->max_value = max;
        info->default_value = param->defaultValue;
        info->cookie = (void*)param;
        return true;
    }

    bool PluginBase::paramsValue(clap_id id, double* value) noexcept {
        auto* param = m_state->getParameterById(id);
        if (!param) return false;
        *value = param->value;
        return true;
    }

    bool PluginBase::implementsAudioPorts() const noexcept {
        MH_LOG("PROC: implementsAudioPorts()");
        const auto audioBusConfig = getAudioBusConfig();
        return audioBusConfig != BusConfig::None;
    }

    std::uint32_t PluginBase::audioPortsCount(bool isInput) const noexcept {
        MH_LOG("PROC: audioPortsCount()");
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

    bool PluginBase::audioPortsInfo(std::uint32_t /*index*/, bool isInput, clap_audio_port_info* info) const noexcept {
        MH_LOG("PROC: audioPortsInfo()");
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

    bool PluginBase::implementsNotePorts() const noexcept {
        MH_LOG("PROC: implementsNotePorts()");
        const auto noteBusConfig = getNoteBusConfig();
        return noteBusConfig != BusConfig::None;
    }

    std::uint32_t PluginBase::notePortsCount(bool isInput) const noexcept {
        MH_LOG("PROC: notePortsCount()");
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

    bool PluginBase::notePortsInfo(std::uint32_t /*index*/, bool isInput, clap_note_port_info* info) const noexcept {
        MH_LOG("PROC: notePortsInfo()");
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

    bool PluginBase::implementsState() const noexcept {
        MH_LOG("STAT: implementsState()");
        return true;
    }

    bool PluginBase::stateSave(const clap_ostream* stream) noexcept {
        MH_LOG("STAT: stateSave()");
        std::ostringstream dest;
        m_state->saveState(dest);
        const auto asString = dest.str();
        auto bytesRemaining{ asString.size() };
        auto* write = asString.c_str();
        while (bytesRemaining > 0) {
            const auto actualBytesWritten = stream->write(stream, (void*)write, bytesRemaining);
            bytesRemaining -= actualBytesWritten;
            write += actualBytesWritten;
        }
        return true;
    }

    bool PluginBase::stateLoad(const clap_istream* stream) noexcept {
        MH_LOG("STAT: stateLoad()");
        constexpr static auto maxSize{ static_cast<size_t>(4096 * 8) };
        std::vector<char> buffer(maxSize);
        constexpr static auto blockSize{ 64U };
        std::int64_t bytesRead{ 0U };
        std::int64_t inferredSize{ 0U };
        auto* read = buffer.data();
        while ((bytesRead = stream->read(stream, (void*)read, blockSize)) > 0) {
            read += bytesRead;
            inferredSize += bytesRead;
        }
        if (inferredSize == 0) return false;
        std::string asStr{ buffer.begin(), buffer.begin() + static_cast<std::ptrdiff_t>(inferredSize) };
        m_state->loadState(asStr);
        for (std::uint32_t paramIndex = 0; paramIndex < m_state->getNumParams(); ++paramIndex) {
            auto* param = m_state->getParameterByIndex(paramIndex);
            const auto pid = param->pid;
            const auto value = param->value;
            m_state->getProcToGuiQueue().tryPush({ .paramId = pid, .value = value });
        }
        _host.paramsRescan(CLAP_PARAM_RESCAN_VALUES);
        return true;
    }

    bool PluginBase::implementsGui() const noexcept {
        MH_LOG("GUI: implementsGui()");
        return m_pluginEntry->hasGui();
    }

    bool PluginBase::guiIsApiSupported(const char* api, bool isFloating) noexcept {
        MH_LOG("GUI: guiIsApiSupported()");
        if (isFloating) return true;
#if defined(MOSTLY_HARMLESS_WINDOWS)
        return strcmp(api, CLAP_WINDOW_API_WIN32) == 0;
#elif defined(MOSTLY_HARMLESS_MACOS)
        return strcmp(api, CLAP_WINDOW_API_COCOA) == 0;
#else
        static_assert(false);
#endif
    }

    bool PluginBase::guiCreate(const char* /*api*/, bool /*isFloating*/) noexcept {
        MH_LOG("GUI: guiCreate()");
        m_editor = m_pluginEntry->createEditor(m_state.get());
        m_editor->initialise();
        m_guiDispatchThread.run(1);
        return true;
    }

    void PluginBase::guiDestroy() noexcept {
        MH_LOG("GUI: guiDestroy()");
        m_guiDispatchThread.stop();
        m_editor.reset();
    }

    bool PluginBase::guiSetParent(const clap_window* window) noexcept {
        MH_LOG("GUI: guiSetParent()");
#if defined(MOSTLY_HARMLESS_WINDOWS)
        m_editor->setParent(window->win32);
#elif defined(MOSTLY_HARMLESS_MACOS)
        m_editor->setParent(window->cocoa);
#else
        static_assert(false);
#endif
        return true;
    }

    bool PluginBase::guiSetScale(double /*scale*/) noexcept {
        MH_LOG("GUI: guiSetScale()");
        return false; // TODO
    }

    bool PluginBase::guiCanResize() const noexcept {
        MH_LOG("GUI: guiCanResize()");
        if (!m_editor) return false;
        return m_editor->allowResize();
    }

    bool PluginBase::guiAdjustSize(std::uint32_t* width, std::uint32_t* height) noexcept {
        MH_LOG("GUI: guiAdjustSize()");
        if (!m_editor) return false;
        if (!m_editor->allowResize()) return false;
        m_editor->onResizeRequested(width, height);
        return true;
    }

    bool PluginBase::guiSetSize(std::uint32_t width, std::uint32_t height) noexcept {
        MH_LOG("GUI: guiSetSize()");
        if (!m_editor) return false;
        m_editor->setSize(width, height);
        return true;
    }

    bool PluginBase::guiGetSize(std::uint32_t* width, std::uint32_t* height) noexcept {
        MH_LOG("GUI: guiGetSize()");
        m_editor->getSize(width, height);
        return true;
    }

    bool PluginBase::guiShow() noexcept {
        if (!m_editor) return false;
        m_editor->show();
        return true;
    }

    bool PluginBase::guiHide() noexcept {
        if (!m_editor) return false;
        m_editor->hide();
        return true;
    }

} // namespace mostly_harmless::internal