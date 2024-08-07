#ifndef MOSTLYHARMLESS_DESCRIPTOR_H
#define MOSTLYHARMLESS_DESCRIPTOR_H
#include <clap/plugin.h>
#include <vector>
namespace mostly_harmless { 
    [[nodiscard]] clap_plugin_descriptor& getDescriptor();
}
#endif