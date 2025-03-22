#ifndef SYNTH_WEBVIEWEDITOR_H
#define SYNTH_WEBVIEWEDITOR_H

#include "synth_SharedState.h"
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>

class Editor final : public mostly_harmless::gui::WebviewEditor {
public:
    explicit Editor(SharedState* sharedState);
    void initialise() override;

private:
    std::unordered_map<std::string, mostly_harmless::gui::WebviewEditor::Resource> m_resources;
    SharedState* m_sharedState{ nullptr };
};

#endif // SYNTH_WEBVIEWEDITOR_H
