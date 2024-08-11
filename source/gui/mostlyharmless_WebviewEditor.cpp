//
// Created by Syl Morrison on 11/08/2024.
//
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>
#if defined(MOSTLY_HARMLESS_MACOS)
#include <mostly_harmless/gui/platform/mostlyharmless_GuiHelpersMacOS.h>
#endif
#include <cassert>
namespace mostly_harmless::gui {
#if defined(MOSTLY_HARMLESS_MACOS)
    class WebviewEditor::Impl {
    public:
        Impl(std::uint32_t initialWidth, std::uint32_t initialHeight) : m_initialWidth(initialWidth),
                                                                        m_initialHeight(initialHeight) {
        }


        void create() {
            m_webview = std::make_unique<choc::ui::WebView>();
            helpers::setViewSize(m_webview->getViewHandle(), m_initialWidth, m_initialHeight);
        }

        void destroy() {
            helpers::removeFromParentView(m_webview->getViewHandle());
            m_webview = nullptr;
        }

        void getSize(std::uint32_t* width, std::uint32_t* height) {
            helpers::getViewSize(m_webview->getViewHandle(), width, height);
        }

        void setSize(std::uint32_t width, std::uint32_t height) {
            helpers::setViewSize(m_webview->getViewHandle(), width, height);
        }

        void setParent(void* parentHandle) {
            helpers::reparentView(parentHandle, m_webview->getViewHandle());
        }

        void show() {
            helpers::showView(m_webview->getViewHandle());
        }

        void hide() {
            helpers::hideView(m_webview->getViewHandle());
        }

        [[nodiscard]] choc::ui::WebView* getWebview() noexcept {
            return m_webview.get();
        }

    private:
        std::uint32_t m_initialWidth{ 0 }, m_initialHeight{ 0 };
        std::unique_ptr<choc::ui::WebView> m_webview{ nullptr };
    };
#else
    static_assert(false);
#endif

    WebviewEditor::WebviewEditor(std::uint32_t initialWidth, std::uint32_t initialHeight) {
        m_impl = std::make_unique<WebviewEditor::Impl>(initialWidth, initialHeight);
    }
    WebviewEditor::~WebviewEditor() noexcept {
    }

    void WebviewEditor::initialise() {
        m_impl->create();
        m_internalWebview = m_impl->getWebview();
    }

    void WebviewEditor::destroy() {
        m_internalWebview = nullptr;
        m_impl->destroy();
    }

    void WebviewEditor::getSize(std::uint32_t* width, std::uint32_t* height) {
        m_impl->getSize(width, height);
    }

    void WebviewEditor::setSize(std::uint32_t width, std::uint32_t height) {
        m_impl->setSize(width, height);
    }

    void WebviewEditor::setParent(void* parentHandle) {
        m_impl->setParent(parentHandle);
    }

    void WebviewEditor::show() {
        m_impl->show();
    }

    void WebviewEditor::hide() {
        m_impl->hide();
    }

} // namespace mostly_harmless::gui