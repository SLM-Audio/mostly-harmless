//
// Created by Syl Morrison on 11/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_GUIHELPERSMACOS_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_GUIHELPERSMACOS_H
#include <cstdint>
namespace mostly_harmless::gui::helpers::macos {
    /**
     * Removes an NSView from its parent NSView.
     * \param viewHandle A void* to the NSView to remove.
     */
    void removeFromParentView(void* viewHandle);
    /**
     * Sets an NSView's size.
     * \param viewHandle A void* to the NSView* to resize.
     * \param width The new width.
     * \param height The new height.
     */
    void setViewSize(void* viewHandle, std::uint32_t width, std::uint32_t height);
    /**
     * Retrieves an NSView's size.
     * \param viewHandle A void* to the NSView to query.
     * \param width A pointer to the width variable - the result will be written here.
     * \param height A pointer to the height variable - the result will be written here.
     */
    void getViewSize(void* viewHandle, std::uint32_t* width, std::uint32_t* height);
    /**
     * Adds an NSView as a subview of another NSView.
     * \param parentViewHandle A void* to the NSView to add the child view to.
     * \param childViewHandle A void* to the NSView to add to the parent view.
     */
    void reparentView(void* parentViewHandle, void* childViewHandle, std::uint32_t backgroundColour);
    /**
     * Sets an NSView as visible.
     * \param viewHandle A void* to the NSView to set visible.
     */
    void showView(void* viewHandle);
    /**
     * Sets an NSView as hidden.
     * \param viewHandle A void* to the NSView to set hidden.
     */
    void hideView(void* viewHandle);
} // namespace mostly_harmless::gui::helpers
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_GUIHELPERSMACOS_H
