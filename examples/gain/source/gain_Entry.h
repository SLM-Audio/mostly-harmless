#ifndef GAIN_ENTRY_H
#define GAIN_ENTRY_H

#include <mostly_harmless/core/mostlyharmless_IPluginEntry.h>
struct Entry final : public mostly_harmless::core::IPluginEntry {
    std::unique_ptr<mostly_harmless::core::ISharedState> createState(mostly_harmless::core::SharedStateContext&& context) override;
    std::unique_ptr<mostly_harmless::core::IEngine> createEngine(mostly_harmless::core::ISharedState* sharedState) override;
    bool hasGui() const noexcept override;
    std::unique_ptr<mostly_harmless::core::IEditor> createEditor(mostly_harmless::core::ISharedState* sharedState) override;
};

#endif // GAIN_ENTRY_H
