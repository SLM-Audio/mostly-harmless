#ifndef DELAY_WEBVIEWEDITOR_H
#define DELAY_WEBVIEWEDITOR_H

#include "delay_SharedState.h"
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>
#include <unordered_map>

class Editor final : public mostly_harmless::gui::WebviewEditor {
public:
    explicit Editor(SharedState* sharedState);
    void initialise() override;

private:
    std::unordered_map<std::string, mostly_harmless::gui::WebviewEditor::Resource> m_resources;
};

#endif // DELAY_WEBVIEWEDITOR_H
