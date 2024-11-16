//
// Created by Syl Morrison on 20/10/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_IPLUGINENTRY_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_IPLUGINENTRY_H
#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
#include <mostly_harmless/core/mostlyharmless_IEngine.h>
#include <mostly_harmless/core/mostlyharmless_IEditor.h>

namespace mostly_harmless::core {
    struct IPluginEntry {
        virtual ~IPluginEntry() noexcept = default;
        virtual std::unique_ptr<ISharedState> createState(SharedStateContext&& context) = 0;
        virtual std::unique_ptr<IEngine> createEngine(ISharedState* sharedState) = 0;
        virtual bool hasGui() const noexcept = 0;
        virtual std::unique_ptr<IEditor> createEditor(ISharedState* sharedState) = 0;
    };
} // namespace mostly_harmless::core

#define MH_REGISTER_PLUGIN_ENTRY(PluginEntryType)                    \
    namespace mostly_harmless::core {                                \
        std::unique_ptr<IPluginEntry> createPluginEntry() noexcept { \
            return std::make_unique<PluginEntryType>();              \
        }                                                            \
    }

#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_IPLUGINENTRY_H
