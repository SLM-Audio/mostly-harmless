//
// Created by Syl Morrison on 20/10/2024.
//
#include "gain2_SharedState.h"
#include <clap/plugin.h>

namespace gain2 {
    std::vector<mostly_harmless::Parameter<float>> createParams() {
        std::vector<mostly_harmless::Parameter<float>> params;
        params.emplace_back(mostly_harmless::Parameter<float>{ Params::kGain, "Gain", "gain", { .min = 0.0f, .max = 1.0f }, 1.0f, CLAP_PARAM_IS_AUTOMATABLE });
        return params;
    }

    SharedState::SharedState(mostly_harmless::core::SharedStateContext&& context) : mostly_harmless::core::ISharedState(std::move(context), createParams()) {
        m_paramView.gainParam = getParameterByIndex(Params::kGain);
    }

    ParamView SharedState::getParamView() const noexcept {
        return m_paramView;
    }

    void SharedState::loadState(std::string_view /*toLoad*/) {
    }

    void SharedState::saveState(std::ostringstream& /*dest*/) {
    }
} // namespace gain2