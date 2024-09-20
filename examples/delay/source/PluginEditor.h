//
// Created by Syl on 19/08/2024.
//

#ifndef MOSTLYHARMLESS_DELAYPLUGINEDITOR_H
#define MOSTLYHARMLESS_DELAYPLUGINEDITOR_H
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>
namespace examples::delay {
    class PluginEditor final : public mostly_harmless::gui::WebviewEditor {
    public:
        PluginEditor(std::uint32_t width, std::uint32_t height);
        void initialise(mostly_harmless::gui::EditorContext context) override;
    };
} // namespace examples::delay
#endif // MOSTLYHARMLESS_PLUGINEDITOR_H
