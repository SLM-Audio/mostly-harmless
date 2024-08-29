// This file is auto-generated!

// clang-format off
#ifndef MOSTLYHARMLESS_@ARG_TARGET_NAME@_H
#define MOSTLYHARMLESS_@ARG_TARGET_NAME@_H
#include <optional>
#include <string>
namespace mostly_harmless::@ARG_TARGET_NAME@ {
    struct BinaryResource final {
        const char* data;
        size_t size;
    };
    [[nodiscard]] std::optional<BinaryResource> getNamedResource(const std::string& name);
}

#endif
// clang-format on