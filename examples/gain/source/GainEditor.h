//
// Created by Syl Morrison on 11/08/2024.
//

#ifndef MOSTLYHARMLESS_GAINEDITOR_H
#define MOSTLYHARMLESS_GAINEDITOR_H

#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>
namespace examples::gain {
    class GainEditor : public mostly_harmless::gui::WebviewEditor {
    public:
        GainEditor(std::uint32_t width, std::uint32_t height);
        void initialise() override;
        void onParamEvent(mostly_harmless::events::ParamEvent event) override;
    };
} // namespace examples::gain


#endif // MOSTLYHARMLESS_GAINEDITOR_H
