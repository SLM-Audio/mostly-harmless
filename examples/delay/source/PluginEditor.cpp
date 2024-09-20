//
// Created by Syl on 19/08/2024.
//

#include "PluginEditor.h"
#include "Parameters.h"
#include <magic_enum.hpp>
namespace examples::delay {
    PluginEditor::PluginEditor(std::uint32_t width, std::uint32_t height) : mostly_harmless::gui::WebviewEditor(width, height, mostly_harmless::gui::Colour{ 0xFF89CC04 }) {
        std::stringstream initialDataStream;
        initialDataStream << "window.params = { \n";
        for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(ParamIds::kNumParams); ++i) {
            const auto asEnum = static_cast<ParamIds>(i);
            const auto asStr = magic_enum::enum_name(asEnum);
            initialDataStream << "    " << asStr << ": " << i << ",\n";
        }
        initialDataStream << "};";
        setOptions({ .enableDebug = true, .initScript = initialDataStream.str() });
    }

    void PluginEditor::initialise(mostly_harmless::gui::EditorContext context) {
        mostly_harmless::gui::WebviewEditor::initialise(context);

        m_internalWebview->navigate("http://localhost:5173");
    }

} // namespace examples::delay