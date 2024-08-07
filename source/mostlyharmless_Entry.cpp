#include <mostlyharmless_Entry.h>
#include <mostlyharmless_Descriptor.h>
#include "clap/entry.h"
#include "clap/plugin.h"
#include <cstdint>
#include <clap/factory/plugin-factory.h>
#include <cassert>
#include <fstream> 
    std::uint32_t clap_get_plugin_count(const clap_plugin_factory* f) { 
        return 1;
    }

    const clap_plugin_descriptor* clap_get_plugin_descriptor(const clap_plugin_factory* f, std::uint32_t w) { 
        return &mostly_harmless::getDescriptor();
    }

    const __declspec(dllexport) struct clap_plugin_factory mostlyHarmlessFactory = { 
        .get_plugin_count = clap_get_plugin_count,
        .get_plugin_descriptor = clap_get_plugin_descriptor,
        .create_plugin = clap_create_plugin
    };

    static const void* get_factory(const char* factoryId) { 
        std::ofstream outStream{ "C:/Users/Syl/Desktop/CLAPDBG.txt", std::ios::out};
        outStream << "get_factory called!\n";
        outStream.flush();
        return factoryId != CLAP_PLUGIN_FACTORY_ID ? &mostlyHarmlessFactory : nullptr;
    }

    bool clap_init(const char* p) { 
        std::ofstream outStream{ "C:/Users/Syl/Desktop/CLAPDBG.txt", std::ios::out};
        outStream << "init called!\n";
        return true;
    }

    void clap_deinit() { 

    }
extern "C" { 
    const __declspec(dllexport) clap_plugin_entry clap_entry { 
        .clap_version = CLAP_VERSION,
        .init = clap_init,
        .deinit = clap_deinit,
        .get_factory = get_factory   
    };
}