//
// Created by Syl on 17/08/2024.
//

#ifndef MOSTLYHARMLESS_PLUGINEDITOR_H
#define MOSTLYHARMLESS_PLUGINEDITOR_H
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>
class PluginEditor : public mostly_harmless::gui::WebviewEditor {
public:
    PluginEditor(std::uint32_t width, std::uint32_t height);
    void initialise(mostly_harmless::gui::EditorContext context) override;
    void onParamEvent(mostly_harmless::events::ProcToGuiParamEvent event) override;

private:
};
#endif // MOSTLYHARMLESS_PLUGINEDITOR_H
