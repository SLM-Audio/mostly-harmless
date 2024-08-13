//
// Created by Syl on 13/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_EDITORCONTEXT_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_EDITORCONTEXT_H
#include <mostly_harmless/events/mostlyharmless_ParamEvent.h>
#include <marvin/containers/marvin_FIFO.h>
#include <functional>
namespace mostly_harmless::gui {
    /**
     * \brief Trivially copyable struct containing extra necessary context for a gui.
     */
    struct EditorContext final {
        /**
         * A pointer to a rt/ts fifo for sending param updates to the audio thread.
         */
        marvin::containers::fifos::SPSC<events::GuiToProcParamEvent>* guiToProcQueue{ nullptr };

        /**
         * A lambda calling back to the clap_host, to request flushing parameters.
         * This should be called after dispatching an event into the guiToProcQueue.
         */
        std::function<void(void)> requestParamFlush{ nullptr };

    };
}

#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_EDITORCONTEXT_H
