//
// Created by Syl Morrison on 11/09/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_TESTHELPERS_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_TESTHELPERS_H
#include <marvin/library/marvin_Concepts.h>
#include <marvin/library/marvin_Literals.h>
#include <marvin/containers/marvin_BufferView.h>
#include <vector>
namespace mostly_harmless::testing {
    template<marvin::FloatType SampleType, size_t N>
    requires (N > 0)
    [[nodiscard]] std::vector<SampleType> generateImpulse() {
        std::vector<SampleType> impulse(N, static_cast<SampleType>(0.0));
        impulse.front() = static_cast<SampleType>(1.0);
        return impulse;
    }

    template<marvin::FloatType SampleType, size_t NumChannels, size_t BufferSize>
    SampleType** generateImpulse() {
        using namespace marvin::literals;
        SampleType** arr = new SampleType*[2];
        for(auto channel = 0_sz; channel < NumChannels; ++channel) {
            arr[channel] = new SampleType[BufferSize];
            std::memset(arr[channel], 0, BufferSize * sizeof(SampleType));
            arr[channel][0] = static_cast<SampleType>(1);
        }
        return arr;
    }

    template<marvin::FloatType SampleType>
    bool checkBuffer(marvin::containers::BufferView<SampleType> buffer)  {
        using namespace marvin::literals;
        const auto* const* read = buffer.getArrayOfReadPointers();
        for(auto channel = 0_sz; channel < buffer.getNumChannels(); ++channel) {
            for(auto sample = 0_sz; sample < buffer.getNumSamples(); ++sample) {
                const auto x = read[channel][sample];
                if(std::isnan(x) || std::isinf(x)) {
                    return false;
                }
            }
        }
        return true;
    }
}
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_TESTHELPERS_H
