//
// Created by Syl Morrison on 20/09/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWBASE_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWBASE_H
#include <mostly_harmless/core/mostlyharmless_IEditor.h>
#include <mostly_harmless/gui/mostlyharmless_Colour.h>
#include <choc/gui/choc_WebView.h>
namespace mostly_harmless::gui {
    /**
     * \brief Tries to retrieve the MIME type for a given filename.
     * \param filename The filename to retrieve the MIME type for
     * \return The associated MIME type for the given filename if found, std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<std::string> getMimeType(const std::string& filename);

    /**
     * \brief Convenience wrapper layer implementing the creation and management of a `choc::ui::WebView`.
     * Still abstract, as `setParams` is left up to the user to implement in a way they see fit.
     *
     * Provides a "clean slate" for a webview based gui, providing windowing, a handle to the internal choc webview, and not much else.
     * If you want a more complete solution, consider deriving from (or directly using) WebviewEditor instead, which has opinionated parameter handling, etc.
     * This class' raison d'etre, then, is for a situation where you have your own strong opinions about how events etc should be handled, but don't want to deal with windowing yourself.
     *
     * The underlying `choc::ui::WebView` can be accessed via the protected `m_internalWebview` member.
     * If you don't call `setOptions` before initialise is called, then the internal webview will be constructed with some default options, namely:
     *
     * ```
     * enableDebug = false
     * transparentBackground = true
     * ```
     *
     */
    class WebviewBase : public core::IEditor {
    public:
        /**
         * \brief Tiny container struct for web resources.
         *
         * If serving from RAM, your editor should hold an internal map of `route:Resource`, populated in your constructor. <br>
         * MIME types can be retrieved with mostly_harmless::gui::getMimeType().
         */
        struct Resource {
            Resource() = default;
            /**
             * Constructs a Resource from a char[] and a mime type.
             * \param content A char[] containing the data for this resource.
             * \param mimeType_ The associated MIME type for this resource.
             */
            Resource(std::string_view content, std::string mimeType_);
            /**
             * The binary data for this resource.
             */
            std::vector<std::uint8_t> data;
            /**
             * The associated MIME type for this resource.
             */
            std::string mimeType;
        };

        /**
         * \brief Contains a set of options to construct the internal webview with.
         */
        struct Options {
            /**
             * If true, the user will be able to right click / inspect element / etc. If false, that behaviour is disabled.
             */
            bool enableDebug{ false };
            /**
             * If not serving from ram, leave this as a nullptr, and call `navigate` instead.<br>
             * If serving the content from ram, the webview will query the backend for files to load, with a call to this lambda.<br>
             * As mentioned in the docs for Resource, you should load these at construction, and keep a map `route:Resource` on hand. This lambda then, should query that map for the requested route, and return the associated resource if
             * it exists, std::nullopt otherwise. Assuming a `std::unordered_map<std::string, mostly_harmless::gui::WebviewEditor::Resource>` called `m_resources`, an implementation could be along the lines of:
             *
             * ```cpp
             * const auto requested = url == "/" ? "/index.html" : url;
             * const auto it = m_resources.find(requested);
             * if (it == m_resources.end()) return {};
             * auto resource = it->second;
             * return resource;
             * ```
             *
             */
            std::function<std::optional<Resource>(const std::string&)> contentProvider{ nullptr };
            /**
             * An optional (javascript) script to be executed before the page loads. If you're hosting from RAM, prefer this to the internal webview's addInitScript function - this is because internally in choc, navigate is called before the init script
             * is added, meaning that a script added with addInitScript won't execute until a refresh. This arg sidesteps that, by passing it into the internal webview's constructor.
             */
            std::optional<std::string> initScript{};
        };
        WebviewBase(std::uint32_t initialWidth, std::uint32_t initialHeight, Colour backgroundColour);
        ~WebviewBase() noexcept override;
        void setOptions(Options&& options) noexcept;
        void initialise() override;
        void destroy() override;
        [[nodiscard]] bool allowResize() const noexcept override;
        void setSize(std::uint32_t width, std::uint32_t height) override;
        void getSize(std::uint32_t* width, std::uint32_t* height) override;
        void onResizeRequested(std::uint32_t* newWidth, std::uint32_t* newHeight) override;
        void setParent(void* parentHandle) override;
        void show() override;
        void hide() override;
        void onParamEvent(events::ProcToGuiParamEvent event) override = 0;

    protected:
        choc::ui::WebView* m_internalWebview{ nullptr };

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
} // namespace mostly_harmless::gui
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWBASE_H
