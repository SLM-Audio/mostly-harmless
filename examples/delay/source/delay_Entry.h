#ifndef DELAY_ENTRY_H
#define DELAY_ENTRY_H

#include <mostly_harmless/core/mostlyharmless_IPluginEntry.h>
struct Entry final : public mostly_harmless::core::IPluginEntry {
    [[nodiscard]] std::unique_ptr<mostly_harmless::core::ISharedState> createState(mostly_harmless::core::SharedStateContext&& context) override;
    [[nodiscard]] std::unique_ptr<mostly_harmless::core::IEngine> createEngine(mostly_harmless::core::ISharedState* sharedState) override;
    [[nodiscard]] bool hasGui() const noexcept override;
    [[nodiscard]] std::unique_ptr<mostly_harmless::core::IEditor> createEditor(mostly_harmless::core::ISharedState* sharedState) override;
};

#endif // DELAY_ENTRY_H
