//
// Created by Syl Morrison on 20/09/2024.
//
#include <mostly_harmless/gui/mostlyharmless_WebviewBase.h>
#include <filesystem>
#if defined(MOSTLY_HARMLESS_MACOS)
#include <mostly_harmless/gui/platform/mostlyharmless_GuiHelpersMacOS.h>
#endif

namespace mostly_harmless::gui {
    static std::unordered_map<std::string, std::string> s_mimeTypes = {
        { "aac", "audio/aac" },
        { "aif", "audio/aiff" },
        { "aiff", "audio/aiff" },
        { "avif", "image/avif" },
        { "bmp", "image/bmp" },
        { "css", "text/css" },
        { "csv", "text/csv" },
        { "flac", "audio/flac" },
        { "gif", "image/gif" },
        { "htm", "text/html" },
        { "html", "text/html" },
        { "ico", "image/vnd.microsoft.icon" },
        { "jpeg", "image/jpeg" },
        { "jpg", "image/jpeg" },
        { "js", "text/javascript" },
        { "json", "application/json" },
        { "md", "text/markdown" },
        { "mid", "audio/midi" },
        { "midi", "audio/midi" },
        { "mjs", "text/javascript" },
        { "mp3", "audio/mpeg" },
        { "mp4", "video/mp4" },
        { "mpeg", "video/mpeg" },
        { "ogg", "audio/ogg" },
        { "otf", "font/otf" },
        { "pdf", "application/pdf" },
        { "png", "image/png" },
        { "rtf", "application/rtf" },
        { "svg", "image/svg+xml" },
        { "svgz", "image/svg+xml" },
        { "tif", "image/tiff" },
        { "tiff", "image/tiff" },
        { "ttf", "font/ttf" },
        { "txt", "text/plain" },
        { "wasm", "application/wasm" },
        { "wav", "audio/wav" },
        { "weba", "audio/webm" },
        { "webm", "video/webm" },
        { "webp", "image/webp" },
        { "woff", "font/woff" },
        { "woff2", "font/woff2" },
        { "xml", "application/xml" },
        { "zip", "application/zip" },
    };

    std::optional<std::string> getMimeType(const std::string& filename) {
        const auto ext = std::filesystem::path{ filename }.extension().string().substr(1);
        auto it = s_mimeTypes.find(ext);
        if (it == s_mimeTypes.end()) return {};
        return s_mimeTypes[ext];
    }

    WebviewBase::Resource::Resource(std::string_view content, std::string mimeType_) : mimeType(std::move(mimeType_)) {
        if (!content.empty()) {
            auto src = content.data();
            data.insert(data.end(), src, src + content.length());
        }
    }
#if defined(MOSTLY_HARMLESS_WINDOWS)
    class WebviewBase::Impl {
    public:
        Impl(std::uint32_t initialWidth, std::uint32_t initialHeight, Colour backgroundColour) : m_initialWidth(initialWidth),
                                                                                                 m_initialHeight(initialHeight) {
            m_brush = ::CreateSolidBrush(RGB(backgroundColour.r, backgroundColour.g, backgroundColour.b));
        }

        ~Impl() noexcept {
            ::DeleteObject(m_brush);
        }

        void setOptions(Options&& options) {
            m_options.enableDebugMode = options.enableDebug;
            m_options.initScript = options.initScript;
            if (options.contentProvider) {
                auto wrapper = [options](const std::string& toFind) -> std::optional<choc::ui::WebView::Options::Resource> {
                    auto res = options.contentProvider(toFind);
                    if (!res) return {};
                    choc::ui::WebView::Options::Resource resource;
                    resource.data = std::move(res->data);
                    resource.mimeType = std::move(res->mimeType);
                    return resource;
                };
                m_options.fetchResource = std::move(wrapper);
            };
        }

        void create() {
            const auto iWidth = static_cast<int>(m_initialWidth);
            const auto iHeight = static_cast<int>(m_initialHeight);
            m_webview = std::make_unique<choc::ui::WebView>(m_options);
            auto* hwnd = static_cast<::HWND>(m_webview->getViewHandle());
            ::SetWindowPos(hwnd, NULL, 0, 0, iWidth, iHeight, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_FRAMECHANGED);
        }

        void destroy() {
            m_webview.reset();
        }

        void getSize(std::uint32_t* width, std::uint32_t* height) {
            auto* handle = static_cast<HWND>(m_webview->getViewHandle());
            ::RECT rect;
            ::GetClientRect(handle, &rect);
            *width = static_cast<std::uint32_t>(rect.right - rect.left);
            *height = static_cast<std::uint32_t>(rect.bottom - rect.top);
        }

        void setSize(std::uint32_t width, std::uint32_t height) {
            auto* hwnd = static_cast<::HWND>(m_webview->getViewHandle());
            ::SetWindowPos(hwnd, NULL, 0, 0, static_cast<int>(width), static_cast<int>(height), SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_FRAMECHANGED);
        }


