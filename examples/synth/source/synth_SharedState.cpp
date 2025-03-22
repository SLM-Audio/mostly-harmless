#include "synth_SharedState.h"
#include <nlohmann/json.hpp>

static mostly_harmless::ParameterID s_attackId{ "Attack" };
static mostly_harmless::ParameterID s_releaseId{ "Release" };

static std::vector<mostly_harmless::Parameter<float>> createParameters() {
    std::vector<mostly_harmless::Parameter<float>> params;
    params.emplace_back(s_attackId, "Attack", "synth", marvin::utils::Range<float>{ 1e-4f, 2.0f }, 0.5f, CLAP_PARAM_IS_AUTOMATABLE);
    params.emplace_back(s_releaseId, "Release", "synth", marvin::utils::Range<float>{ 1e-4f, 2.0f }, 0.5f, CLAP_PARAM_IS_AUTOMATABLE);
    return params;
}

SharedState::SharedState(mostly_harmless::core::SharedStateContext&& context) : mostly_harmless::core::ISharedState(std::move(context), createParameters()) {
    m_params.attackParam = getParameterById(s_attackId);
    m_params.releaseParam = getParameterById(s_releaseId);
}

Parameters SharedState::getParamView() noexcept {
    return m_params;
}

void SharedState::loadState(std::string_view loadedData) {
    auto j = nlohmann::json::parse(loadedData);
    auto rawParamView = getRawParameterView();
    for (auto& p : rawParamView) {
        const auto pid = std::to_string(p.pid);
        if (!j.contains(pid)) {
            continue;
        }
        p.value = j[pid].get<float>();
    }
}

void SharedState::saveState(std::ostringstream& dest) {

    nlohmann::json j;
    auto rawParamView = getRawParameterView();
    for (auto& p : rawParamView) {
        const auto pid = std::to_string(p.pid);
        j[pid] = p.value;
    }
    dest << j;
}