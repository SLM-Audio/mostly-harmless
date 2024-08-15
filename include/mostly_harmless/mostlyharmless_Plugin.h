#ifndef MOSTLYHARMLESS_PLUGIN_H
#define MOSTLYHARMLESS_PLUGIN_H
#include <mostly_harmless/mostlyharmless_Descriptor.h>
#include "mostly_harmless/events/mostlyharmless_InputEventContext.h"
#include <mostly_harmless/mostlyharmless_Parameters.h>
#include <mostly_harmless/gui/mostlyharmless_IEditor.h>
#include <mostly_harmless/utils/mostlyharmless_Timer.h>
#include <mostly_harmless/events/mostlyharmless_ParamEvent.h>

#include "clap/events.h"
#include "clap/ext/gui.h"
#include "clap/plugin-features.h"
#include "clap/helpers/checking-level.hh"
#include "clap/helpers/misbehaviour-handler.hh"
#include "clap/helpers/plugin.hh"
#include <marvin/library/marvin_Concepts.h>
#include <marvin/containers/marvin_BufferView.h>
#include <marvin/containers/marvin_FIFO.h>
#include <fstream>
#include <iostream>
#include <unordered_map>
/**
 * Macro to register your custom plugin type with the clap factory.
 * Must be called at the end of your plugin type's source file, with `MOSTLYHARMLESS_REGISTER(YourPluginType)`.
 * Failure to do so will result in no entry point being found, and the plugin not working.
 */
#define MOSTLYHARMLESS_REGISTER(ProcessorType)                                                                        \
    namespace mostly_harmless::entry {                                                                                \
        const clap_plugin* clap_create_plugin(const clap_plugin_factory* /*f*/, const clap_host* h, const char* id) { \
            auto& desc = getDescriptor();                                                                             \
            if (std::strcmp(desc.id, id) != 0) {                                                                      \
                return nullptr;                                                                                       \
            }                                                                                                         \
            auto* p = new ::ProcessorType(h);                                                                         \
            return p->clapPlugin();                                                                                   \
        }                                                                                                             \
    }

namespace mostly_harmless {

    /**
     * \brief The base class for Plugins to subclass.
     *
     * To register your subclassed plugin type with the clap factory, you <b>must</b> call `MOSTLYHARMLESS_REGISTER(YourPluginType)` at the end of your plugin source file.
     */
    template <marvin::FloatType SampleType>
    class Plugin : public clap::helpers::Plugin<clap::helpers::MisbehaviourHandler::Terminate, clap::helpers::CheckingLevel::Maximal> {
    public:
        /**
         * To establish parameters with the plugin, create a vector of them, and std::move them into the `params` arg here.
         * (In my opinion) the cleanest way to do so is with a free function - for example:
         * ```cpp
         * std::vector<mostly_harmless::Parameter<float>> createParams() noexcept {
         *      return {
         *          mostly_harmless::Parameter<float>{ 0, "MyParam", "Plugin", { 0.0f, 1.0f }, 1.0f, CLAP_PARAM_IS_AUTOMATABLE }
         *      }
         * }
         *
         * class YourPluginType : public mostly_harmless::Plugin<float> {
         * public:
         *     YourPluginType(const clap_host* host) : mostly_harmless::Plugin<float>(host, createParams()) {
         *     }
         *     // The rest of the boilerplate
         * };
         * ```
         * \param host A clap internal type provided when the plugin is created by the factory - don't worry about it, just make sure your PluginType subclass also takes one to its constructor.
         * \param params An rvalue ref to a vector of type mostly_harmless::Parameter.
         */
        explicit Plugin(const clap_host* host, std::vector<Parameter<SampleType>>&& params);
        ~Plugin() noexcept override = default;
        /**
         * Called by the host when the audio is getting ready to play - you should do any audio initialisation you need here.
         * \param sampleRate The sample rate we're currently running at.
         * \param minFrameCount The minimum amount of samples a buffer might contain.
         * \param maxFrameCount The maximum amount of samples a buffer might contain.
         */
        virtual void initialise(double sampleRate, std::uint32_t minFrameCount, std::uint32_t maxFrameCount) noexcept = 0;
        /**
         * The main audio processing function (will run on the audio thread). All audio process should be done in (or called from) this function.
         * We provide a convenience function to handle the events in the EventContext, for more info see Plugin::pollEventQueue.
         * \param buffer A non owning view into the host provided buffer.
         * \param inputEventTonext A trivially copyable wrapper around the host provided clap input event queue.
         */
        virtual void process(marvin::containers::BufferView<SampleType> buffer, events::InputEventContext inputEventContext) noexcept = 0;
        /**
         * Called when audio processing is bypassed, to allow param updates to continue to update the gui.
         * \param inputEventContext A trivially copyable wrapper around the host provided clap event queue.
         */
        virtual void flushParams(events::InputEventContext inputEventContext) noexcept = 0;

        /**
         * Called to clear all audio buffers, and state, etc.
         */
        void reset() noexcept override = 0;
        /**
            Implement this to supply your own custom gui editor using a framework of your choice.
            Called internally when the gui is created,  - we internally take ownership of the pointer allocated here.
            \return An allocated editor class deriving from `gui::IEditor`, for us to take ownership of.
        */
        virtual std::unique_ptr<gui::IEditor> createEditor() noexcept = 0;

    protected:
        /**
            Retrieves a parameter by its param id was constructed with.
            \param id The id to retrieve
            \return A pointer to the associated parameter.
         */
        [[nodiscard]] Parameter<SampleType>* getParameter(clap_id id) noexcept;

