#ifndef MOSTLYHARMLESS_IEDITOR_H
#define MOSTLYHARMLESS_IEDITOR_H
#include <mostly_harmless/events/mostlyharmless_ParamEvent.h>
#include <marvin/containers/marvin_FIFO.h>
#include <cstdint>
namespace mostly_harmless::gui {
    /**
     * \brief Interface for custom gui editors to implement.
     *
     * To use a framework of your choosing, you'll need to subclass this interface, and implement all its methods.
     * Provides a pointer to a rt/thread safe fifo for messaging between the message thread and the audio thread.
     */
    struct IEditor {
        /**
         * Virtual destructor.
         */
        virtual ~IEditor() noexcept = default;
        /**
         * Called immediately after the gui is created, any setup should be done here.
         */
        virtual void initialise() = 0;
        /**
         * Called when the editor is closed.
         */
        virtual void destroy() = 0;
        /**
         * Called when the size of the parent window has been changed.
         * \param width The new parent width.
         * \param height The new parent height.
         */
        virtual void setSize(std::uint32_t width, std::uint32_t height) = 0;
        /**
         * Queries the editor about the size of its window. Initially, the parent window's size depends entirely on this call.
         * \param width A pointer to the width, to set.
         * \param height A pointer to the height, to set.
         */
        virtual void getSize(std::uint32_t* width, std::uint32_t* height) = 0;
        /**
         *  Use this function to add your custom window to the parent view.
         *  \param parentHandle A void* to the parent window's handle - the actual type is platform specific (HWND on Windows, NSView on macOS).
         */
        virtual void setParent(void* parentHandle) = 0;
        /**
         * Called when the editor should show.
         */
        virtual void show() = 0;
        /**
         * Called when the editor should hide.
         */
        virtual void hide() = 0;

        /**
         * Called via the internal gui dispatch thread when a new parameter event is sent by the host - use this hook to sync the changes with the gui!
         */
        virtual void onParamEvent(events::ProcToGuiParamEvent event) = 0;

        /// \private
        void setGuiToProcQueue(marvin::containers::fifos::SPSC<events::GuiToProcParamEvent>* queue);
    protected:
        /**
         * A pointer to a realtime-safe, thread-safe fifo, for pushing param events from the gui back to the audio thread.
         * This will be assigned *before* initialise() is called.
         */
        marvin::containers::fifos::SPSC<events::GuiToProcParamEvent>* m_guiToProcQueue{ nullptr };
    };
} // namespace mostly_harmless::gui

#endif