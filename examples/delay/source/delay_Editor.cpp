#include "delay_Editor.h"
#include <mostlyharmless_DelayWebResources.h>
#include <mostly_harmless/utils/mostlyharmless_Macros.h>

[[nodiscard]] mostly_harmless::gui::WebviewEditor::Resource createResourceFor(const std::string& name) {
    auto resOpt = mostly_harmless::DelayWebResources::getNamedResource(name);
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

Editor::Editor(SharedState* sharedState) : mostly_harmless::gui::WebviewEditor(sharedState, 500, 500, mostly_harmless::gui::Colour{ 0xFF89CC04 }) {
    auto paramView = sharedState->getParamView();
    std::stringstream initialDataStream;
    initialDataStream << "window.params = {\n";
    initialDataStream << "    kTime: {\n ";
    initialDataStream << "        id: " << Params::kTime << ",\n";
    initialDataStream << "        min: " << paramView.timeParam->range.min << ",\n";
    initialDataStream << "        max: " << paramView.timeParam->range.max << ",\n";
    initialDataStream << "    },\n";
    initialDataStream << "    kFeedback: {\n";
    initialDataStream << "        id: " << Params::kFeedback << ",\n";
    initialDataStream << "        min: " << paramView.feedbackParam->range.min << ",\n";
    initialDataStream << "        max: " << paramView.feedbackParam->range.max << ",\n";
    initialDataStream << "    },\n";
    initialDataStream << "    kDryWet: {\n";
    initialDataStream << "        id: " << Params::kDryWet << ",\n";
    initialDataStream << "        min: " << paramView.dryWetParam->range.min << ",\n";
    initialDataStream << "        max: " << paramView.dryWetParam->range.max << ",\n";
    initialDataStream << "    },\n";
    initialDataStream << "};";
#if defined(HOT_RELOAD)
    this->setOptions({ .enableDebug = true, .initScript = initialDataStream.str() });
#else
    m_resources.emplace("/index.html", createResourceFor("index.html"));
    m_resources.emplace("/index.css", createResourceFor("index.css"));
    m_resources.emplace("/index.js", createResourceFor("index.js"));
    this->setOptions({
        .enableDebug = true,
        .contentProvider = [this](const std::string& name) -> std::optional<mostly_harmless::gui::WebviewEditor::Resource> {
            const auto requested = name == "/" ? "/index.html" : name;
            const auto it = m_resources.find(requested);
            if (it == m_resources.end()) return {};
            auto resource = it->second;
            return resource;
        },
        .initScript = initialDataStream.str(),
    });
#endif
}


void Editor::initialise() {
    mostly_harmless::gui::WebviewEditor::initialise();
#if defined(HOT_RELOAD)
    m_internalWebview->navigate("http://localhost:5173");
#endif
}
