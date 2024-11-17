//
// Created by Syl Morrison on 21/10/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_SHAREDSTATECONTEXT_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_SHAREDSTATECONTEXT_H
#include <functional>
namespace mostly_harmless::core {
    /**
     * \brief Container for the context required by ISharedState's constructor.
     */
    struct SharedStateContext {

        /**
         * A lambda wrapping a call to the PluginBase's runOnMainThread function.
         */
        std::function<void(std::function<void(void)>&&)> runOnMainThread{ nullptr };

        /**
         * A lambda wrapping a call to the PluginBase's requestParamFlush function.
         */
        std::function<void(void)> requestParamFlush{ nullptr };
    };
} // namespace mostly_harmless::core
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_SHAREDSTATECONTEXT_H
