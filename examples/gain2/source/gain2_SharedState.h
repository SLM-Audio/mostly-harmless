//
// Created by Syl Morrison on 20/10/2024.
//

#ifndef MOSTLYHARMLESS_GAIN2_SHAREDSTATE_H
#define MOSTLYHARMLESS_GAIN2_SHAREDSTATE_H
#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
namespace gain2 {
    enum Params : std::uint32_t {
        kGain
    };

    struct ParamView final {
        mostly_harmless::Parameter<float>* gainParam{ nullptr };
    };

    class SharedState : public mostly_harmless::core::ISharedState {
    public:
        explicit SharedState(mostly_harmless::core::SharedStateContext&& context);
        [[nodiscard]] ParamView getParamView() const noexcept;

    private:
        void loadState(std::string_view toLoad) override;
        void saveState(std::ostringstream& dest) override;
        ParamView m_paramView;
    };
} // namespace gain2

#endif // MOSTLYHARMLESS_GAIN2_SHAREDSTATE_H
