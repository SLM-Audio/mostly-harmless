#ifndef MOSTLYHARMLESS_DESCRIPTOR_H
#define MOSTLYHARMLESS_DESCRIPTOR_H
#include <mostly_harmless/mostlyharmless_BusConfig.h>
#include <clap/plugin.h>
#include <vector>
namespace mostly_harmless {
    /**
     * Retrieve the currently set clap_plugin_descriptor details. This is populated via the mostly_harmless_add_plugin cmake function.
     */
    [[nodiscard]] clap_plugin_descriptor& getDescriptor();
    /**
     * Retrieves the current mostly_harmless::BusConfig in use for audio io.
     */
    [[nodiscard]] BusConfig getAudioBusConfig() noexcept;
    /**
     * Retrieves the current mostly_harmless::BusConfig in use for midi io.
     */
    [[nodiscard]] BusConfig getNoteBusConfig() noexcept;
} // namespace mostly_harmless
#endif