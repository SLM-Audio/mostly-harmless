#include "Gain.h"
#include "marvin/containers/marvin_BufferView.h"
#include "mostlyharmless_Plugin.h"
#include <iostream>
namespace mostly_harmless::examples { 
    Gain::Gain(const clap_host* host) : Plugin<float>(host) { 
    }

    void Gain::initialise(double, std::uint32_t, std::uint32_t) noexcept { 

    }

    void Gain::process(marvin::containers::BufferView<float> buffer) noexcept { 

    }


}
MOSTLYHARMLESS_REGISTER(mostly_harmless::examples::Gain)