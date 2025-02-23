#ifndef SYNTH_SHAREDSTATE_H
#define SYNTH_SHAREDSTATE_H

#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
#include <synth_Parameters.h>
class SharedState final : public mostly_harmless::core::ISharedState {
public:
    explicit SharedState(mostly_harmless::core::SharedStateContext&& context);
    [[nodiscard]] Parameters getParamView() noexcept;
    void loadState(std::string_view loadedData) override;
    void saveState(std::ostringstream& dest) override;

private:
    Parameters m_params;
};

#endif // SYNTH_SHAREDSTATE_H
