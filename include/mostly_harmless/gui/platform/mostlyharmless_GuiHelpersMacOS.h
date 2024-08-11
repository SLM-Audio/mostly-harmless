//
// Created by Syl Morrison on 11/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_GUIHELPERSMACOS_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_GUIHELPERSMACOS_H
#include <cstdint>
namespace mostly_harmless::gui::helpers {
    void removeFromParentView(void* viewHandle);
    void setViewSize(void* viewHandle, std::uint32_t width, std::uint32_t height);
    void getViewSize(void* viewHandle, std::uint32_t* width, std::uint32_t* height);
    void reparentView(void* parentViewHandle, void* childViewHandle);
    void showView(void* viewHandle);
    void hideView(void* viewHandle);
} // namespace mostly_harmless::gui::helpers
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_GUIHELPERSMACOS_H
