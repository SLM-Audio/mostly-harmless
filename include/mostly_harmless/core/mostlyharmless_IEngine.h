//
// Created by Syl Morrison on 20/10/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_IENGINE_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_IENGINE_H
#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
#include <mostly_harmless/mostlyharmless_TransportState.h>
#include <mostly_harmless/mostlyharmless_Concepts.h>
#include <marvin/containers/marvin_BufferView.h>
#include <marvin/library/marvin_Concepts.h>
namespace mostly_harmless::core {

    /**
     * \brief Container for context passed to IEngine::initialise.
     */
    struct InitContext {
        /**
         * The current host sample rate.
         */
        double sampleRate;
        /**
         * The minimum buffer size the host can pass to the plugin.
         */
        std::uint32_t minBufferSize;
        /**
         * The maximum buffer size the host can pass to the plugin.
         */
        std::uint32_t maxBufferSize;
    };

    /**
     * \brief Container for context passed to IEngine::process.
     */
    struct ProcessContext {
        /**
         * A non owning view into the current buffer slice, passed by runBlockDispatch.
         */
        marvin::containers::BufferView<float> buffer;
        /**
         * If provided by the host, contains the last reported transport info. std::nullopt otherwise.
         */
        std::optional<mostly_harmless::TransportState> transportState;
    };

    /**
     * \brief Interface for the plugin's audio processing functionality.
     *
     * As detailed in the FX Plugin tutorial, users must implement this, ISharedState, IPluginEntry, and optionally IEditor, and register their subclasses with
     * the framework to create an audio plugin. These will also be created if using the provided `create.py` script.
     *
     * Encapsulates all things audio-thread related, and all functions either run on the audio thread, or are guaranteed not to be called while the audio thread is running.
     *
     * You probably want your subclass to take a pointer to your subclass of ISharedState, for access to parameters, and the procToGui / guiToProc queues.
     */
    class IEngine {
    public:
        /**
         * Virtual destructor.
         */
        virtual ~IEngine() noexcept = default;

        /**
         * Called when the host is preparing to play. Any sample-rate-dependent resizing etc should be done here.
         * Will probably not be called on the audio thread, but is guaranteed to not be called concurrently with the audio thread.
         * \param context An InitContext containing various settings provided by the host.
         */
        virtual void initialise(InitContext context) noexcept = 0;

        /**
         * The main audio callback function - any DSP processing should stem from this function.
         * Called on the audio thread.
         * \param context A ProcessContext containing a non-owning view into the current buffer region, and an optional for transport info.
         */
        virtual void process(ProcessContext context) noexcept = 0;

        /**
         * Called to cleanup any resources that require cleaning up - use this to clear delay lines / temp buffers, etc etc.
         */
        virtual void reset() noexcept = 0;

        /**
         * Called if the plugin receives a midi (or clap) note on event - not pure virtual, as this function isn't relevant if you haven't requested midi functionality.
         * Called on the audio thread, in response to a noteOn event.
         * \param portIndex The clap port index the event originated from.
         * \param channel The midi channel the event was passed to
         * \param note The 0-127 midi note that was turned on.
         * \param velocity The 0-1 velocity of the note event
         */
        virtual void handleNoteOn([[maybe_unused]] std::uint16_t portIndex, [[maybe_unused]] std::uint8_t channel, [[maybe_unused]] std::uint8_t note, [[maybe_unused]] double velocity) {}

        /**
         * Called if the plugin receives a midi (or clap) note off event - not pure virtual, as this function isn't relevant if you haven't requested midi functionality.
         * Called on the audio thread, in response to a noteOff event.
         * \param portIndex The clap port index the event originated from.
         * \param channel The midi channel the event was passed to
         * \param note The 0-127 midi note that was turned off.
         * \param velocity The 0-1 velocity of the note event
         */
        virtual void handleNoteOff([[maybe_unused]] std::uint16_t portIndex, [[maybe_unused]] std::uint8_t channel, [[maybe_unused]] std::uint8_t note, [[maybe_unused]] double velocity) {}
    };
} // namespace mostly_harmless::core
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_IENGINE_H
