
#include <mostly_harmless/mostlyharmless_Descriptor.h>
namespace mostly_harmless {
    // clang-format off
    static BusConfig s_audioBusConfig { BusConfig::InputOutput };
    static BusConfig s_noteBusConfig { BusConfig::None };

    static std::vector<const char*> s_features { "audio-effect", nullptr };
    static clap_plugin_descriptor s_descriptor{
        .clap_version = CLAP_VERSION,
        .id = "slma.gain",
        .name = "Gain",
        .vendor = "",
        .url = "",
        .manual_url = "",
        .support_url = "",
        .version = "",
        .description = "",
        .features = s_features.data()
    };

    clap_plugin_descriptor& getDescriptor() {
        return s_descriptor;
    }

    BusConfig getAudioBusConfig() noexcept {
        return s_audioBusConfig;
    }

    BusConfig getNoteBusConfig() noexcept {
        return s_noteBusConfig;
    }
    // clang-format on
} // namespace mostly_harmless
