//
// Created by Syl Morrison on 21/10/2024.
//
#include "gain2_PluginEntry.h"
#include "gain2_SharedState.h"
#include "gain2_Engine.h"
#include "gain2_Editor.h"
namespace gain2 {
    SharedState* asUserState(mostly_harmless::core::ISharedState* sharedState) {
        return static_cast<SharedState*>(sharedState);
    }

    std::unique_ptr<mostly_harmless::core::ISharedState> PluginEntry::createState(mostly_harmless::core::SharedStateContext&& context) {
        return std::make_unique<SharedState>(std::move(context));
    }

    std::unique_ptr<mostly_harmless::core::IEngine> PluginEntry::createEngine(mostly_harmless::core::ISharedState* sharedState) {
        return std::make_unique<Engine>(asUserState(sharedState));
    }

    std::unique_ptr<mostly_harmless::core::IEditor> PluginEntry::createEditor(mostly_harmless::core::ISharedState* sharedState) {
        return std::make_unique<Editor>(asUserState(sharedState));
    }

} // namespace gain2
MH_REGISTER_PLUGIN_ENTRY(gain2::PluginEntry);
