#ifndef $PREPRO_NAME$_WEBVIEWEDITOR_H
#define $PREPRO_NAME$_WEBVIEWEDITOR_H

#include "$LOWER_PROJECT_NAME$_SharedState.h"
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>

class Editor final : public mostly_harmless::gui::WebviewEditor {
public:
    explicit Editor(SharedState* sharedState);
    void initialise();
};

#endif // $PREPRO_NAME$_WEBVIEWEDITOR_H
