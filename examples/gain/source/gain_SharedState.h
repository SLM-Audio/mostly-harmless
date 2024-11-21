#ifndef GAIN_SHAREDSTATE_H
#define GAIN_SHAREDSTATE_H

#include <mostly_harmless/core/mostlyharmless_ISharedState.h>

struct ParamView final {
    mostly_harmless::Parameter<float>* gainParam{ nullptr };
};

class SharedState final : public mostly_harmless::core::ISharedState {
public:
    explicit SharedState(mostly_harmless::core::SharedStateContext&& context);
    void loadState(std::string_view loadedData) override;
    void saveState(std::ostringstream& dest) override;
    [[nodiscard]] ParamView getParamView() const noexcept;

private:
    ParamView m_paramView;
};

#endif // GAIN_SHAREDSTATE_H
