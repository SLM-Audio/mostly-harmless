//
// Created by Syl Morrison on 20/10/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_IEDITOR_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_IEDITOR_H
#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
#include <mostly_harmless/mostlyharmless_Concepts.h>
namespace mostly_harmless::core {
    /**
     * \brief Interface for the plugin's gui functionality.
     *
     * As detailed in the FX Plugin tutorial, users can optionally implement this (or descendents), and must implement ISharedState, IEngine and IPluginEntry, and register their subclasses with
     * the framework to create an audio plugin. These will also be created if using the provided `create.py` script.
     *
     * This interface serves as the the barebones gui hooks for implementing a framework of your choosing with the framework, and doesn't provide any functionality out of the box.
     * If you'd like to use a webview gui, take a look at WebviewEditor or WebviewBase instead.
     */
    struct IEditor {
        /**
         * Virtual destructor.
         */
        virtual ~IEditor() noexcept = default;

        /**
         * Called immediately after the gui has been created - use this to perform any post-creation initialisation.
         */
        virtual void initialise() = 0;

        /**
         * Called when the editor is closed.
         */
        virtual void destroy() = 0;

        /**
         * Return true here if you want your gui to be resizable, otherwise return false.
         * \return Whether or not the gui is allowed to be resized.
         */
        [[nodiscard]] virtual bool allowResize() const noexcept = 0;

        /**
         * Called when the size of the parent window has been changed.
         * \param width The new parent width.
         * \param height The new parent height.
         */
        virtual void setSize(std::uint32_t width, std::uint32_t height) = 0;

        /**
         * Called when the host tries to adjust the editor's size, and allows the user to add extra constraints to the requested bounds.
         * Calls to this will be followed by a subsequent call to setSize, using the values stored in the newWidth and newHeight parameters.
         * \param newWidth A pointer containing the new requested width. Can be mutated by your implementation, to clamp the size to a certain max or aspect ratio, for example.
         * \param newHeight A pointer containing the new requested height. Can be mutated by your implementation, to clamp the size to a certain max or aspect ratio, for example.
         */
        virtual void onResizeRequested(std::uint32_t* newWidth, std::uint32_t* newHeight) = 0;

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
