//
// Created by Syl Morrison on 11/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWEDITOR_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWEDITOR_H
#include <mostly_harmless/gui/mostlyharmless_IEditor.h>
#include <mostly_harmless/gui/mostlyharmless_Colour.h>
#include <mostly_harmless/events/mostlyharmless_WebEvent.h>
#include <marvin/library/marvin_Concepts.h>
#include <choc/gui/choc_WebView.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <memory>
namespace mostly_harmless::gui {
    /**
     * \brief Tries to retrieve the MIME type for a given filename.
     * \param filename The filename to retrieve the MIME type for
     * \return The associated MIME type for the given filename if found, std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<std::string> getMimeType(const std::string& filename);

    /**
     * \brief Convenience wrapper layer implementing the creation and management of a `choc::ui::WebView`.
     *
     * Provides the basic setup code for the Webview, but you'll likely want to derive from this, and override some of its functions.
     * The underlying `choc::ui::WebView` can be accessed via the protected `m_internalWebview` member.
     * If you don't call `setOptions` before initialise is called, then the internal webview will be constructed with some default options, namely:
     *
     * ```
     * enableDebug = false
     * transparentBackground = true
     * ```
     *
     */
    class WebviewEditor : public IEditor {
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
        /**
         * \param initialWidth The initial width for the webview.
         * \param initialHeight The initial height for the webview.
         * \param backgroundColour The colour to paint the actual window beneath the webview.
         */
        WebviewEditor(std::uint32_t initialWidth, std::uint32_t initialHeight, Colour backgroundColour);
        /**
         * Non default destructor for pimpl
         */
        ~WebviewEditor() noexcept override;
        /**
         * Specify some options to pass to the internal webview's constructor - note that this *must* be called *before* initialise (ie in your constructor) for them to get picked up.<br>
         * \param opts The options to pass to the internal webview
         */
        void setOptions(Options&& options) noexcept;
        /**
         * Implementation of mostly_harmless::gui::IEditor::initialise().
         * \param context The editor context (see IEditor::initialise() and EditorContext for more details).
         */
        void initialise(EditorContext context) override;
        /**
         * Implementation of mostly_harmless::gui::IEditor::destroy().
         */
        void destroy() override;
        /**
         * Implementation of mostly_harmless::gui::IEditor::setSize().
         */
        void setSize(std::uint32_t width, std::uint32_t height) override;
        /**
         * Implementation of mostly_harmless::gui::IEditor::getSize().
         */
        void getSize(std::uint32_t* width, std::uint32_t* height) override;
        /**
         * Implementation of mostly_harmless::gui::IEditor::setParent().
         */
        void setParent(void* parentHandle) override;
        /**
         * Implementation of mostly_harmless::gui::IEditor::show().
         */
        void show() override;
        /**
         * Implementation of mostly_harmless::gui::IEditor::hide().
         */
        void hide() override;

        /**
         * Still pure virtual - you need to implement this to send data to your webview.
         * see IEditor::onParamEvent() for more details.
         */
        void onParamEvent(events::ProcToGuiParamEvent event) override;

        virtual void sendEvent(events::WebEvent&& event) noexcept;


    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        /**
         * A pointer to the internal choc::ui::WebView owned by this class, for extension in any derived classes you may write.
         */
        choc::ui::WebView* m_internalWebview{ nullptr };
    };
} // namespace mostly_harmless::gui
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWEDITOR_H
