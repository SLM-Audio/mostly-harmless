//
// Created by Syl on 12/08/2024.
//
#include <mostly_harmless/mostlyharmless_Plugin.h>
namespace mostly_harmless::entry {
    const clap_plugin* clap_create_plugin(const clap_plugin_factory* /*f*/, const clap_host* /*h*/, const char* /*id*/) {
        return nullptr;
    }
}