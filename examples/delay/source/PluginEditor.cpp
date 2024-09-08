//
// Created by Syl on 19/08/2024.
//

#include "PluginEditor.h"
#include "Parameters.h"
#include <magic_enum.hpp>
namespace examples::delay {
    PluginEditor::PluginEditor(std::uint32_t width, std::uint32_t height) : mostly_harmless::gui::WebviewEditor(width, height, mostly_harmless::gui::Colour{ 0xFF89CC04 }) {
    }

    void PluginEditor::initialise(mostly_harmless::gui::EditorContext context) {
        mostly_harmless::gui::WebviewEditor::initialise(context);
        auto beginParamGestureCallback = [context](const choc::value::ValueView& args) -> choc::value::Value {
            if (!context.guiToProcQueue) return {};
            const auto paramId = static_cast<std::uint32_t>(args[0]["paramId"].getInt64());
            context.guiToProcQueue->tryPush({ .type = mostly_harmless::events::GuiToProcParamEvent::Type::Begin,
                                              .paramId = paramId,
                                              .value = 0.0 });
            context.requestParamFlush();
            return {};
        };

        auto paramChangeCallback = [context](const choc::value::ValueView& args) -> choc::value::Value {
            if (!context.guiToProcQueue) return {};
            const auto paramId = static_cast<std::uint32_t>(args[0]["paramId"].getInt64());
            const auto value = std::stod(args[0]["value"].toString());
            context.guiToProcQueue->tryPush({ .type = mostly_harmless::events::GuiToProcParamEvent::Type::Adjust,
                                              .paramId = paramId,
                                              .value = value });
            context.requestParamFlush();
            return {};
        };

        auto endParamGestureCallback = [context](const choc::value::ValueView& args) -> choc::value::Value {
            if (!context.guiToProcQueue) return {};
            const auto paramId = static_cast<std::uint32_t>(args[0]["paramId"].getInt64());
            context.guiToProcQueue->tryPush({ .type = mostly_harmless::events::GuiToProcParamEvent::Type::End,
                                              .paramId = paramId,
                                              .value = 0.0 });
            context.requestParamFlush();
            return {};
        };

        std::stringstream initialDataStream;
        initialDataStream << "window.params = { \n";
        for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(ParamIds::kNumParams); ++i) {
            const auto asEnum = static_cast<ParamIds>(i);
            const auto asStr = magic_enum::enum_name(asEnum);
            initialDataStream << "    " << asStr << ": " << i << ",\n";
        }
        initialDataStream << "};";
        m_internalWebview->addInitScript(initialDataStream.str());
        m_internalWebview->navigate("http://localhost:5173");
        m_internalWebview->bind("beginParamGesture", std::move(beginParamGestureCallback));
        m_internalWebview->bind("setParamValue", std::move(paramChangeCallback));
        m_internalWebview->bind("endParamGesture", std::move(endParamGestureCallback));
    }

    void PluginEditor::onParamEvent(mostly_harmless::events::ProcToGuiParamEvent event) {
        std::stringstream stream;
        stream << "event = new CustomEvent(\"param\", {\n";
        stream << "    detail: { \n";
        stream << "        paramId: " << event.paramId << ",\n";
        stream << "        value: " << event.value << "\n";
        stream << "    }\n";
        stream << "});\n";
        stream << "window.dispatchEvent(event);";
        m_internalWebview->evaluateJavascript(stream.str(), {});
    }

} // namespace examples::delay