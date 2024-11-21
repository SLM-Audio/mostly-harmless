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
         * Creates a Parameter, setting the pid field based on a ParameterID instance. We'd recommend using this constructor, but the other overload is available if you want custom logic for your PIDs.
         * \param parameterId_ A ParameterID constructed with the unique internal name for your parameter. If you want to remain backwards compatible, make sure these strings do not change, and are unique.
         * \param name_ The parameter's user-facing name.
         * \param category_ The parameter's category - a clap feature supported by some hosts, to organise parameters in trees as opposed to an overwhelming flat list.
         * \param range_ The parameter's range.
         * \param defaultValue_ The parameter's default value.
         * \param flags_ The parameter's clap param flags - see [here](https://github.com/free-audio/clap/blob/c2c1dea9f72b38e23e285a9a10a5d936920895ab/include/clap/ext/params.h#L133) for more details.
         */
        Parameter(ParameterID parameterId_, std::string_view name_, std::string_view category_, marvin::utils::Range<SampleType> range_, SampleType defaultValue_, std::uint32_t flags_);

        /**
         * Creates a Parameter, setting the pid directly from the pid_ arg. Most of the time, the other overload is preferable here, but if you really want to manage your own PIDs, use this one.
         * \param pid_ A unique uint32_t pid for your parameter. For backwards compatability, this must stay fixed, and must be unique.
         * \param name_ The parameter's user-facing name.
         * \param category_ The parameter's category - a clap feature supported by some hosts, to organise parameters in trees as opposed to an overwhelming flat list.
         * \param range_ The parameter's range.
         * \param defaultValue_ The parameter's default value.
         * \param flags_ The parameter's clap param flags - see [here](https://github.com/free-audio/clap/blob/c2c1dea9f72b38e23e285a9a10a5d936920895ab/include/clap/ext/params.h#L133) for more details.
         */
        Parameter(std::uint32_t pid_, std::string_view name_, std::string_view category_, marvin::utils::Range<SampleType> range_, SampleType defaultValue_, std::uint32_t flags_);

        /**
         * The Parameter's internal ID, used for lookups, etc. CLAP requires that this stay fixed to avoid compatability issues, and it *must* be unique.
         */
        std::uint32_t pid;

        /**
         * The parameter's user-facing name.
         */
        std::string name;

        /**
         * The parameter's category - a clap feature supported by some hosts, to organise parameters in trees as opposed to an overwhelming flat list.
         */
        std::string category;

        /**
         * The parameter's range.
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