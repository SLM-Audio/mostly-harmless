#ifndef MOSTLYHARMLESS_BUSCONFIG_H
#define MOSTLYHARMLESS_BUSCONFIG_H
namespace mostly_harmless {
    /**
     * \brief Represents available IO configs, for both audio and midi.
     */
    enum class BusConfig { 
        InputOutput,
        InputOnly,
        OutputOnly,
        None
    };
}

#endif