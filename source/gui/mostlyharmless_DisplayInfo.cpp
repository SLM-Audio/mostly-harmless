//
// Created by Syl Morrison on 06/02/2026.
//
#include "mostly_harmless/gui/platform/mostlyharmless_GuiHelpersMacOS.h"


#include <mostly_harmless/gui/mostlyharmless_DisplayInfo.h>
namespace mostly_harmless::gui::display_info {

#if defined(MOSTLY_HARMLESS_MACOS)
#include <mostly_harmless/gui/platform/mostlyharmless_GuiHelpersMacOS.h>
    void getScreenDimensions(std::uint32_t* width, std::uint32_t* height) {
        helpers::macos::getScreenSize(width, height);
    }

    double getDevicePixelRatio() {
        return helpers::macos::getDevicePixelRatio();
    }

#else

#endif

} // namespace mostly_harmless::gui::display_info