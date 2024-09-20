//
// Created by Syl Morrison on 11/08/2024.
//

#include "GainEditor.h"
#include <fmt/core.h>
#include <iostream>
#include <sstream>
#include <magic_enum.hpp>
#include "Gain.h"
#include <mostlyharmless_WebResources.h>

namespace examples::gain {
    [[nodiscard]] mostly_harmless::gui::WebviewEditor::Resource createResourceFor(const std::string& name) {
        auto resOpt = mostly_harmless::WebResources::getNamedResource(name);
        assert(resOpt);
        auto [data, size] = *resOpt;
        auto mimeType = mostly_harmless::gui::getMimeType(name);
        assert(mimeType);
        // Evil evil evil evil evil evil (but safe I think)
        auto* asUnsigned = reinterpret_cast<const std::uint8_t*>(data);
        mostly_harmless::gui::WebviewEditor::Resource temp;
        temp.data = { asUnsigned, asUnsigned + size };
        temp.mimeType = *mimeType;
        return temp;
    }

    GainEditor::GainEditor(std::uint32_t width, std::uint32_t height) : mostly_harmless::gui::WebviewEditor(width, height, mostly_harmless::gui::Colour(0xFF89CC04)) {
        // a map of paramEnum : paramId will be available in `window.params`.
        std::stringstream initialDataStream;
        initialDataStream << "window.params = { \n";
        for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(ParamIds::kNumParams); ++i) {
            const auto asEnum = static_cast<ParamIds>(i);
            const auto asStr = magic_enum::enum_name(asEnum);
            initialDataStream << "    " << asStr << ": " << i << ",\n";
        }
        initialDataStream << "};";
#ifndef GAIN_HOT_RELOAD
        m_resources.emplace("/index.html", createResourceFor("index.html"));
        m_resources.emplace("/index.css", createResourceFor("index.css"));
        m_resources.emplace("/index.js", createResourceFor("index.js"));
        auto contentProvider = [this](const std::string& url) -> std::optional<mostly_harmless::gui::WebviewEditor::Resource> {
            const auto requested = url == "/" ? "/index.html" : url;
            const auto it = m_resources.find(requested);
            if (it == m_resources.end()) return {};
            auto resource = it->second;
            return resource;
        };
        this->setOptions({ .enableDebug = true, .contentProvider = std::move(contentProvider), .initScript = initialDataStream.str() });
#else
        this->setOptions({ .enableDebug = true, .initScript = initialDataStream.str() });
#endif
    }

    void GainEditor::initialise(mostly_harmless::gui::EditorContext context) {
        mostly_harmless::gui::WebviewEditor::initialise(context);
#if defined(GAIN_HOT_RELOAD)
        m_internalWebview->navigate("http://localhost:5173");
#endif
    }

} // namespace examples::gain