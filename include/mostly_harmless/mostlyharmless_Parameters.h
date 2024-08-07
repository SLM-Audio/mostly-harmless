#ifndef MOSTLYHARMLESS_PARAMETERS_H
#define MOSTLYHARMLESS_PARAMETERS_H
#include <marvin/utils/marvin_Range.h>
#include <functional>
namespace mostly_harmless {
    template <marvin::FloatType SampleType>
    struct Parameter final {
        std::uint32_t pid;
        std::string name;
        std::string category;
        marvin::utils::Range<SampleType> range;
        SampleType defaultValue;
        // https://github.com/free-audio/clap/blob/c2c1dea9f72b38e23e285a9a10a5d936920895ab/include/clap/ext/params.h#L133
        std::uint32_t flags;
        std::function<std::string(const Parameter&, double)> valueToString{ nullptr };
        SampleType value{ defaultValue };
    };
} // namespace mostly_harmless
#endif