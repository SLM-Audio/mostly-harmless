#include "delay_SharedState.h"

std::vector<mostly_harmless::Parameter<float>> createParameters() {
    std::vector<mostly_harmless::Parameter<float>> params;
    params.emplace_back(mostly_harmless::Parameter<float>{ Params::kTime, "Time", "delay", { .min = 0.1f, .max = 1.0f }, 0.5f, CLAP_PARAM_IS_AUTOMATABLE });
    params.emplace_back(mostly_harmless::Parameter<float>{ Params::kFeedback, "Feedback", "delay", { .min = 0.0f, .max = 1.0f }, 0.5f, CLAP_PARAM_IS_AUTOMATABLE });
    params.emplace_back(mostly_harmless::Parameter<float>{ Params::kDryWet, "Dry/Wet", "delay", { .min = 0.0f, .max = 1.0f }, 0.5f, CLAP_PARAM_IS_AUTOMATABLE });
    return params;
}

SharedState::SharedState(mostly_harmless::core::SharedStateContext&& context) : mostly_harmless::core::ISharedState(std::move(context), createParameters()) {
    if (auto timeParam = getParameterByIndex(Params::kTime)) {
        m_paramView.timeParam = timeParam;
    } else {
        assert(false);
    }
    if (auto feedbackParam = getParameterByIndex(Params::kFeedback)) {
        m_paramView.feedbackParam = feedbackParam;
    } else {
        assert(false);
    }
    if (auto dryWetParam = getParameterByIndex(Params::kDryWet)) {
        m_paramView.dryWetParam = dryWetParam;
    } else {
        assert(false);
    }
}

ParamView SharedState::getParamView() const noexcept {
    return m_paramView;
}

void SharedState::loadState(std::string_view /*loadedData*/) {
}

void SharedState::saveState(std::ostringstream& /*dest*/) {
}