#include "delay_SharedState.h"
#include <mostly_harmless/utils/mostlyharmless_Macros.h>
#include <nlohmann/json.hpp>

std::vector<mostly_harmless::Parameter<float>> createParameters() {
    std::vector<mostly_harmless::Parameter<float>> params;
    params.emplace_back(mostly_harmless::Parameter<float>{ Params::kTime, "Time", "delay", { .min = 0.1f, .max = 1.0f }, 0.5f, CLAP_PARAM_IS_AUTOMATABLE });
    params.emplace_back(mostly_harmless::Parameter<float>{ Params::kFeedback, "Feedback", "delay", { .min = 0.0f, .max = 1.0f }, 0.5f, CLAP_PARAM_IS_AUTOMATABLE });
    params.emplace_back(mostly_harmless::Parameter<float>{ Params::kDryWet, "Dry/Wet", "delay", { .min = 0.0f, .max = 1.0f }, 0.5f, CLAP_PARAM_IS_AUTOMATABLE });
    return params;
}

std::span<mostly_harmless::Parameter<float>*> ParamView::toSpan() noexcept {
    auto* asArray = reinterpret_cast<mostly_harmless::Parameter<float>**>(this);
    std::span<mostly_harmless::Parameter<float>*> asSpan{ asArray, kNumParams };
    return asSpan;
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

void SharedState::loadState(std::string_view loadedData) {
    auto paramArray = m_paramView.toSpan();
    nlohmann::json j = nlohmann::json::parse(loadedData);
    for (auto& p : paramArray) {
        const auto pid = std::to_string(p->pid);
        if (!j.contains(pid)) continue;
        const auto value = j[pid].get<float>();
        p->value = value;
    }
    MH_LOG(loadedData);
}

void SharedState::saveState(std::ostringstream& dest) {
    auto paramArray = m_paramView.toSpan();
    nlohmann::json j;
    for (auto& p : paramArray) {
        j[std::to_string(p->pid)] = p->value;
    }
    dest << j;
}