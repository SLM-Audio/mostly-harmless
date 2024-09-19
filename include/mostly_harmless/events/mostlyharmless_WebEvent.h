//
// Created by Syl Morrison on 20/09/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_WEBEVENT_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_WEBEVENT_H
#include <nlohmann/json.hpp>
namespace mostly_harmless::events {
    struct ProcToGuiParamEvent;
    struct WebEvent {
        WebEvent(std::string id_, nlohmann::json content_);
        explicit WebEvent(ProcToGuiParamEvent event);
        std::string id;
        nlohmann::json content;
    };
} // namespace mostly_harmless::events
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_WEBEVENT_H
