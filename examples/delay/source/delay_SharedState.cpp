#include "delay_SharedState.h"
#include <mostly_harmless/utils/mostlyharmless_Macros.h>
#include <nlohmann/json.hpp>

std::vector<mostly_harmless::Parameter<float>> createParameters() {
    std::vector<mostly_harmless::Parameter<float>> params;
    params.emplace_back(mostly_harmless::Parameter<float>{ mostly_harmless::ParameterID{ "Time" }, "Time", "delay", { .min = 0.1f, .max = 1.0f }, 0.5f, CLAP_PARAM_IS_AUTOMATABLE });
    params.emplace_back(mostly_harmless::Parameter<float>{ mostly_harmless::ParameterID{ "Feedback" }, "Feedback", "delay", { .min = 0.0f, .max = 1.0f }, 0.5f, CLAP_PARAM_IS_AUTOMATABLE });
    params.emplace_back(mostly_harmless::Parameter<float>{ mostly_harmless::ParameterID{ "Dry/Wet" }, "Dry/Wet", "delay", { .min = 0.0f, .max = 1.0f }, 0.5f, CLAP_PARAM_IS_AUTOMATABLE });
    return params;
}


SharedState::SharedState(mostly_harmless::core::SharedStateContext&& context) : mostly_harmless::core::ISharedState(std::move(context), createParameters()) {
    if (auto timeParam = getParameterById(mostly_harmless::ParameterID{ "Time" })) {
        m_paramView.timeParam = timeParam;
    } else {
        assert(false);
    }
    if (auto feedbackParam = getParameterById(mostly_harmless::ParameterID{ "Feedback" })) {
        m_paramView.feedbackParam = feedbackParam;
    } else {
        assert(false);
    }
    if (auto dryWetParam = getParameterById(mostly_harmless::ParameterID{ "Dry/Wet" })) {
        m_paramView.dryWetParam = dryWetParam;
    } else {
        assert(false);
    }
}

ParamView SharedState::getParamView() const noexcept {
    return m_paramView;
}

void SharedState::loadState(std::string_view loadedData) {
    auto paramArray = getRawParameterView();
    nlohmann::json j = nlohmann::json::parse(loadedData);
    for (auto& p : paramArray) {
        const auto pid = p.parameterId.toString();
        if (!j.contains(pid)) continue;
        const auto value = j[pid].get<float>();
        p.value = value;
    }
    MH_LOG(loadedData);
}

void SharedState::saveState(std::ostringstream& dest) {
    auto paramArray = getRawParameterView();
    nlohmann::json j;
    for (auto& p : paramArray) {
        const auto pid = p.parameterId.toString();
        j[pid] = p.value;
    }
    dest << j;
}