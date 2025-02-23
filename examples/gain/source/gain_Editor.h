#ifndef GAIN_WEBVIEWEDITOR_H
#define GAIN_WEBVIEWEDITOR_H

#include "gain_SharedState.h"
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>
#include <unordered_map>

class Editor final : public mostly_harmless::gui::WebviewEditor {
public:
    Editor(SharedState* sharedState);
    void initialise();
    bool allowResize() const noexcept override;

private:
    std::unordered_map<std::string, mostly_harmless::gui::WebviewEditor::Resource> m_resources;
};

#endif // GAIN_WEBVIEWEDITOR_H
