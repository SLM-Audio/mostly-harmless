#ifndef MOSTLYHARMLESS_BUSCONFIG_H
#define MOSTLYHARMLESS_BUSCONFIG_H
namespace mostly_harmless { 
    enum class BusConfig { 
        InputOutput,
        InputOnly,
        OutputOnly,
        None
    };
}

#endif