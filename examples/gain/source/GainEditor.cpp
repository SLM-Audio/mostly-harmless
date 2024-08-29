//
// Created by Syl Morrison on 11/08/2024.
//

#include "GainEditor.h"
#include <fmt/core.h>
#include <iostream>
#include <sstream>
#include <magic_enum.hpp>
#include "Gain.h"
#include <mostlyharmless_WebResources.h>

namespace examples::gain {
    [[nodiscard]] Resource createResourceFor(const std::string& name) {
        auto resOpt = mostly_harmless::WebResources::getNamedResource(name);
        assert(resOpt);
        auto [data, size] = *resOpt;
        auto mimeType = mostly_harmless::gui::getMimeType(name);
        assert(mimeType);
        // Evil evil evil evil evil evil (but safe I think)
        auto* asUnsigned = reinterpret_cast<const std::uint8_t*>(data);
        Resource temp;
        temp.data = { asUnsigned, asUnsigned + size };
        temp.mimeType = *mimeType;
        return temp;
    }

    GainEditor::GainEditor(std::uint32_t width, std::uint32_t height) : mostly_harmless::gui::WebviewEditor(width, height) {
        m_resources.emplace("/index.html", createResourceFor("index.html"));
        m_resources.emplace("/index.css", createResourceFor("index.css"));
        m_resources.emplace("/index.js", createResourceFor("index.js"));
        auto fetchResourceCallback = [this](const std::string& url) -> std::optional<Resource> {
            const auto requested = url == "/" ? "/index.html" : url;
            const auto it = m_resources.find(requested);
            if (it == m_resources.end()) return {};
            auto resource = it->second;
            return resource;
        };
        // a map of paramEnum : paramId will be available in `window.params`.
        std::stringstream initialDataStream;
        initialDataStream << "window.params = { \n";
        for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(ParamIds::kNumParams); ++i) {
            const auto asEnum = static_cast<ParamIds>(i);
            const auto asStr = magic_enum::enum_name(asEnum);
            initialDataStream << "    " << asStr << ": " << i << ",\n";
        }
        initialDataStream << "};";
#if defined(GAIN_HOT_RELOAD)
        this->setOptions({ .enableDebugMode = true, .initScript = initialDataStream.str() });
#else
        this->setOptions({ .enableDebugMode = true, .fetchResource = std::move(fetchResourceCallback), .initScript = initialDataStream.str() });
#endif
    }

    void GainEditor::initialise(mostly_harmless::gui::EditorContext context) {

        mostly_harmless::gui::WebviewEditor::initialise(context);
        auto beginParamGestureCallback = [context](const choc::value::ValueView& args) -> choc::value::Value {
            if (!context.guiToProcQueue) return {};
            const auto paramId = static_cast<std::uint32_t>(args[0]["paramId"].getInt64());
            // Value doesn't matter here..
            context.guiToProcQueue->tryPush({ .type = mostly_harmless::events::GuiToProcParamEvent::Type::Begin,
                                              .paramId = paramId,
                                              .value = 0.0 });
            context.requestParamFlush();
            return {};
        };

        auto paramChangeCallback = [this, context](const choc::value::ValueView& args) -> choc::value::Value {
            if (!context.guiToProcQueue) return {};
            [[maybe_unused]] const auto paramId = static_cast<std::uint32_t>(args[0]["paramId"].getInt64());
            [[maybe_unused]] const auto value = std::stod(args[0]["value"].toString());
            context.guiToProcQueue->tryPush({ .type = mostly_harmless::events::GuiToProcParamEvent::Type::Adjust,
                                              .paramId = paramId,
                                              .value = value });
            context.requestParamFlush();
            m_internalWebview->evaluateJavascript("console.log(\"Hello world!\");");
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


//        m_internalWebview->addInitScript(initialDataStream.str());
#if defined(GAIN_HOT_RELOAD)
        m_internalWebview->navigate("http://localhost:5173");
#endif
        m_internalWebview->bind("beginParamGesture", std::move(beginParamGestureCallback));
        m_internalWebview->bind("setParamValue", std::move(paramChangeCallback));
        m_internalWebview->bind("endParamGesture", std::move(endParamGestureCallback));
    }

    void GainEditor::onParamEvent(mostly_harmless::events::ProcToGuiParamEvent event) {
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
} // namespace examples::gain