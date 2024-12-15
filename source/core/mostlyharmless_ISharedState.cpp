//
// Created by Syl Morrison on 20/10/2024.
//
#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
namespace mostly_harmless::core {
    ISharedState::ISharedState(SharedStateContext&& context, std::vector<Parameter<float>>&& params) : m_context(std::move(context)),
                                                                                                       m_params(std::move(params)),
                                                                                                       m_procToGuiQueue(1024),
                                                                                                       m_guiToProcQueue(1024) {
        for (auto& param : m_params) {
            m_paramLookup[param.pid] = &param;
        }
    }

    std::span<Parameter<float>> ISharedState::getRawParameterView() noexcept {
        return m_params;
    }

    Parameter<float>* ISharedState::getParameterByIndex(std::uint32_t index) noexcept {
        if (index >= m_params.size()) {
            return nullptr;
        }
        return &(m_params[index]);
    }

    Parameter<float>* ISharedState::getParameterById(mostly_harmless::ParameterID parameterId) {
        return getParameterById(parameterId.pid);
    }

    Parameter<float>* ISharedState::getParameterById(std::uint32_t pid) {
        if (!m_paramLookup.contains(pid)) {
            return nullptr;
        }
        return m_paramLookup.at(pid);
    }

    std::uint32_t ISharedState::getNumParams() const noexcept {
        return m_params.size();
    }

    marvin::containers::fifos::SPSC<events::ProcToGuiParamEvent>& ISharedState::getProcToGuiQueue() noexcept {
        return m_procToGuiQueue;
    }

    marvin::containers::fifos::SPSC<events::GuiToProcParamEvent>& ISharedState::getGuiToProcQueue() noexcept {
        return m_guiToProcQueue;
    }

    void ISharedState::runOnMainThread(std::function<void(void)>&& toRun) const {
        m_context.runOnMainThread(std::move(toRun));
    }

    void ISharedState::requestParamFlush() const {
        m_context.requestParamFlush();
    }

    void ISharedState::rescanParams() const {
        m_context.requestParamRescan();
    }


} // namespace mostly_harmless::core