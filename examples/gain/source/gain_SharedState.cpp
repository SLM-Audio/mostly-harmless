#include "gain_SharedState.h"
#include <nlohmann/json.hpp>

std::vector<mostly_harmless::Parameter<float>> createParameters() {
    std::vector<mostly_harmless::Parameter<float>> params;
    params.emplace_back(mostly_harmless::Parameter<float>{ mostly_harmless::ParameterID{ "Gain" }, "Gain", "gain", { .min = 0.0f, .max = 1.0f }, 1.0f, CLAP_PARAM_IS_AUTOMATABLE });
    return params;
}

SharedState::SharedState(mostly_harmless::core::SharedStateContext&& context) : mostly_harmless::core::ISharedState(std::move(context), createParameters()) {
    m_paramView.gainParam = getParameterById(mostly_harmless::ParameterID{ "Gain" });
}

void SharedState::loadState(std::string_view loadedData) {
    nlohmann::json j = nlohmann::json::parse(loadedData);
    const auto pid = std::to_string(m_paramView.gainParam->pid);
    if (!j.contains(pid)) return;
    m_paramView.gainParam->value = j[pid].get<float>();
}

void SharedState::saveState(std::ostringstream& dest) {
    nlohmann::json j;
    const auto pid = std::to_string(m_paramView.gainParam->pid);
    j[pid] = m_paramView.gainParam->value;
    dest << j;
}

ParamView SharedState::getParamView() const noexcept {
    return m_paramView;
}