        void setParent(void* parentHandle) {
            auto* parentHwnd = static_cast<HWND>(parentHandle);
            auto* handle = static_cast<::HWND>(m_webview->getViewHandle());
            ::SetClassLongPtrW(handle, GCLP_HBRBACKGROUND, (::LONG_PTR)m_brush);
            ::InvalidateRect(handle, NULL, false);
            ::SetWindowLongPtrW(handle, GWL_STYLE, WS_CHILD);
            ::SetParent(handle, parentHwnd);
            show();
        }

        void show() {
            auto* webviewHwnd = static_cast<HWND>(m_webview->getViewHandle());
            ::ShowWindow(webviewHwnd, SW_SHOW);
        }

        void hide() {
            auto* webviewHwnd = static_cast<HWND>(m_webview->getViewHandle());
            ::ShowWindow(webviewHwnd, SW_HIDE);
        }

        [[nodiscard]] choc::ui::WebView* getWebview() noexcept {
            return m_webview.get();
        }

    private:
        std::uint32_t m_initialWidth{ 0 }, m_initialHeight{ 0 };
        ::HBRUSH m_brush{ nullptr };
        choc::ui::WebView::Options m_options{
            .enableDebugMode = false,
            .acceptsFirstMouseClick = true,
            .transparentBackground = true,
        };
        std::unique_ptr<choc::ui::WebView> m_webview{ nullptr };
    };

#elif defined(MOSTLY_HARMLESS_MACOS)
    class WebviewBase::Impl {
    public:
        Impl(std::uint32_t initialWidth, std::uint32_t initialHeight, Colour backgroundColour) : m_initialWidth(initialWidth),
                                                                                                 m_initialHeight(initialHeight),
                                                                                                 m_backgroundColour(backgroundColour) {
        }

        void setOptions(Options&& options) {
            m_options.enableDebugMode = options.enableDebug;
            m_options.initScript = options.initScript;
            if (options.contentProvider) {
                auto wrapper = [options](const std::string& toFind) -> std::optional<choc::ui::WebView::Options::Resource> {
                    auto res = options.contentProvider(toFind);
                    if (!res) return {};
                    choc::ui::WebView::Options::Resource resource;
                    resource.data = std::move(res->data);
                    resource.mimeType = std::move(res->mimeType);
                    return resource;
                };
                m_options.fetchResource = std::move(wrapper);
            };
        }

        void create() {
            m_clientView = helpers::macos::createView(m_initialWidth, m_initialHeight);
            m_webview = std::make_unique<choc::ui::WebView>(m_options);
            helpers::macos::setViewSize(m_clientView, m_initialWidth, m_initialHeight);
        }

        void destroy() {
            m_webview.reset();
        }

        void getSize(std::uint32_t* width, std::uint32_t* height) {
            helpers::macos::getViewSize(m_clientView, width, height);
        }

        void setSize(std::uint32_t width, std::uint32_t height) {
            helpers::macos::setViewSize(m_clientView, width, height);
        }

        void setParent(void* parentHandle) {
            helpers::macos::reparentView(parentHandle, m_clientView, m_webview->getViewHandle(), m_backgroundColour);
        }

        void show() {
            helpers::macos::showView(m_clientView);
        }

        void hide() {
            helpers::macos::hideView(m_clientView);
        }

        [[nodiscard]] choc::ui::WebView* getWebview() noexcept {
            return m_webview.get();
        }

    private:
        std::uint32_t m_initialWidth{ 0 }, m_initialHeight{ 0 };
        choc::ui::WebView::Options m_options{
            .enableDebugMode = false,
            .acceptsFirstMouseClick = true,
            .transparentBackground = true
        };
        void* m_clientView{ nullptr };
        std::unique_ptr<choc::ui::WebView> m_webview{ nullptr };
        Colour m_backgroundColour;
    };
#else
    static_assert(false);
#endif

    WebviewBase::WebviewBase(std::uint32_t initialWidth, std::uint32_t initialHeight, mostly_harmless::gui::Colour backgroundColour) {
        m_impl = std::make_unique<WebviewBase::Impl>(initialWidth, initialHeight, backgroundColour);
    }

    WebviewBase::~WebviewBase() noexcept {
    }

    void WebviewBase::setOptions(mostly_harmless::gui::WebviewBase::Options&& options) noexcept {
        m_impl->setOptions(std::move(options));
    }

    void WebviewBase::initialise(mostly_harmless::gui::EditorContext /*context*/) {
        m_impl->create();
        m_internalWebview = m_impl->getWebview();
    }

    void WebviewBase::destroy() {
        m_internalWebview = nullptr;
        m_impl->destroy();
    }

    void WebviewBase::getSize(std::uint32_t* width, std::uint32_t* height) {
        m_impl->getSize(width, height);
    }

    void WebviewBase::setSize(std::uint32_t width, std::uint32_t height) {
        m_impl->setSize(width, height);
    }

    void WebviewBase::setParent(void* parentHandle) {
        m_impl->setParent(parentHandle);
    }

    void WebviewBase::show() {
        m_impl->show();
    }

    void WebviewBase::hide() {
        m_impl->hide();
    }


} // namespace mostly_harmless::gui
