//
// Created by Syl Morrison on 06/02/2026.
//

#ifndef GLEO_MOSTLYHARMLESS_DISPLAYINFO_H
#define GLEO_MOSTLYHARMLESS_DISPLAYINFO_H
#include <cstdint>
namespace mostly_harmless::gui::display_info {
    void getScreenDimensions(std::uint32_t* width, std::uint32_t* height);
    double getDevicePixelRatio();
} // namespace mostly_harmless::gui::display_info
#endif // GLEO_MOSTLYHARMLESS_DISPLAYINFO_H
