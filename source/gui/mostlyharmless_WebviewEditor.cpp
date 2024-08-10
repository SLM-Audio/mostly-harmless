#include <gui/mostlyharmless_WebviewEditor.h>
namespace mostly_harmless::gui {
    void WebviewEditor::windowCreated() noexcept {
        m_webview = std::make_unique<choc::ui::WebView>();
        // When this is called, it's the *only* place the child HWND's visibility gets set -
        // So if we start with the parent invisible, the child will always be invisible
        // To remedy this, we can set the vis to SW_SHOW in show(), and SW_HIDE in hide().
        // What a nightmare...
        m_window->setContent(m_webview->getViewHandle());
        m_webview->navigate("https://github.com/SLM-Audio/mostly-harmless.git");
    }

    void WebviewEditor::windowDestroyed() noexcept {
        m_webview.reset();
    }

    void WebviewEditor::windowShown() noexcept {
#if defined(MOSTLY_HARMLESS_WINDOWS)
        ShowWindow(static_cast<HWND>(m_webview->getViewHandle()), SW_SHOW);
#endif
    }

    void WebviewEditor::windowHidden() noexcept {
#if defined(MOSTLY_HARMLESS_WINDOWS)
        ShowWindow(static_cast<HWND>(m_webview->getViewHandle()), SW_HIDE);
#endif
    }


} // namespace mostly_harmless::gui