#ifndef DELAY_WEBVIEWEDITOR_H
#define DELAY_WEBVIEWEDITOR_H

#include "delay_SharedState.h"
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>

class Editor final : public mostly_harmless::gui::WebviewEditor {
public:
    Editor(SharedState* sharedState);
    void initialise();
};

#endif // DELAY_WEBVIEWEDITOR_H
