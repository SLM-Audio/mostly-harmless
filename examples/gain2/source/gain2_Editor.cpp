//
// Created by Syl Morrison on 20/10/2024.
//
#include "gain2_Editor.h"
#include <mostlyharmless_Gain2WebResources.h>
namespace gain2 {
    [[nodiscard]] mostly_harmless::gui::WebviewEditor::Resource createResourceFor(const std::string& name) {
        auto resOpt = mostly_harmless::Gain2WebResources::getNamedResource(name);
        assert(resOpt);
        auto [data, size] = *resOpt;
        auto mimeType = mostly_harmless::gui::getMimeType(name);
        assert(mimeType);
        auto* asUnsigned = reinterpret_cast<const std::uint8_t*>(data);
        mostly_harmless::gui::WebviewEditor::Resource temp;
        temp.data = { asUnsigned, asUnsigned + size };
        temp.mimeType = *mimeType;
        return temp;
    }

    Editor::Editor(SharedState* sharedState) : mostly_harmless::gui::WebviewEditor(sharedState, 500, 500, mostly_harmless::gui::Colour(0xFF89CC04)) {
        std::stringstream initialDataStream;
        initialDataStream << "window.params = { \n";
        initialDataStream << "    kGain: " << Params::kGain << ",\n";
        initialDataStream << "};";
        this->setOptions({ .enableDebug = true, .initScript = initialDataStream.str() });
    }

    void Editor::initialise() {
        mostly_harmless::gui::WebviewEditor::initialise();
#if defined(GAIN_HOT_RELOAD)
        m_internalWebview->navigate("http://localhost:5173");
#endif
    }
} // namespace gain2