#include "$PROJECT_NAME$_Editor.h"

[[nodiscard]] std::optional<mostly_harmless::gui::WebviewEditor::Resource> createResourceFor(const std::string& name) {
    /**
     * TODO: Fill in your logic for retrieving a resource here..
     */
    return {};
}

Editor::Editor(SharedState* sharedState) : mostly_harmless::gui::WebviewEditor(sharedState, 500, 500, mostly_harmless::gui::Colour{ 0xFF000000 }) {
    std::stringstream initialDataStream;
    /**
     * TODO: Add any data you need in the webview at init time here...
     */
#if defined(HOT_RELOAD)
    this->setOptions({ .enableDebug = true, .initScript = initialDataStream.str() });
#else
    this->setOptions({
        .enableDebug = true,
        .contentProvider = [](const std::string& name) -> std::optional<mostly_harmless::gui::WebviewEditor::Resource> {
            return createResourceFor(name);
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
