#include <mostly_harmless/mostlyharmless_Descriptor.h>
#include "clap/entry.h"
#include "clap/plugin.h"
#include <cstdint>
#include <clap/factory/plugin-factory.h>

namespace mostly_harmless::entry {
    const clap_plugin* clap_create_plugin(const clap_plugin_factory* f, const clap_host* h, const char* id);

    std::uint32_t clap_get_plugin_count(const clap_plugin_factory* /*f*/) {
        return 1;
    }

    const clap_plugin_descriptor* clap_get_plugin_descriptor(const clap_plugin_factory* /*f*/, std::uint32_t /*w*/) {
        return &mostly_harmless::getDescriptor();
    }

    // const __declspec(dllexport) struct clap_plugin_factory mostlyHarmlessFactory = {
    const CLAP_EXPORT struct clap_plugin_factory mostlyHarmlessFactory = {
        .get_plugin_count = mostly_harmless::entry::clap_get_plugin_count,
        .get_plugin_descriptor = mostly_harmless::entry::clap_get_plugin_descriptor,
        .create_plugin = mostly_harmless::entry::clap_create_plugin
    };

    static const void* get_factory(const char* factoryId) {
        return strcmp(factoryId, CLAP_PLUGIN_FACTORY_ID) == 0 ? &mostlyHarmlessFactory : nullptr;
    }

    bool clap_init(const char* /*p*/) {
        return true;
    }

    void clap_deinit() {
    }

} // namespace mostly_harmless::entry
extern "C" {
// const __declspec(dllexport) clap_plugin_entry clap_entry{
const CLAP_EXPORT clap_plugin_entry clap_entry{
    .clap_version = CLAP_VERSION,
    .init = mostly_harmless::entry::clap_init,
    .deinit = mostly_harmless::entry::clap_deinit,
    .get_factory = mostly_harmless::entry::get_factory
};
}