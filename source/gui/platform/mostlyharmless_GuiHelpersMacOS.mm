#include <mostly_harmless/gui/platform/mostlyharmless_GuiHelpersMacOS.h>
#include <Foundation/Foundation.h>
#include <AppKit/AppKit.h>
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

    void reparentView(void* parentViewHandle, void* childViewHandle) {
        auto* parent = static_cast<NSView*>(parentViewHandle);
        auto* child = static_cast<NSView*>(childViewHandle);
        child.frame = parent.bounds;
        [parent addSubview:child];
        [[maybe_unused]] const auto converted = [parent.window.contentView convertRect:child.frame fromView:child];
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