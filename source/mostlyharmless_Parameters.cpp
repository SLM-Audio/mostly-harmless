//
// Created by Syl Morrison on 21/11/2024.
//
#include <mostly_harmless/mostlyharmless_Parameters.h>
namespace mostly_harmless {
    template <marvin::FloatType SampleType>
    Parameter<SampleType>::Parameter(mostly_harmless::ParameterID parameterId_,
                                     std::string_view name_,
                                     std::string_view category_,
                                     marvin::utils::Range<SampleType> range_,
                                     SampleType defaultValue_,
                                     std::uint32_t flags_) : pid(parameterId_.pid),
                                                             name(name_),
                                                             category(category_),
                                                             range(range_),
                                                             defaultValue(defaultValue_),
                                                             flags(flags_),
                                                             value(defaultValue_) {
    }

    template <marvin::FloatType SampleType>
    Parameter<SampleType>::Parameter(std::uint32_t pid_,
                                     std::string_view name_,
                                     std::string_view category_,
                                     marvin::utils::Range<SampleType> range_,
                                     SampleType defaultValue_,
                                     std::uint32_t flags_) : pid(pid_),
                                                             name(name_),
                                                             category(category_),
                                                             range(range_),
                                                             defaultValue(defaultValue_),
                                                             flags(flags_),
                                                             value(defaultValue_) {
    }

    template struct Parameter<float>;
    template struct Parameter<double>;
} // namespace mostly_harmless