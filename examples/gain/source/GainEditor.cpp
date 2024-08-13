//
// Created by Syl Morrison on 11/08/2024.
//

#include "GainEditor.h"
#include <fmt/core.h>
#include <iostream>
#include <sstream>
#include "Gain.h"

namespace examples::gain {
    GainEditor::GainEditor(std::uint32_t width, std::uint32_t height) : mostly_harmless::gui::WebviewEditor(width, height) {
    }

    void GainEditor::initialise() {
        mostly_harmless::gui::WebviewEditor::initialise();
        auto beginParamGestureCallback = [this](const choc::value::ValueView& args) -> choc::value::Value {
            if(!m_guiToProcQueue) return{};
            const auto paramId = static_cast<std::uint32_t>(args[0]["paramId"].getInt64());
            // Value doesn't matter here..
            m_guiToProcQueue->tryPush({ .type = mostly_harmless::events::GuiToProcParamEvent::Type::Begin,
                .paramId = paramId,
                .value = 0.0
            });
            return {};
        };

        auto paramChangeCallback = [this] ( const choc::value::ValueView& args) -> choc::value::Value {
            if(!m_guiToProcQueue) return{};
            [[maybe_unused]] const auto paramId = static_cast<std::uint32_t>(args[0]["paramId"].getInt64());
            [[maybe_unused]] const auto value = std::stod(args[0]["value"].toString());
                      m_guiToProcQueue->tryPush({
                          .type = mostly_harmless::events::GuiToProcParamEvent::Type::Adjust,
                          .paramId = paramId,
                          .value = value
                      });
            return choc::value::Value{ choc::json::toString(args) };
        };

        auto endParamGestureCallback = [this] (const choc::value::ValueView& args) -> choc::value::Value {
            if(!m_guiToProcQueue) return {};
            const auto paramId = static_cast<std::uint32_t>(args[0]["paramId"].getInt64());
            m_guiToProcQueue->tryPush({
                .type = mostly_harmless::events::GuiToProcParamEvent::Type::End,
                .paramId = paramId,
                .value = 0.0
            });
            return {};
        };

        m_internalWebview->navigate("http://localhost:5173");
        m_internalWebview->bind("beginParamGesture", std::move(beginParamGestureCallback));
        m_internalWebview->bind("setParamValue", std::move(paramChangeCallback));
        m_internalWebview->bind("endParamGesture", std::move(endParamGestureCallback));
    }

    void GainEditor::onParamEvent(mostly_harmless::events::ProcToGuiParamEvent event) {
        std::stringstream stream;
        stream << "event = new CustomEvent(\"param-event\", {\n";
        stream << "    detail: { \n";
        stream << "        paramId: " << event.paramId << ",\n";
        stream << "        value: " << event.value << "\n";
        stream << "    }\n";
        stream << "});\n";
        stream << "window.dispatchEvent(event);";
        m_internalWebview->evaluateJavascript(stream.str(), {});
    }
} // namespace examples::gain