        /**
            Convenience function to handle incoming events for the current sample
            \param currentSample The index of the current sample into the block.
            \param context The InputEventContext containing the event queue.
         */
        void pollEventQueue(size_t currentSample, events::InputEventContext context) noexcept;

        /**
         *  Convenience function to handle all incoming events for the current block.
         *  \param context InputEventContext containing the event queue.
         */
        void pollEventQueue(events::InputEventContext context) noexcept;

        /**
            Called Internally by pollEventQueue when a midi note on message is received.
            If note on functionality is required, make sure you override this function!
            \param portIndex The port this event occured on
            \param channel The midi channel this event was targeted at
            \param note The midi note value
            \param velocity The midi velocity value
         */
        virtual void handleNoteOn([[maybe_unused]] std::uint16_t portIndex, [[maybe_unused]] std::uint8_t channel, [[maybe_unused]] std::uint8_t note, [[maybe_unused]] double velocity) { assert(false); }
        /**
            Called Internally by pollEventQueue when a midi note off message is received.
            If note off functionality is required, make sure you override this function!
            \param portIndex The port this event occured on
            \param channel The midi channel this event was targeted at
            \param note The midi note value
            \param velocity The midi velocity value
         */
        virtual void handleNoteOff([[maybe_unused]] std::uint16_t portIndex, [[maybe_unused]] std::uint8_t channel, [[maybe_unused]] std::uint8_t note, [[maybe_unused]] double velocity) { assert(false); }

        /**
         * Called when the host needs to serialize the plugin's current state.
         * \param data An (uninitialised) vector of unsigned chars to write your state to.
         */
        virtual void saveState(std::ostringstream& stream) = 0;

        /**
         * Called when the host has loaded a plugin's state.
         * \param data A vector of unsigned chars holding the loaded data.
         */
        virtual void loadState(std::string_view loaded) = 0;

    private:
        bool activate(double sampleRate, std::uint32_t minFrameCount, std::uint32_t maxFrameCount) noexcept override;
        clap_process_status process(const clap_process* processContext) noexcept override;
        void paramsFlush(const clap_input_events* in, const clap_output_events* out) noexcept override;
        void handleEvent(const clap_event_header_t* event) noexcept;
        void handleGuiEvents(const clap_output_events_t* outputQueue) noexcept;

        [[nodiscard]] bool implementsParams() const noexcept override;
        [[nodiscard]] bool isValidParamId(clap_id paramId) const noexcept override;
        [[nodiscard]] std::uint32_t paramsCount() const noexcept override;
        [[nodiscard]] bool paramsInfo(std::uint32_t paramIndex, clap_param_info* info) const noexcept override;
        [[nodiscard]] bool paramsValue(clap_id id, double* value) noexcept override;
        [[nodiscard]] bool paramsValueToText(clap_id id, double value, char* display, std::uint32_t size) noexcept override;
        [[nodiscard]] bool implementsAudioPorts() const noexcept override;
        [[nodiscard]] std::uint32_t audioPortsCount(bool isInput) const noexcept override;
        [[nodiscard]] bool audioPortsInfo(std::uint32_t index, bool isInput, clap_audio_port_info* info) const noexcept override;
        [[nodiscard]] bool implementsNotePorts() const noexcept override;
        [[nodiscard]] std::uint32_t notePortsCount(bool isInput) const noexcept override;
        [[nodiscard]] bool notePortsInfo(std::uint32_t index, bool isInput, clap_note_port_info* info) const noexcept override;

        [[nodiscard]] bool implementsState() const noexcept override;
        [[nodiscard]] bool stateSave(const clap_ostream* stream) noexcept override;
        [[nodiscard]] bool stateLoad(const clap_istream* stream) noexcept override;

        [[nodiscard]] bool implementsGui() const noexcept override;
        [[nodiscard]] bool guiIsApiSupported(const char* api, bool isFloating) noexcept override;
        [[nodiscard]] bool guiCreate(const char* api, bool isFloating) noexcept override;
        void guiDestroy() noexcept override;
        [[nodiscard]] bool guiSetParent(const clap_window* window) noexcept override;
        [[nodiscard]] bool guiSetScale(double scale) noexcept override;
        [[nodiscard]] bool guiCanResize() const noexcept override;
        [[nodiscard]] bool guiAdjustSize(std::uint32_t* width, std::uint32_t* height) noexcept override;
        [[nodiscard]] bool guiSetSize(std::uint32_t width, std::uint32_t height) noexcept override;
        [[nodiscard]] bool guiGetSize(std::uint32_t* width, std::uint32_t* height) noexcept override;


    private:
        std::vector<Parameter<SampleType>> m_indexedParams;
        std::unordered_map<clap_id, Parameter<SampleType>*> m_idParams;
        utils::Timer m_guiDispatchThread;
        std::unique_ptr<gui::IEditor> m_editor{ nullptr };

    protected:
        /**
         * Realtime/thread-safe fifo used for posting param updates to the gui thread. Used internally by both overloads of pollEventQueue().
         */
        marvin::containers::fifos::SPSC<events::ProcToGuiParamEvent> m_procToGuiQueue;
        /**
         * Realtime/thread-safe fifo used for posting param updates from the gui to the audio thread. gui::IEditor gets a pointer to this queue.
         */
        marvin::containers::fifos::SPSC<events::GuiToProcParamEvent> m_guiToProcQueue;
    };
} // namespace mostly_harmless
#endif