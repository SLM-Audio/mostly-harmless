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

    void PluginEditor::create() {
        m_webView = std::make_unique<choc::ui::WebView>();
        m_window = std::make_unique<choc::ui::DesktopWindow>(choc::ui::Bounds{ 0, 0, 800, 800 });
        m_window->setMinimumSize(800, 800);
        m_window->setMaximumSize(800, 800);
        m_window->setBounds({ 0, 0, 800, 800 });
        m_window->setWindowTitle("Test");
        m_webView->navigate("https://www.github.com/SLM-Audio/mostly-harmless");
        // When this is called, it's the *only* place the child HWND's visibility gets set -
        // So if we start with the parent invisible, the child will always be invisible
        // To remedy this, we can set the vis to SW_SHOW in show(), and SW_HIDE in hide().
        // What a nightmare...
        m_window->setContent(m_webView->getViewHandle());
    }

    void PluginEditor::destroy() {
        m_window.reset();
        m_webView.reset();
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
#if defined(MOSTLY_HARMLESS_WINDOWS)
        ShowWindow(static_cast<HWND>(m_webView->getViewHandle()), SW_SHOW);
#endif
    }

    void PluginEditor::hide() {
        m_window->setVisible(false);
#if defined(MOSTLY_HARMLESS_WINDOWS)
        ShowWindow(static_cast<HWND>(m_webView->getViewHandle()), SW_HIDE);
#endif
    }


} // namespace mostly_harmless::gui