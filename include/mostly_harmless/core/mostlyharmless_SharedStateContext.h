//
// Created by Syl Morrison on 21/10/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_SHAREDSTATECONTEXT_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_SHAREDSTATECONTEXT_H
#include <mostly_harmless/utils/mostlyharmless_HostInfo.h>
#include <functional>
namespace mostly_harmless::core {
    /**
     * \brief Container for the context required by ISharedState's constructor.
     */
    struct SharedStateContext {

        /**
         * A non owning view into the clap_host provided strings identifying the current host.
         */
        utils::HostInfoView hostInfo;
        /**
         * A lambda wrapping a call to the PluginBase's runOnMainThread function.
         */
        std::function<void(std::function<void(void)>&&)> runOnMainThread{ nullptr };

        /**
         * A lambda wrapping a call to the PluginBase's requestParamFlush function.
         */
        std::function<void(void)> requestParamFlush{ nullptr };

        /**
         * Call this to request that the host rescans the param values, to keep them up to date.
         */
        std::function<void(void)> requestParamRescan{ nullptr };

        /**
         * Call this to ask the host to resize your editor to the specified dimensions.
         * It's up to the host *when* it does this, and doesn't block.
         * It's *technically* fine to call this from any thread, but obviously it doesn't really make sense to do from the audio thread...
         * Args are width and height, and return value is whether the host acknowledged your resize request.
         */
        std::function<bool(std::uint32_t, std::uint32_t)> requestGuiResize{ nullptr };
    };
} // namespace mostly_harmless::core
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_SHAREDSTATECONTEXT_H
