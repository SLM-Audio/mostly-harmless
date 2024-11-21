#ifndef DELAY_SHAREDSTATE_H
#define DELAY_SHAREDSTATE_H

#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
#include <mostly_harmless/utils/mostlyharmless_Hash.h>


struct ParamView final {
    mostly_harmless::Parameter<float>* timeParam{ nullptr };
    mostly_harmless::Parameter<float>* feedbackParam{ nullptr };
    mostly_harmless::Parameter<float>* dryWetParam{ nullptr };
};

class SharedState final : public mostly_harmless::core::ISharedState {
public:
    explicit SharedState(mostly_harmless::core::SharedStateContext&& context);
    [[nodiscard]] ParamView getParamView() const noexcept;
    void loadState(std::string_view loadedData) override;
    void saveState(std::ostringstream& dest) override;

private:
    ParamView m_paramView;
};

#endif // DELAY_SHAREDSTATE_H
