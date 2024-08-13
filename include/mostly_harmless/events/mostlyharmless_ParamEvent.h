//
// Created by Syl on 12/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_PARAMEVENT_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_PARAMEVENT_H
#include <cstdint>
namespace mostly_harmless::events {
    /**
     * \brief POD type representing a param change event from the host, to be passed to the gui.
     */
    struct ProcToGuiParamEvent {
        /**
         * The unique param identifier
         */
        std::uint32_t paramId;
        /**
         * The new value of the param
         */
        double value;
    };

    struct GuiToProcParamEvent {
        /**
         * Represents the type of event sent by the gui.
         */
        enum class Type {
            /**
             * Called before the parameter is changed
             */
            Begin,
            /**
             * Called while the parameter is changing
             */
            Adjust,
            /**
             * Called after the parameter has changed.
             */
            End
        } type;
        /**
         * The id of the target parameter.
         */
        std::uint32_t paramId;
        /**
         * The new value of the target parameter.
         */
        double value;
    };
}
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_PARAMEVENT_H
