//
// Created by Syl on 12/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_PARAMEVENT_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_PARAMEVENT_H
#include <cstdint>
namespace mostly_harmless::events {
    /**
     * \brief POD type representing a param change event.
     */
    struct ParamEvent {
        /**
         * The unique param identifier
         */
        std::uint32_t paramId;
        /**
         * The new value of the param
         */
        double value;
    };
}
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_PARAMEVENT_H
