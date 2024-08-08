#ifndef GAIN_H
#define GAIN_H
#include "marvin/containers/marvin_BufferView.h"
#include "mostlyharmless_EventContext.h"
#include <mostlyharmless_Plugin.h>
namespace mostly_harmless::examples {
    enum ParamIds : std::uint32_t {
        kGain,
        kNumParams
    };

    struct Parameters final {
        Parameter<float>* gainParam{ nullptr };
    };
    class Gain : public mostly_harmless::Plugin<float> {
    public:
        explicit Gain(const clap_host* host);
        ~Gain() noexcept override = default;
        void initialise(double sampleRate, std::uint32_t minFrames, std::uint32_t maxFrames) noexcept override;
        void process(marvin::containers::BufferView<float> buffer, EventContext eventContext) noexcept override;

    private:
        Parameters m_params;
    };
} // namespace mostly_harmless::examples
#endif