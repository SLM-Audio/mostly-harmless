#include "Plugin.h"
#include "marvin/containers/marvin_BufferView.h"
#include "mostlyharmless_Parameters.h"
#include "mostlyharmless_Plugin.h"
std::vector<mostly_harmless::Parameter<float>> createParameters() {
    return {};
}
Plugin::Plugin(const clap_host* host) : mostly_harmless::Plugin<float>(host, createParameters()) {
}

void Plugin::initialise(double sampleRate, std::uint32_t minFrames, std::uint32_t maxFrames) noexcept {
}

void Plugin::process(marvin::containers::BufferView<float> buffer) noexcept {
}

MOSTLYHARMLESS_REGISTER(Plugin)