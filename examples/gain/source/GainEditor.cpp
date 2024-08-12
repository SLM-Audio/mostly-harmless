//
// Created by Syl Morrison on 11/08/2024.
//

#include "GainEditor.h"
#include <fmt/core.h>
#include <iostream>

namespace examples::gain {
    GainEditor::GainEditor(std::uint32_t width, std::uint32_t height) : mostly_harmless::gui::WebviewEditor(width, height) {
    }

    void GainEditor::initialise() {
        mostly_harmless::gui::WebviewEditor::initialise();
        m_internalWebview->navigate("http://localhost:5173");
    }

    void GainEditor::onParamEvent(mostly_harmless::events::ParamEvent event) {
        const auto formattedJs = fmt::format("console.log(\"param: {}, value: {}\")", event.paramId, event.value);
        m_internalWebview->evaluateJavascript(formattedJs, {});

    }
} // namespace examples::gain