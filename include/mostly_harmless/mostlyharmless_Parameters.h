#ifndef MOSTLYHARMLESS_PARAMETERS_H
#define MOSTLYHARMLESS_PARAMETERS_H
#include <clap/ext/params.h>
#include <mostly_harmless/utils/mostlyharmless_Hash.h>
#include <marvin/utils/marvin_Range.h>
#include <functional>
namespace mostly_harmless {

    /**
     * \brief Convenience class for generating a parameter id.
     *
     * Generates a hash for the given string, to be used as a unique parameter id when registering the parameters in the framework. Note that this hinges on your internalNames for each parameter
     * being unique - if they're not, all hell will break loose
     */
    struct ParameterID {
        /**
         * Creates a ParameterID from a given string view. Generates the hash for the string at construction, and stores as a member.
         * \param internalName The name to hash. Again, when using this to create params, make sure this is unique!
         */
        explicit ParameterID(std::string_view internalName) : pid(mostly_harmless::utils::hash<std::uint32_t>(internalName)) {
        }

        /**
         * Converts the PID to a string.
         * \return std::to_string on the pid member.
         */
        [[nodiscard]] std::string toString() const noexcept {
            return std::to_string(pid);
        }

        /**
         * The hashed representation of the argument passed to the constructor.
         */
        const std::uint32_t pid;
    };

    /**
     * \brief Container class for a single parameter.
     */
    template <marvin::FloatType SampleType>
    struct Parameter final {

        /**
         * A ParameterID for this parameter - the name passed to its constuctor should be unique, and to maintain backwards compatability with different versions of your plugin,
         * must not change.
         */
        ParameterID parameterId;

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

        /**
         * The parameter's clap param flags - see [here](https://github.com/free-audio/clap/blob/c2c1dea9f72b38e23e285a9a10a5d936920895ab/include/clap/ext/params.h#L133) for more details.
         */
        std::uint32_t flags;

        /**
         * An optional user-provided function for custom value displaying.
         */
        std::function<std::string(const Parameter&, double)> valueToString{ nullptr };

        /**
         * The parameter's internal value.
         */
        SampleType value{ defaultValue };
    };
} // namespace mostly_harmless
#endif