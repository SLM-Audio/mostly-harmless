//
// Created by Syl Morrison on 23/02/2025.
//

#ifndef MOSTLYHARMLESS_SYNTH_PARAMETERS_H
#define MOSTLYHARMLESS_SYNTH_PARAMETERS_H
#include <mostly_harmless/mostlyharmless_Parameters.h>
struct Parameters {
    mostly_harmless::Parameter<float>* attackParam{ nullptr };
    mostly_harmless::Parameter<float>* releaseParam{ nullptr };
};
#endif // MOSTLYHARMLESS_SYNTH_PARAMETERS_H
