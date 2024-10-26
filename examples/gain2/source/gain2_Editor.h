//
// Created by Syl Morrison on 20/10/2024.
//

#ifndef MOSTLYHARMLESS_GAIN2_EDITOR_H
#define MOSTLYHARMLESS_GAIN2_EDITOR_H
#include "gain2_SharedState.h"
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>
namespace gain2 {
    class Editor : public mostly_harmless::gui::WebviewEditor {
    public:
        explicit Editor(SharedState* sharedState);
        void initialise() override;

    private:
    };
} // namespace gain2

#endif // MOSTLYHARMLESS_GAIN2_EDITOR_H
