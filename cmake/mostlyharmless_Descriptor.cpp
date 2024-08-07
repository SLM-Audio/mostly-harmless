#include <mostlyharmless_Descriptor.h>
namespace mostly_harmless { 
    static std::vector<const char*> s_features { @PLUGIN_FEATURES@, nullptr};
    static clap_plugin_descriptor s_descriptor { 
        .clap_version = CLAP_VERSION,
        .id = "@PLUGIN_ID@",
        .name = "@PLUGIN_NAME@",
        .vendor = "@PLUGIN_VENDOR@",
        .url = "@PLUGIN_URL@",
        .manual_url = "@PLUGIN_MANUAL_URL@",
        .support_url = "@PLUGIN_SUPPORT_URL@",
        .version = "@PLUGIN_VERSION@",
        .description = "@PLUGIN_DESCRIPTION@",
        .features = s_features.data()
    };

    clap_plugin_descriptor& getDescriptor() { 
        return s_descriptor;
    }
}