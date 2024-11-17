#include "gain_SharedState.h"
#include <nlohmann/json.hpp>

std::vector<mostly_harmless::Parameter<float>> createParameters() {
    std::vector<mostly_harmless::Parameter<float>> params;
    params.emplace_back(mostly_harmless::Parameter<float>{ Params::kGain, "Gain", "gain", { .min = 0.0f, .max = 1.0f }, 1.0f, CLAP_PARAM_IS_AUTOMATABLE });
    return params;
}

SharedState::SharedState(mostly_harmless::core::SharedStateContext&& context) : mostly_harmless::core::ISharedState(std::move(context), createParameters()) {
    m_paramView.gainParam = getParameterByIndex(Params::kGain);
}

void SharedState::loadState(std::string_view loadedData) {
    nlohmann::json j = nlohmann::json::parse(loadedData);
    const auto gainPid = std::to_string(Params::kGain);
    if (!j.contains(gainPid)) return;
    m_paramView.gainParam->value = j[gainPid].get<float>();
}

void SharedState::saveState(std::ostringstream& dest) {
    nlohmann::json j;
    j[std::to_string(Params::kGain)] = m_paramView.gainParam->value;
    dest << j;
}

ParamView SharedState::getParamView() const noexcept {
    return m_paramView;
}