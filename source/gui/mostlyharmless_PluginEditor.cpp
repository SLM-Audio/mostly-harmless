#include "choc/gui/choc_DesktopWindow.h"
#include "choc/gui/choc_WebView.h"
#include <gui/mostlyharmless_PluginEditor.h>
#include <memory>
#include <windef.h>
#include <winuser.h>
namespace mostly_harmless::gui {

    PluginEditor::PluginEditor() {
        m_dummyHwnd = ::CreateWindowA("STATIC", "dummy", WS_DISABLED, 0, 0, 100, 100, NULL, NULL, NULL, NULL);
        m_webView = std::make_unique<choc::ui::WebView>();
        m_webView->navigate("https://www.github.com/SLM-Audio/mostly-harmless");
        ::SetParent(static_cast<HWND>(m_webView->getViewHandle()), static_cast<HWND>(m_dummyHwnd));
    }

    PluginEditor::~PluginEditor() noexcept {
        ::DestroyWindow(static_cast<HWND>(m_dummyHwnd));
    }

    void PluginEditor::create() {
        m_window = std::make_unique<choc::ui::DesktopWindow>(choc::ui::Bounds{ 0, 0, 800, 800 });
        m_window->setMinimumSize(800, 800);
        m_window->setMaximumSize(800, 800);
        m_window->setWindowTitle("Test");
        setSize(800, 800);
        m_window->setContent(m_webView->getViewHandle());
    }

    void PluginEditor::destroy() {
// DON'T Destroy the webview - instead just reparent it
#if defined(MOSTLY_HARMLESS_WINDOWS)
        SetParent(static_cast<HWND>(m_webView->getViewHandle()), static_cast<HWND>(m_dummyHwnd));
#else
        static_assert(false);
#endif
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
        SetParent(static_cast<HWND>(windowHandle), static_cast<HWND>(parentHandle));
#else
        static_assert(false);
#endif
        show();
    }

    void PluginEditor::show() {
        m_window->setVisible(true);
    }

    void PluginEditor::hide() {
        m_window->setVisible(false);
    }


} // namespace mostly_harmless::gui