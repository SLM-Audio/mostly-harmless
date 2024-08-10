#ifndef MOSTLYHARMLESS_PLUGINEDITOR_H
#define MOSTLYHARMLESS_PLUGINEDITOR_H
#include "choc/gui/choc_WebView.h"
#include "clap/ext/gui.h"
#include <choc/gui/choc_DesktopWindow.h>

namespace mostly_harmless::gui {
    class PluginEditor {
    public:
        PluginEditor();
        virtual ~PluginEditor() noexcept;
        void create() noexcept;
        void destroy();
        void setSize(std::uint32_t width, std::uint32_t height);
        void getSize(std::uint32_t* width, std::uint32_t* height);
        void setParent(void* parentHandle);
        void show();
        void hide();

        virtual void windowCreated() noexcept = 0;
        virtual void windowDestroyed() noexcept = 0;
        virtual void windowShown() noexcept = 0;
        virtual void windowHidden() noexcept = 0;

    protected:
        std::unique_ptr<choc::ui::DesktopWindow> m_window;
    };
} // namespace mostly_harmless::gui
#endif