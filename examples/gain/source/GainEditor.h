//
// Created by Syl Morrison on 11/08/2024.
//

#ifndef MOSTLYHARMLESS_GAINEDITOR_H
#define MOSTLYHARMLESS_GAINEDITOR_H

#include "Parameters.h"
#include <mostly_harmless/gui/mostlyharmless_WebviewEditor.h>
namespace examples::gain {
    class GainEditor : public mostly_harmless::gui::WebviewEditor {
    public:
        GainEditor(std::uint32_t width, std::uint32_t height);
        void initialise(mostly_harmless::gui::EditorContext context) override;

    private:
        // Usually I'd have this as a static in the TU, but because the underlying map `getNamedResource()` queries is also static, it leads to weird issues with init-order
        // If I find a way to guarantee the order, I'll probably switch to that approach instead, but for now this will have to do
        std::unordered_map<std::string, mostly_harmless::gui::WebviewEditor::Resource> m_resources;
    };
} // namespace examples::gain


#endif // MOSTLYHARMLESS_GAINEDITOR_H
