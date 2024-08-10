#ifndef MOSTLYHARMLESS_PLUGINEDITOR_H
#define MOSTLYHARMLESS_PLUGINEDITOR_H
#include "choc/gui/choc_WebView.h"
#include "clap/ext/gui.h"
#include <choc/gui/choc_DesktopWindow.h>

namespace mostly_harmless::gui {
    class PluginEditor {
    public:
        PluginEditor();
        ~PluginEditor() noexcept;
        void create();
        void destroy();
        void setSize(std::uint32_t width, std::uint32_t height);
        void getSize(std::uint32_t* width, std::uint32_t* height);
        void setParent(void* parentHandle);
        void show();
        void hide();

    private:
        std::unique_ptr<choc::ui::DesktopWindow> m_window;
        std::unique_ptr<choc::ui::WebView> m_webView;
    };
} // namespace mostly_harmless::gui
#endif