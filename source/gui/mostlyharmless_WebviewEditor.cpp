//
// Created by Syl Morrison on 11/08/2024.
//
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>
#include "mostly_harmless/utils/mostlyharmless_OnScopeExit.h"
#include <mostly_harmless/utils/mostlyharmless_Macros.h>
#if defined(MOSTLY_HARMLESS_MACOS)
#include <mostly_harmless/gui/platform/mostlyharmless_GuiHelpersMacOS.h>
#elif defined(MOSTLY_HARMLESS_WINDOWS)
#include <windef.h>
#include <winuser.h>
#endif
#include <choc/gui/choc_DesktopWindow.h>
#include <choc/gui/choc_WebView.h>
#include <cassert>
#include <filesystem>
#include <mostly_harmless/gui/mostlyharmless_Cursor.h>
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
        auto beginScopedMouseMoveGestureCallback_ = [this](const choc::value::ValueView& args) -> choc::value::Value {
            std::uint32_t x, y;
            cursor::getCursorPosition(&x, &y);
            m_cursorState.lastMouseDownLocation = std::make_pair(x, y);
            cursor::setCursorState(false);
            return {};
        };

        auto endScopedMouseMoveGestureCallback_ = [this](const choc::value::ValueView& args) -> choc::value::Value {
            mostly_harmless::utils::OnScopeExit se{ [this]() -> void {
                cursor::setCursorState(true);
            } };
            if (!m_cursorState.lastMouseDownLocation) {
                return {};
            }
            const auto [x, y] = *m_cursorState.lastMouseDownLocation;
            cursor::setCursorPosition(x, y);
            m_cursorState.lastMouseDownLocation = {};
            return {};
        };

        auto resetMousePositonCallback_ = [this](const choc::value::ValueView& args) -> choc::value::Value {
            const auto [x, y] = m_cursorState.lastMouseDownLocation.value_or(std::make_pair(0, 0));
            mostly_harmless::gui::cursor::setCursorPosition(x, y);
            m_cursorState.lastMousePosition = std::make_pair(x, y);
            return {};
        };

        auto tickMouseMoveCallback_ = [this](const choc::value::ValueView& args) -> choc::value::Value {
            const auto lastMouseDownLocation = m_cursorState.lastMouseDownLocation.value_or(std::make_pair(0, 0));
            const auto [prevX, prevY] = m_cursorState.lastMousePosition.value_or(lastMouseDownLocation);
            std::uint32_t x, y;
            mostly_harmless::gui::cursor::getCursorPosition(&x, &y);
            m_cursorState.lastMousePosition = std::make_pair(x, y);
            const auto delta = static_cast<std::int32_t>(y) - static_cast<std::int32_t>(prevY);
            return choc::value::Value{ delta };
        };

        auto clearPreviousMousePositionCallback_ = [this](const choc::value::ValueView& args) -> choc::value::Value {
            m_cursorState.lastMousePosition = {};
            return {};
        };

        m_internalWebview->bind("beginParamGesture", std::move(beginParamGestureCallback_));
        m_internalWebview->bind("setParamValue", std::move(paramChangeCallback_));
        m_internalWebview->bind("endParamGesture", std::move(endParamGestureCallback_));
        m_internalWebview->bind("beginScopedMouseMoveGesture", std::move(beginScopedMouseMoveGestureCallback_));
        m_internalWebview->bind("endScopedMouseMoveGesture", std::move(endScopedMouseMoveGestureCallback_));
        m_internalWebview->bind("resetMousePosition", std::move(resetMousePositonCallback_));
        m_internalWebview->bind("tickMouseMove", std::move(tickMouseMoveCallback_));
        m_internalWebview->bind("clearPreviousMousePosition", std::move(clearPreviousMousePositionCallback_));
    }

    bool WebviewEditor::allowResize() const noexcept {
        return true;
    }

    void WebviewEditor::onResizeRequested(std::uint32_t* /*newWidth*/, std::uint32_t* /*newHeight*/) {
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