#include "choc/gui/choc_DesktopWindow.h"
#include "choc/gui/choc_WebView.h"
#include <gui/mostlyharmless_PluginEditor.h>
#include <memory>
#include <windef.h>
#include <winuser.h>
namespace mostly_harmless::gui {

    PluginEditor::PluginEditor() {
        choc::ui::setWindowsDPIAwareness();
    }

    PluginEditor::~PluginEditor() noexcept {
    }

    void PluginEditor::create() noexcept {
        // m_webView = std::make_unique<choc::ui::WebView>();
        m_window = std::make_unique<choc::ui::DesktopWindow>(choc::ui::Bounds{ 0, 0, 800, 800 });
        m_window->setMinimumSize(800, 800);
        m_window->setMaximumSize(800, 800);
        m_window->setBounds({ 0, 0, 800, 800 });
        m_window->setWindowTitle("Test");
        windowCreated();
    }

    void PluginEditor::destroy() {
        m_window.reset();
        windowDestroyed();
    }

    void PluginEditor::setSize(std::uint32_t width, std::uint32_t height) {
        m_window->setBounds({ 0, 0, static_cast<int>(width), static_cast<int>(height) });
    }

    void PluginEditor::getSize(std::uint32_t* width, std::uint32_t* height) {
        auto* handle = m_window->getWindowHandle();
#if defined(MOSTLY_HARMLESS_WINDOWS)
        RECT rect;
        GetClientRect(static_cast<HWND>(handle), &rect);
        *width = static_cast<std::uint32_t>(rect.right - rect.left);
        *height = static_cast<std::uint32_t>(rect.bottom - rect.top);
#else
        static_assert(false);
#endif
    }

    void PluginEditor::setParent(void* parentHandle) {
        auto* windowHandle = m_window->getWindowHandle();
#if defined(MOSTLY_HARMLESS_WINDOWS)
        auto asHwnd = static_cast<HWND>(windowHandle);
        ::SetWindowLongPtrW(asHwnd, GWL_STYLE, WS_CHILD);
        ::SetParent(asHwnd, static_cast<HWND>(parentHandle));
#else
        static_assert(false);
#endif
        show();
    }

    void PluginEditor::show() {
        m_window->setVisible(true);
        windowShown();
    }

    void PluginEditor::hide() {
        m_window->setVisible(false);
        windowHidden();
    }


} // namespace mostly_harmless::gui