#include <mostly_harmless/gui/platform/mostlyharmless_GuiHelpersMacOS.h>
#include <Foundation/Foundation.h>
#include <AppKit/AppKit.h>
#include <CoreGraphics/CoreGraphics.h>
#include <mostly_harmless/utils/mostlyharmless_Macros.h>
namespace mostly_harmless::gui::helpers::macos {
    void* createView(std::uint32_t width, std::uint32_t height) {
        const auto fwidth = static_cast<CGFloat>(width);
        const auto fheight = static_cast<CGFloat>(height);
        auto rect = NSMakeRect(0.0, 0.0, fwidth, fheight);
        auto* view = [NSView alloc];
        [view initWithFrame:rect];
        return view;
    }

    void removeFromParentView(void* viewHandle) {
        MH_LOG("GUI: Removing from parent view");
        auto* asView = static_cast<NSView*>(viewHandle);
        [asView removeFromSuperview];
    }

    void setViewSize(void* viewHandle, std::uint32_t width, std::uint32_t height) {
        auto* asView = static_cast<NSView*>(viewHandle);
        CGSize newSize{ static_cast<CGFloat>(width), static_cast<CGFloat>(height) };
        [asView setFrameSize:newSize];
    }

    void getViewSize(void* viewHandle, std::uint32_t* width, std::uint32_t* height) {
        auto* asView = static_cast<NSView*>(viewHandle);
        auto bounds = [asView bounds];
        *width = static_cast<std::uint32_t>(bounds.size.width);
        *height = static_cast<std::uint32_t>(bounds.size.height);
    }

    void reparentView(void* hostViewHandle, void* clientViewHandle, void* childViewHandle, Colour backgroundColour) {
        auto* host = static_cast<NSView*>(hostViewHandle);
        auto* client = static_cast<NSView*>(clientViewHandle);
        auto* child = static_cast<NSView*>(childViewHandle);
        [client setWantsLayer:true];

        [[maybe_unused]] const auto [a, r, g, b] = backgroundColour;
        CGFloat f32R = static_cast<CGFloat>(r) / 255.0f;
        CGFloat f32G = static_cast<CGFloat>(g) / 255.0f;
        CGFloat f32B = static_cast<CGFloat>(b) / 255.0f;
        auto* color = [NSColor colorWithCalibratedRed:f32R green:f32G blue:f32B alpha:1];
        [[client layer] setBackgroundColor:color.CGColor];
        host.autoresizesSubviews = YES;
        host.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
        host.translatesAutoresizingMaskIntoConstraints = true;
        client.autoresizesSubviews = YES;
        client.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
        client.translatesAutoresizingMaskIntoConstraints = true;
        child.autoresizesSubviews = YES;
        child.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
        child.translatesAutoresizingMaskIntoConstraints = true;
        client.frame = host.bounds;
        child.frame = host.bounds;
        [client addSubview:child];
        [host addSubview:client];
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