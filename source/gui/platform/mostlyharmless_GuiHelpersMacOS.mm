#include <mostly_harmless/gui/platform/mostlyharmless_GuiHelpersMacOS.h>
#include <Foundation/Foundation.h>
#include <AppKit/AppKit.h>
#include <CoreGraphics/CoreGraphics.h>
namespace mostly_harmless::gui::helpers::macos {
    void removeFromParentView(void* viewHandle) {
        auto* asView = static_cast<NSView*>(viewHandle);
        [asView removeFromSuperview];
    }

    void setViewSize(void* viewHandle, std::uint32_t width, std::uint32_t height) {
        auto* asView = static_cast<NSView*>(viewHandle);
        asView.frame = CGRectMake(0, 0, width, height);
    }

    void getViewSize(void* viewHandle, std::uint32_t* width, std::uint32_t* height) {
        auto* asView = static_cast<NSView*>(viewHandle);
        auto bounds = [asView bounds];
        *width = static_cast<std::uint32_t>(bounds.size.width);
        *height = static_cast<std::uint32_t>(bounds.size.height);
    }

    void reparentView(void* parentViewHandle, void* childViewHandle, std::uint32_t backgroundColour) {
        auto* parent = static_cast<NSView*>(parentViewHandle);
        [parent setWantsLayer:true];
        const auto r = (backgroundColour >> 16) & 0xFF;
        const auto g = (backgroundColour >> 8) & 0xFF;
        const auto b = backgroundColour & 0xFF;
        auto* color = [NSColor colorWithCalibratedRed:r green:g blue:b alpha:1];
        [[parent layer] setBackgroundColor:color.CGColor];
        auto* child = static_cast<NSView*>(childViewHandle);
        child.frame = parent.bounds;
        [parent addSubview:child];
    }

    void showView(void* viewHandle) {
        auto* asView = static_cast<NSView*>(viewHandle);
        asView.hidden = false;
    }

    void hideView(void* viewHandle) {
        auto* asView = static_cast<NSView*>(viewHandle);
        asView.hidden = true;
    }
} // namespace mostly_harmless::gui::helpers::macos