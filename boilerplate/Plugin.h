#ifndef PLUGIN_H
#define PLUGIN_H
#include "marvin/containers/marvin_BufferView.h"
#include <mostlyharmless_Plugin.h>
class Plugin : public mostly_harmless::Plugin<float> {
public:
    explicit Plugin(const clap_host* host);
    ~Plugin() noexcept override = default;
    void initialise(double sampleRate, std::uint32_t minFrames, std::uint32_t maxFrames) noexcept override;
    void process(marvin::containers::BufferView<float> buffer) noexcept override;

private:
};
#endif