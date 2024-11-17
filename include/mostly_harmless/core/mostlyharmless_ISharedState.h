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

    /**
     * \brief Base class for the plugin's shared state.
     *
     * As detailed in the FX Plugin tutorial, users must implement this, IEngine, IPluginEntry, and optionally IEditor, and register their subclasses with
     * the framework to create an audio plugin. These will also be created if using the provided `create.py` script.
     *
     * Designed such that the Engine and Editor can be completely unaware of eachother, all communications between the two should go through this class.
     *
     * Handles and owns all things shared between the gui and the engine - things like parameters, queues, and state restoration.
     * Custom shared state should also be implemented in this class.
     */
    class ISharedState {
    public:
        /**
         * Constructs an ISharedState - obviously subclasses must call this constructor in their init lists.
         *
         * \param context An rvalue ref to a SharedStateContext, containing functionality needed by SharedState (callOnMainThread, and the ability to request parameter flushes). In practice, an instance of this class will be passed to IPluginEntry::createState, and the user can forward this argument to this constructor.
         * \param params An rvalue ref to a vector containing all the plugin's parameters - ISharedState takes ownership of the parameters and manages them, but needs to know what the parameters are in the first place.
         */
        explicit ISharedState(SharedStateContext&& context, std::vector<Parameter<float>>&& params);

        /**
         * Virtual destructor.
         */
        virtual ~ISharedState() noexcept = default;

        /**
         * Attempts to retrieve a parameter by index - see Parameter for more details.
         * \param index The index to attempt to retrieve
         * \return A raw pointer to the parameter with index if found, nullptr otherwise.
         */
        [[nodiscard]] Parameter<float>* getParameterByIndex(std::uint32_t index) noexcept;

        /**
         * Attempts to retrieve a parameter by id - see Parameter for more details.
         * \param id The id to attempt to retrieve.
         * \return A raw pointer to the parameter with id if found, nullptr otherwise.
         */
        [[nodiscard]] Parameter<float>* getParameterById(std::uint32_t id);

        /**
         * Retrieves the number of params registered with the ISharedState instance.
         * \return The number of parameters.
         */
        [[nodiscard]] std::uint32_t getNumParams() const noexcept;

        /**
         * Called when the host wants to load state, users must override this to describe how deserialisation should be performed, and update their parameters accordingly.
         * \param loadedData A non-owning view into the deserialised state provided by the host.
         */
        virtual void loadState(std::string_view loadedData) = 0;

        /**
         * Called when the host wants to save state, users must override this to describe how serialisation should be performed.
         * \param dest A stringstream to push the serialised data into.
         */
        virtual void saveState(std::ostringstream& dest) = 0;

        /**
         * Retrieves the proc->gui parameter spsc, for direct access by either the engine or the editor.
         * \return A ref to the proc->gui parameter queue.
         */
        [[nodiscard]] marvin::containers::fifos::SPSC<events::ProcToGuiParamEvent>& getProcToGuiQueue() noexcept;

        /**
         * Retrieves the gui->proc parameters spsc, for direct access by either the engine or the editor.
         * \return A ref to the gui->proc parameter queue.
         */
        [[nodiscard]] marvin::containers::fifos::SPSC<events::GuiToProcParamEvent>& getGuiToProcQueue() noexcept;

        /**
         * Schedules a lambda to be called on the main/message thread. Note that this call won't be immediate, and it's up to the host to call it in a timely manner.
         * \param toRun An rvalue ref to the lambda to invoke on the message thread.
         */
        void runOnMainThread(std::function<void(void)>&& toRun) const;

        /**
         * Requests a param flush from the host.
         */
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