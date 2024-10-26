//
// Created by Syl Morrison on 21/10/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_SHAREDSTATECONTEXT_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_SHAREDSTATECONTEXT_H
#include <functional>
namespace mostly_harmless::core {
    struct SharedStateContext {
        std::function<void(std::function<void(void)>&&)> runOnMainThread{ nullptr };
        std::function<void(void)> requestParamFlush{ nullptr };
    };
} // namespace mostly_harmless::core
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_SHAREDSTATECONTEXT_H
