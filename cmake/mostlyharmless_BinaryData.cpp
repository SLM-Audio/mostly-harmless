// This file is auto-generated!
// clang-format off
#include "mostlyharmless_@ARG_TARGET_NAME@.h"
#include <cmrc/cmrc.hpp>
#include <system_error>
CMRC_DECLARE(@ARG_TARGET_NAME@);
namespace mostly_harmless::@ARG_TARGET_NAME@ {
    std::optional<BinaryResource> getNamedResource(const std::string& name) {
        auto fs = cmrc::@ARG_TARGET_NAME@::get_filesystem();
        try {
            auto data = fs.open(name);
            const auto size = data.end() - data.begin();
            BinaryResource res{ .data = data.begin(), .size = static_cast<size_t>(size) };
            return res;
        } catch(std::system_error& /*e*/) {
            return {};
        }
    }
}
// clang-format on