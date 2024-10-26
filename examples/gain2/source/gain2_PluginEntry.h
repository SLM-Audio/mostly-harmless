//
// Created by Syl Morrison on 21/10/2024.
//

#ifndef MOSTLYHARMLESS_GAIN2_PLUGINENTRY_H
#define MOSTLYHARMLESS_GAIN2_PLUGINENTRY_H
#include <mostly_harmless/core/mostlyharmless_IPluginEntry.h>
namespace gain2 {
    struct PluginEntry : mostly_harmless::core::IPluginEntry {
        std::unique_ptr<mostly_harmless::core::ISharedState> createState(mostly_harmless::core::SharedStateContext&& context) override;
        std::unique_ptr<mostly_harmless::core::IEngine> createEngine(mostly_harmless::core::ISharedState* sharedState) override;
        std::unique_ptr<mostly_harmless::core::IEditor> createEditor(mostly_harmless::core::ISharedState* sharedState) override;
    };
} // namespace gain2

#endif // MOSTLYHARMLESS_GAIN2_PLUGINENTRY_H
