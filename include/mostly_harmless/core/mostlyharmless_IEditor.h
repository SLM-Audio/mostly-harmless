//
// Created by Syl Morrison on 20/10/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_IEDITOR_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_IEDITOR_H
#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
#include <mostly_harmless/mostlyharmless_Concepts.h>
namespace mostly_harmless::core {
    /**
     * \brief Interface for custom gui editors to implement.
     *
     * To use a framework of your choosing, you'll need to subclass this interface, and implement all its methods.
     */
    struct IEditor {
        /**
         * Virtual destructor.
         */
        virtual ~IEditor() noexcept = default;

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
    };

} // namespace mostly_harmless::core

#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_IEDITOR_H
