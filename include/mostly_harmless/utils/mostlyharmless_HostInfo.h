//
// Created by Syl Morrison on 25/01/2026.
//

#ifndef GLEO_MOSTLYHARMLESS_HOSTINFO_H
#define GLEO_MOSTLYHARMLESS_HOSTINFO_H
#include <string>
namespace mostly_harmless::utils {
    /**
     * \brief Contains some strings to help identify the host currently running your plugin.
     */
    struct HostInfoView {
        std::string_view hostName;
        std::string_view vendorName;
        std::string_view version;
    };

    /**
     * @private
     */
    struct HostInfo {
        std::string hostName;
        std::string vendorName;
        std::string version;
        auto view() const -> HostInfoView {
            return { .hostName = hostName, .vendorName = vendorName, .version = version };
        }
    };

} // namespace mostly_harmless::utils
#endif // GLEO_MOSTLYHARMLESS_HOSTINFO_H
