//
// Created by Syl Morrison on 17/01/2026.
//

#ifndef MOSTLYHARMLESS_CURSOR_H
#define MOSTLYHARMLESS_CURSOR_H
#include <cstdint>
namespace mostly_harmless::gui::cursor {
    enum class Style {
        Normal,
        Edit,
        Wait,
        Crosshair,
        UpArrow,
        VSize,
        HSize,
        VHSize
    };

    auto setCursorState(bool show) -> void;
    auto getCursorPosition(std::uint32_t* x, std::uint32_t* y) -> void;
    auto setCursorPosition(std::uint32_t x, std::uint32_t y) -> void;
    auto centreCursorInScreen() -> void;
    auto setCursorStyle(Style style) -> void;
} // namespace mostly_harmless::gui::cursor
#endif // MOSTLYHARMLESS_CURSOR_H
