//
// Created by Syl on 08/09/2024.
//

#ifndef MOSTLY_HARMLESS_COLOUR_H
#define MOSTLY_HARMLESS_COLOUR_H
#include <cstdint>
namespace mostly_harmless::gui {
    /**
     * \brief Convenience struct representing a colour.
     */
    struct Colour final {
        /**
         * Constructs a colour from an ARGB hex colour, in the format `0xAARRGGBB` - for example, fully opaque red would be `0xFFFF0000`
         * \param argb The hex code for the desired colour.
         */
        explicit Colour(std::uint32_t argb);
        /**
         * Constructs a colour from individual (0 to 255) rgb args. In this overload, alpha defaults to 255.
         * \param r_ The 0 to 255 value for red.
         * \param g_ The 0 to 255 value for green.
         * \param b_ The 0 to 255 value for blue.
         */
        Colour(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_);
        /**
         * Constructs a colour from individual (0 to 255) argb args.
         * \param a_ The 0 to 255 value for alpha (255 being fully opaque)
         * \param r_ The 0 to 255 value for red.
         * \param g_ The 0 to 255 value for green.
         * \param b_ The 0 to 255 value for blue.
         */
        Colour(std::uint8_t a_, std::uint8_t r_, std::uint8_t g_, std::uint8_t b_);
        std::uint8_t a, r, g, b;
    };
} // namespace mostly_harmless::gui
#endif // MOSTLY_HARMLESS_COLOUR_H
