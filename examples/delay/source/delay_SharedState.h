#ifndef DELAY_SHAREDSTATE_H
#define DELAY_SHAREDSTATE_H

#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
class SharedState final : public mostly_harmless::core::ISharedState {
public:
    SharedState(mostly_harmless::core::SharedStateContext&& context);
    void loadState(std::string_view loadedData) override;
    void saveState(std::ostringstream& dest) override;

private:
};

#endif // DELAY_SHAREDSTATE_H
