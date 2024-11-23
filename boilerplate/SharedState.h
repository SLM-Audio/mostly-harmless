#ifndef $PREPRO_NAME$_SHAREDSTATE_H
#define $PREPRO_NAME$_SHAREDSTATE_H

#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
class SharedState final : public mostly_harmless::core::ISharedState {
public:
    explicit SharedState(mostly_harmless::core::SharedStateContext&& context);
    void loadState(std::string_view loadedData) override;
    void saveState(std::ostringstream& dest) override;

private:
};

#endif // $PREPRO_NAME$_SHAREDSTATE_H
