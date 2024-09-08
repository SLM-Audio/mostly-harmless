//
// Created by Syl on 08/09/2024.
//
#include <mostly_harmless/gui/mostlyharmless_Colour.h>
namespace mostly_harmless::gui {
    Colour::Colour(std::uint32_t argb) {
        a = static_cast<std::uint8_t>((argb >> 24) & 0xFF);
        r = static_cast<std::uint8_t>((argb >> 16) & 0xFF);
        g = static_cast<std::uint8_t>((argb >> 8) & 0xFF);
        b = static_cast<std::uint8_t>(argb & 0xFF);
    }

    Colour::Colour(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_) : a(static_cast<std::uint8_t>(255)), r(r_), g(g_), b(b_) {
    }

    Colour::Colour(std::uint8_t a_, std::uint8_t r_, std::uint8_t g_, std::uint8_t b_) : a(a_), r(r_), g(g_), b(b_) {
    }
} // namespace mostly_harmless::gui
