//
// Created by Syl Morrison on 20/09/2024.
//
#include <mostly_harmless/events/mostlyharmless_WebEvent.h>
#include <mostly_harmless/events/mostlyharmless_ParamEvent.h>
namespace mostly_harmless::events {
    WebEvent::WebEvent(std::string id_, nlohmann::json content_) : id(std::move(id_)),
                                                                   content(std::move(content_)) {
    }

    WebEvent::WebEvent(mostly_harmless::events::ProcToGuiParamEvent event) {
        id = "param";
        auto& detail = content["detail"];
        detail["paramId"] = std::to_string(event.paramId);
        detail["value"] = std::to_string(event.value);
    }
} // namespace mostly_harmless::events