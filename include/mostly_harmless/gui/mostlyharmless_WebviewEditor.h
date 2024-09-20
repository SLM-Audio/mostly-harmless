//
// Created by Syl Morrison on 11/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWEDITOR_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWEDITOR_H
#include <mostly_harmless/gui/mostlyharmless_WebviewBase.h>
#include <mostly_harmless/events/mostlyharmless_WebEvent.h>
#include <marvin/library/marvin_Concepts.h>
#include <choc/gui/choc_WebView.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <memory>
namespace mostly_harmless::gui {

    /**
     * \brief An opinionated subclass of WebviewBase, providing default impls for bidirectional comms.
     *
     * Where WebviewBase handles lower level things like window creation / management, this class handles slightly higher level
     * details - namely, an event system, and param updates.
     *
     * To use it, you'll probably want to still subclass it, but the boilerplate will be fairly minimal. See WebviewBase for more fine grained details,
     * and onParamEvent() and sendEvent() for specifics on how the data is organised.
     */
    class WebviewEditor : public WebviewBase {
    public:
        /**
         * \param initialWidth The initial width for the webview.
         * \param initialHeight The initial height for the webview.
         * \param backgroundColour The colour to paint the actual window beneath the webview.
         */
        WebviewEditor(std::uint32_t initialWidth, std::uint32_t initialHeight, Colour backgroundColour);
        ~WebviewEditor() noexcept override = default;
        /**
         * Called when the webview is created, immediately after construction.
         * Our default implementation here establishes bindings for 3 javascript functions - beginParamGesture(), setParamValue(), and endParamGesture().
         * You should call these in your javascript code at the appropriate times to send param updates to the backend.
         * These events are expected to take
         * `
         * \param context The editor context (see IEditor::initialise() and EditorContext for more details).
         */
        void initialise(EditorContext context) override;
        /**
         * Called when the host sends a param update, to inform the gui that a change has occurred.\n
         * Actually gets invoked from a timer thread, on the message thread. \n
         * This is still virtual, and can be overridden as you like, but the default implementation will call sendEvent() with a WebEvent constructed from the
         * event passed from the param queue - see `sendEvent()` for more details.
         * \param event An event specifying
         */
        void onParamEvent(events::ProcToGuiParamEvent event) override;

        /**
         * Sends a javascript event to the internal webview.\n
         * The structure of the event in the default implementation is:
         *
         * ```js
         * event = new CustomEvent("the event's id", {
         *     detail: {
         *          the event's content
         *     }
         * });
         * window.dispatchEvent(event);
         * ```
         *
         * So to respond to this, you can register an event listener:
         *
         * ```js
         * let callback = (ev) => {
         *      // your code here, details are accessible through ev.detail
         * };
         * addEventListener("the event's id", callback);
         * // to unsubscribe (do this for cleanup!)
         * removeEventListener("the event's id", callback);
         * ```
         *
         * This function is called by the default implementation of onParamEvent(). The event is structured as:
         *
         * ```
         * CustomEvent("param", {
         *     detail: {
         *         "paramId": "the param id",
         *         "value": the param's value
         *     }
         * });
         * ```
         *
         * for example,
         *
         * ```js
         * let callback = (ev) => {
         *     const paramId = ev.detail.paramId;
         *     const value = parseFloat(ev.detail.value);
         *     // .... do something with this data....
         * };
         * addEventListener("param", callback);
         * ```
         *
         * You're also totally free to overload this if you don't like the default impl - see events::WebEvent for details about the WebEvent structure.
         * \param event An rvalue ref to the event to dispatch - you can create arbitrary custom types of event.
         */
        virtual void sendEvent(events::WebEvent&& event) noexcept;


    protected:
        virtual choc::value::Value beginParamChangeGestureCallback(EditorContext context, const choc::value::ValueView& args);
        virtual choc::value::Value paramChangeGestureCallback(EditorContext context, const choc::value::ValueView& args);
        virtual choc::value::Value endParamChangeGestureCallback(EditorContext context, const choc::value::ValueView& args);
    };
} // namespace mostly_harmless::gui
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWEDITOR_H
