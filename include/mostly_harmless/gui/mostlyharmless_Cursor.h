//
// Created by Syl Morrison on 17/01/2026.
//

#ifndef MOSTLYHARMLESS_CURSOR_H
#define MOSTLYHARMLESS_CURSOR_H
#include <cstdint>
namespace mostly_harmless::gui::cursor {
    auto setCursorState(bool show) -> void;
    auto getCursorPosition(std::uint32_t* x, std::uint32_t* y) -> void;
    auto setCursorPosition(std::uint32_t x, std::uint32_t y) -> void;
} // namespace mostly_harmless::gui::cursor
#endif // MOSTLYHARMLESS_CURSOR_H
