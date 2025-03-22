#include "$PROJECT_NAME$_Editor.h"

[[nodiscard]] static mostly_harmless::gui::WebviewEditor::Resource createResourceFor(const std::string& name) {
    /**
     * TODO: Fill in your logic for retrieving a resource here..
     * Something like::
     * auto resOpt = mostly_harmless::[[YourBinaryData]]::getNamedResource(name);
     * assert(resOpt);
     * auto [data, size] = *resOpt;
     * auto mimeType = mostly_harmless::gui::getMimeType(name);
     * assert(mimeType);
     * auto* asUnsigned = reinterpret_cast<const std::uint8_t*>(data);
     * mostly_harmless::gui::WebviewEditor::Resource temp;
     * temp.data = { asUnsigned, asUnsigned + size };
     * temp.mimeType = *mimeType;
     * return temp;
     */
    mostly_harmless::gui::WebviewEditor::Resource temp;
    return temp;
}

Editor::Editor(SharedState* sharedState) : mostly_harmless::gui::WebviewEditor(sharedState, 500, 500, mostly_harmless::gui::Colour{ 0xFF000000 }) {
    std::stringstream initialDataStream;
    /**
     * TODO: Add any data you need in the webview at init time here...
     */
#if defined(HOT_RELOAD)
    this->setOptions({ .enableDebug = true, .initScript = initialDataStream.str() });
#else
    /**
     * TODO: emplace any resources you need into m_resources here...
     * eg: m_resources.emplace("/index.html", createResourceFor("index.html"));
     */
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

bool Editor::allowResize() const noexcept {
    return true;
}
