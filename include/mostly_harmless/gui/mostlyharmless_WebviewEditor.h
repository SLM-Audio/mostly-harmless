//
// Created by Syl Morrison on 11/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWEDITOR_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWEDITOR_H
#include <mostly_harmless/gui/mostlyharmless_IEditor.h>
#include <choc/gui/choc_WebView.h>
#include <memory>
namespace mostly_harmless::gui {
    class WebviewEditor : public IEditor {
    public:
        WebviewEditor(std::uint32_t initialWidth, std::uint32_t initialHeight);
        // Non default destructor for pimpl
        ~WebviewEditor() noexcept override;
        void initialise() override;
        void destroy() override;
        void setSize(std::uint32_t width, std::uint32_t height) override;
        void getSize(std::uint32_t* width, std::uint32_t* height) override;
        void setParent(void* parentHandle) override;
        void show() override;
        void hide() override;


    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        choc::ui::WebView* m_internalWebview{ nullptr };
    };
} // namespace mostly_harmless::gui
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_WEBVIEWEDITOR_H
