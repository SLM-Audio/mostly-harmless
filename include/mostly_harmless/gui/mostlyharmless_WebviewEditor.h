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
     * To use it, you'll probably want to still subclass it, but the boilerplate will be fairly minimal. See WebviewBase for more fine grained details.
     *
     * The default implementation establishes bindings to 3 javascript functions, pertaining to parameter updates. These are:
     * `beginParamGesture()`, `setParamValue()`, and `endParamGesture()`. \n
     * Each of these functions take an object as an arg, expected to be formatted as json containing the paramId to affect, and the value to set.
     * For example:
     *
     * ```js
     * const args = {
     *     paramId: 0,
     *     value: 0.8
     * };
     * ```
     *
     * beginParamGesture is expected to be called when a slider first begins being dragged,
     * setParamValue while it is being changed, and endParamValue once the user stops dragging.
     * These functions are bound to internal native functions (which you can override if you like), beginParamChangeGestureCallback(),
     * paramChangeGestureCallback(), and endParamChangeGestureCallback(). These functions are actually promises, which we don't really leverage here, aside from to report errors in the case of an arg-parsing failure.
     * The default implementation will attempt to parse the args (and assert fail if it failed),
     * and then enqueue the param changes to the guiToProcQueue, for the host and audio side to pick up.
     *
     *
     * The structure of an event in the default implementation is:
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
     *
     *
     */
    class WebviewEditor : public WebviewBase {
    public:
        /**
         * \param initialWidth The initial width for the webview.
         * \param initialHeight The initial height for the webview.
         * \param backgroundColour The colour to paint the actual window beneath the webview.
         */
        WebviewEditor(core::ISharedState* sharedState, std::uint32_t initialWidth, std::uint32_t initialHeight, Colour backgroundColour);

        /**
         * Default destructor.
         */
        ~WebviewEditor() noexcept override = default;

        void initialise() override;

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
         * You're also totally free to overload this if you don't like the default impl - see events::WebEvent for details about the WebEvent structure.
         * \param event An rvalue ref to the event to dispatch - you can create arbitrary custom types of event.
         */
        virtual void sendEvent(events::WebEvent&& event) noexcept;


    protected:
        /**
         * Called internally by any javascript side calls to `beginParamGesture()`. Informs the host/audio thread that a change gesture has started.
         * \param context The editor context.
         * \param args The args provided by the frontend.
         * \return An empty value if everything went well, the exception message if parsing the args failed.
         */
        virtual choc::value::Value beginParamChangeGestureCallback(const choc::value::ValueView& args);

        /**
         * Called internally by any javascript side calls to 'setParamValue()`. Informs the host/audio thread that an adjustment was made, as part of a change gesture.
         * \param context The editor context.
         * \param args The args provided by the frontend.
         * \return An empty value if everything went well, the exception message if parsing the args failed.
         */
        virtual choc::value::Value paramChangeGestureCallback(const choc::value::ValueView& args);

        /**
         * Called internally by any javascript side calls to 'endParamGesture()`. Informs the host/audio thread that a change gesture has ended.
         * \param context The editor context.
         * \param args The args provided by the frontend.
         * \return An empty value if everything went well, the exception message if parsing the args failed.
         */
        virtual choc::value::Value endParamChangeGestureCallback(const choc::value::ValueView& args);

        core::ISharedState* m_sharedState{ nullptr };
    };
} // namespace mostly_harmless::gui
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWEDITOR_H
