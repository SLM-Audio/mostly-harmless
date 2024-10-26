//
// Created by Syl Morrison on 11/08/2024.
//
#include <choc/gui/choc_DesktopWindow.h>
#include <choc/gui/choc_WebView.h>
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>
#if defined(MOSTLY_HARMLESS_MACOS)
#include <mostly_harmless/gui/platform/mostlyharmless_GuiHelpersMacOS.h>
#elif defined(MOSTLY_HARMLESS_WINDOWS)
#include <windef.h>
#include <winuser.h>
#endif
#include <cassert>
#include <filesystem>
namespace mostly_harmless::gui {

    WebviewEditor::WebviewEditor(core::ISharedState* sharedState, std::uint32_t initialWidth, std::uint32_t initialHeight, Colour backgroundColour) : WebviewBase(initialWidth,
                                                                                                                                                                  initialHeight,
                                                                                                                                                                  backgroundColour),
                                                                                                                                                      m_sharedState(sharedState) {
    }

    void WebviewEditor::initialise() {
        WebviewBase::initialise();
        auto beginParamGestureCallback_ = [this](const choc::value::ValueView& args) -> choc::value::Value {
            return beginParamChangeGestureCallback(args);
        };

        auto paramChangeCallback_ = [this](const choc::value::ValueView& args) -> choc::value::Value {
            return paramChangeGestureCallback(args);
        };

        auto endParamGestureCallback_ = [this](const choc::value::ValueView& args) -> choc::value::Value {
            return endParamChangeGestureCallback(args);
        };
        m_internalWebview->bind("beginParamGesture", std::move(beginParamGestureCallback_));
        m_internalWebview->bind("setParamValue", std::move(paramChangeCallback_));
        m_internalWebview->bind("endParamGesture", std::move(endParamGestureCallback_));
    }

    void WebviewEditor::onParamEvent(events::ProcToGuiParamEvent event) {
        sendEvent(events::WebEvent{ event });
    }

    void WebviewEditor::sendEvent(events::WebEvent&& event) noexcept {
        std::stringstream stream;
        stream << "event = new CustomEvent(\"" << event.id << "\", {\n detail:  \n";
        stream << event.content << "\n";
        stream << "});\n";
        stream << "window.dispatchEvent(event);";
        m_internalWebview->evaluateJavascript(stream.str(), {});
    }

    choc::value::Value WebviewEditor::beginParamChangeGestureCallback(const choc::value::ValueView& args) {
        try {
            auto& guiToProcQueue = m_sharedState->getGuiToProcQueue();
            const auto paramId = static_cast<std::uint32_t>(args[0]["paramId"].getInt64());
            guiToProcQueue.tryPush({ .type = events::GuiToProcParamEvent::Type::Begin,
                                     .paramId = paramId,
                                     .value = 0.0 });
            m_sharedState->requestParamFlush();
        } catch (std::exception& e) {
            assert(false);
            return choc::value::Value{ e.what() };
        }
        return {};
    }

    choc::value::Value WebviewEditor::paramChangeGestureCallback(const choc::value::ValueView& args) {
        try {
            auto& guiToProcQueue = m_sharedState->getGuiToProcQueue();
            const auto paramId = static_cast<std::uint32_t>(args[0]["paramId"].getInt64());
            const auto value = args[0]["value"].get<float>();
            guiToProcQueue.tryPush({ .type = events::GuiToProcParamEvent::Type::Adjust,
                                     .paramId = paramId,
                                     .value = value });
            m_sharedState->requestParamFlush();
        } catch (std::exception& e) {
            assert(false);
            return choc::value::Value{ e.what() };
        }
        return {};
    }

    choc::value::Value WebviewEditor::endParamChangeGestureCallback(const choc::value::ValueView& args) {
        try {
            auto& guiToProcQueue = m_sharedState->getGuiToProcQueue();
            const auto paramId = static_cast<std::uint32_t>(args[0]["paramId"].getInt64());
            guiToProcQueue.tryPush({ .type = events::GuiToProcParamEvent::Type::End,
                                     .paramId = paramId,
                                     .value = 0.0 });
            m_sharedState->requestParamFlush();
        } catch (std::exception& e) {
            std::cout << e.what();
            assert(false);
        }
        return {};
    }


} // namespace mostly_harmless::gui