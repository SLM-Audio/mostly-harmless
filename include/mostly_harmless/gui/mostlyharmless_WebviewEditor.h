//
// Created by Syl Morrison on 11/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWEDITOR_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWEDITOR_H
#include <mostly_harmless/gui/mostlyharmless_IEditor.h>
#include <choc/gui/choc_WebView.h>
#include <memory>
namespace mostly_harmless::gui {
    /**
     * \brief Convenience wrapper layer implementing the creation and management of a `choc::ui::WebView`.
     *
     * Provides the basic setup code for the Webview, but you'll likely want to derive from this, and override some of its functions.
     * The underlying `choc::ui::WebView` can be accessed via the protected `m_internalWebview` member.
     */
    class WebviewEditor : public IEditor {
    public:
        /**
         * \param initialWidth The initial width for the webview.
         * \param initialHeight The initial height for the webview.
         */
        WebviewEditor(std::uint32_t initialWidth, std::uint32_t initialHeight);
        /**
         * Non default destructor for pimpl
         */
        ~WebviewEditor() noexcept override;
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
        void onParamEvent(events::ProcToGuiParamEvent event) override = 0;


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
