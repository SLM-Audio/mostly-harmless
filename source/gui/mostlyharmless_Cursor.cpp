//
// Created by Syl Morrison on 17/01/2026.
//

#if defined(MOSTLY_HARMLESS_WINDOWS)
#include <Windows.h>
#elif defined(MOSTLY_HARMLESS_MACOS)
#include <mostly_harmless/gui/platform/mostlyharmless_GuiHelpersMacOS.h>
#endif
#include <mostly_harmless/gui/mostlyharmless_Cursor.h>
namespace mostly_harmless::gui::cursor {
#if defined(MOSTLY_HARMLESS_WINDOWS)
    auto setCursorState(bool show) -> void {
        ::ShowCursor(show ? SW_SHOW : SW_HIDE);
    }

    auto getCursorPosition(std::uint32_t* x, std::uint32_t* y) -> void {
        ::POINT cursorPos;
        ::GetCursorPos(&cursorPos);
        *x = static_cast<std::uint32_t>(cursorPos.x);
        *y = static_cast<std::uint32_t>(cursorPos.y);
    }

    auto setCursorPosition(std::uint32_t x, std::uint32_t y) -> void {
        ::SetCursorPos(static_cast<::LONG>(x), static_cast<::LONG>(y));
    }

    auto centreCursorInScreen() -> void {
        const auto desktopWindow = ::GetDesktopWindow();
        ::RECT rect;
        ::GetClientRect(desktopWindow, &rect);
        const auto x = rect.right / 2;
        const auto y = rect.bottom / 2;
        ::SetCursorPos(x, y);
    }

    auto setCursorStyle(Style style) -> void {
        const auto flag = [style]() -> LPSTR {
            switch (style) {
                case Style::Edit: return IDC_IBEAM;
                case Style::Wait: return IDC_WAIT;
                case Style::Crosshair: return IDC_CROSS;
                case Style::UpArrow: return IDC_UPARROW;
                case Style::VSize: return IDC_SIZENS;
                case Style::HSize: return IDC_SIZEWE;
                case Style::Normal: [[fallthrough]];
                default: return IDC_ARROW;
            }
        }();
        ::SetCursor(::LoadCursor(nullptr, flag));
    }


#elif defined(MOSTLY_HARMLESS_MACOS)
    auto setCursorState(bool show) -> void {
        if (show) {
            gui::helpers::macos::showCursor();
        } else {
            gui::helpers::macos::hideCursor();
        }
    }

    auto getCursorPosition(std::uint32_t* x, std::uint32_t* y) -> void {
        gui::helpers::macos::getMousePos(x, y);
    }

    auto setCursorPosition(std::uint32_t x, std::uint32_t y) -> void {
        gui::helpers::macos::setMousePos(x, y);
    }

#else
    static_assert(false);
#endif


} // namespace mostly_harmless::gui::cursor