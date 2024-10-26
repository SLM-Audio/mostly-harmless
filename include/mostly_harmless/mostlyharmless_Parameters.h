#ifndef MOSTLYHARMLESS_PARAMETERS_H
#define MOSTLYHARMLESS_PARAMETERS_H
#include <clap/ext/params.h>
#include <marvin/utils/marvin_Range.h>
#include <functional>
namespace mostly_harmless {
    /**
     * \brief Container class for a single parameter.
     */
    template <marvin::FloatType SampleType>
    struct Parameter final {
        /**
         * A unique id for this parameter.
         */
        std::uint32_t pid;
        /**
         * The parameter's user readable name.
         */
        std::string name;
        /**
         * The parameter's category, for display purposes.
         */
        std::string category;
        /**
         * The parameter's min and max values.
         */
        marvin::utils::Range<SampleType> range;
        /**
         * The parameter's default value.
         */
        SampleType defaultValue;
        //
        /**
         * The parameter's clap param flags - see [here](https://github.com/free-audio/clap/blob/c2c1dea9f72b38e23e285a9a10a5d936920895ab/include/clap/ext/params.h#L133) for more details.
         */
        std::uint32_t flags;
        /**
         * Am optional user-provided function for custom value displaying.
         */
        std::function<std::string(const Parameter&, double)> valueToString{ nullptr };
        /**
         * The parameter's internal value.
         */
        SampleType value{ defaultValue };
    };
} // namespace mostly_harmless
#endif