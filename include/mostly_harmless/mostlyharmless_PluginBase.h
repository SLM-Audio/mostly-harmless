//
// Created by Syl Morrison on 20/10/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_PLUGINBASE_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_PLUGINBASE_H
#include <mostly_harmless/mostlyharmless_Descriptor.h>
#include <mostly_harmless/core/mostlyharmless_IPluginEntry.h>
#include <mostly_harmless/core/mostlyharmless_IEngine.h>
#include <mostly_harmless/core/mostlyharmless_IEditor.h>
#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
#include <mostly_harmless/utils/mostlyharmless_Timer.h>
#include <clap/helpers/plugin.hh>

namespace mostly_harmless::core {
    /**
     * Called by PluginBase to create the user's IPluginEntry subclass.
     *
     * Forward declared here, and defined by calling the MH_REGISTER_PLUGIN_ENTRY macro from within your IPluginEntry subclass' TU.
     * \return A unique_ptr to the user-defined PluginEntry derivative.
     */
    std::unique_ptr<IPluginEntry> createPluginEntry() noexcept;
} // namespace mostly_harmless::core

namespace mostly_harmless::internal {
    /**
     * \brief The internal representation of a Plugin, more akin to JUCE's Processor.
     *
     * The end user should never need to interface with this class, and is purely there as an abstraction layer between the CLAP Plugin, and the user defined interfaces.
     *
     * Under the hood, essentially forwards salient calls to the appropriate functions in the user-defined interfaces.
     */
    class PluginBase final : public clap::helpers::Plugin<clap::helpers::MisbehaviourHandler::Ignore, clap::helpers::CheckingLevel::Maximal> {
    public:
        /**
         * Constructs a PluginBase instance.
         * \param host A clap-provided host pointer.
         */
        explicit PluginBase(const clap_host* host);

    private:
        bool activate(double sampleRate, std::uint32_t minFrameCount, std::uint32_t maxFrameCount) noexcept override;
        clap_process_status process(const clap_process* processContext) noexcept override;
        void paramsFlush(const clap_input_events* in, const clap_output_events* out) noexcept override;
        void handleGuiEvents(const clap_output_events_t* outputQueue) noexcept;
        void handleEvent(const clap_event_header_t* event) noexcept;

        [[nodiscard]] bool implementsParams() const noexcept override;
        [[nodiscard]] bool isValidParamId(clap_id paramId) const noexcept override;
        [[nodiscard]] std::uint32_t paramsCount() const noexcept override;
        [[nodiscard]] bool paramsInfo(std::uint32_t paramIndex, clap_param_info* info) const noexcept override;
        [[nodiscard]] bool paramsValue(clap_id id, double* value) noexcept override;

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
        [[nodiscard]] bool guiShow() noexcept override;
        [[nodiscard]] bool guiHide() noexcept override;
        void guiDestroy() noexcept override;
        [[nodiscard]] bool guiSetParent(const clap_window* window) noexcept override;
        [[nodiscard]] bool guiSetScale(double scale) noexcept override;
        [[nodiscard]] bool guiCanResize() const noexcept override;
        [[nodiscard]] bool guiAdjustSize(std::uint32_t* width, std::uint32_t* height) noexcept override;
        [[nodiscard]] bool guiSetSize(std::uint32_t width, std::uint32_t height) noexcept override;
        [[nodiscard]] bool guiGetSize(std::uint32_t* width, std::uint32_t* height) noexcept override;

        std::optional<TransportState> m_lastTransportState{};
        utils::Timer m_guiDispatchThread;
        std::unique_ptr<core::IPluginEntry> m_pluginEntry{ nullptr };
        std::unique_ptr<core::ISharedState> m_state{ nullptr };
        std::unique_ptr<core::IEngine> m_engine{ nullptr };
        std::unique_ptr<core::IEditor> m_editor{ nullptr };
    };
} // namespace mostly_harmless::internal
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_PLUGINBASE_H
