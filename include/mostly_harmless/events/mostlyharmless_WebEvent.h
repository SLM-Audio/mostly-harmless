//
// Created by Syl Morrison on 20/09/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_WEBEVENT_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_WEBEVENT_H
#include <nlohmann/json.hpp>
namespace mostly_harmless::events {
    struct ProcToGuiParamEvent;
    /**
     * \brief An convenience wrapper around an event to send to a webview.
     */
    struct WebEvent {
        /**
         * Constructs a WebEvent from an id, and some json.
         * \param id_ Some name the event should have - useful for js CustomEvents, etc.
         * \param content_ The json content for the event.
         */
        WebEvent(std::string id_, nlohmann::json content_);
        /**
         * Constructs a WebEvent from a ProcToGuiParamEvent.
         * Sets id to param, and content to:
         *
         * ```
         * {
         *     "paramId": The event's pid,
         *     "value" The event's value
         * }
         * ```
         *
         * \param event a ProcToGuiParamEvent, to convert to a WebEvent.
         */
        explicit WebEvent(ProcToGuiParamEvent event);

        /**
         * The event's id
         */
        std::string id;

        /**
         * The event's json content.
         */
        nlohmann::json content;
    };
} // namespace mostly_harmless::events
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_WEBEVENT_H
