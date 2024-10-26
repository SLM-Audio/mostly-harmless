//
// Created by Syl Morrison on 20/10/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_ISHAREDSTATE_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_ISHAREDSTATE_H
#include <mostly_harmless/core/mostlyharmless_SharedStateContext.h>
#include <mostly_harmless/mostlyharmless_Parameters.h>
#include <mostly_harmless/events/mostlyharmless_ParamEvent.h>
#include <marvin/containers/marvin_FIFO.h>
#include <vector>
#include <unordered_map>
namespace mostly_harmless::core {

    class ISharedState {
    public:
        explicit ISharedState(SharedStateContext&& context, std::vector<Parameter<float>>&& params);
        virtual ~ISharedState() noexcept = default;

        [[nodiscard]] Parameter<float>* getParameterByIndex(std::uint32_t index) noexcept;
        [[nodiscard]] Parameter<float>* getParameterById(std::uint32_t id);

        [[nodiscard]] std::uint32_t getNumParams() const noexcept;

        virtual void loadState(std::string_view loadedData) = 0;
        virtual void saveState(std::ostringstream& dest) = 0;

        [[nodiscard]] marvin::containers::fifos::SPSC<events::ProcToGuiParamEvent>& getProcToGuiQueue() noexcept;
        [[nodiscard]] marvin::containers::fifos::SPSC<events::GuiToProcParamEvent>& getGuiToProcQueue() noexcept;

        void runOnMainThread(std::function<void(void)>&& toRun) const;
        void requestParamFlush() const;

    private:
        SharedStateContext m_context;
        std::vector<Parameter<float>> m_params;
        std::unordered_map<std::uint32_t, Parameter<float>*> m_paramLookup;
        marvin::containers::fifos::SPSC<events::ProcToGuiParamEvent> m_procToGuiQueue;
        marvin::containers::fifos::SPSC<events::GuiToProcParamEvent> m_guiToProcQueue;
    };


} // namespace mostly_harmless::core
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_ISHAREDSTATE_H
