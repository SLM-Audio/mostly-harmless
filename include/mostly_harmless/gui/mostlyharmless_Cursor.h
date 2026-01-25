//
// Created by Syl Morrison on 17/01/2026.
//

#ifndef MOSTLYHARMLESS_CURSOR_H
#define MOSTLYHARMLESS_CURSOR_H
#include <cstdint>
namespace mostly_harmless::gui::cursor {
    /**
     * Shows or hides the cursor
     * @param show Whether the cursor should be shown or hidden.
     */
    auto setCursorState(bool show) -> void;

    /**
     * Retrieves the screen x & y for the cursor, and stores the results in x and y.
     * @param x A pointer to a var to store the cursor's x position in.
     * @param y A pointer to a var to store the cursor's y position in.
     */
    auto getCursorPosition(std::uint32_t* x, std::uint32_t* y) -> void;

    /**
     * Sets the cursor's position to the specified (screen) x & y coords.
     * @param x The new x position for the cursor (in screen bounds).
     * @param y The new y position for the cursor (in screen bounds).
     */
    auto setCursorPosition(std::uint32_t x, std::uint32_t y) -> void;
} // namespace mostly_harmless::gui::cursor
#endif // MOSTLYHARMLESS_CURSOR_H
