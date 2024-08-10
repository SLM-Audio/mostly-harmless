#ifndef MOSTLYHARMLESS_WEBVIEWEDITOR_H
#define MOSTLYHARMLESS_WEBVIEWEDITOR_H
#include "choc/gui/choc_WebView.h"
#include "gui/mostlyharmless_PluginEditor.h"
namespace mostly_harmless::gui {
    class WebviewEditor : public PluginEditor {
    public:
        void windowCreated() noexcept override;
        void windowDestroyed() noexcept override;

        void windowShown() noexcept override;
        void windowHidden() noexcept override;

    private:
        std::unique_ptr<choc::ui::WebView> m_webview;
    };
} // namespace mostly_harmless::gui
#endif
