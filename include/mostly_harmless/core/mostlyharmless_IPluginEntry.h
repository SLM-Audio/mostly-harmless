//
// Created by Syl Morrison on 20/10/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_IPLUGINENTRY_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_IPLUGINENTRY_H
#include <mostly_harmless/core/mostlyharmless_ISharedState.h>
#include <mostly_harmless/core/mostlyharmless_IEngine.h>
#include <mostly_harmless/core/mostlyharmless_IEditor.h>

namespace mostly_harmless::core {
    /**
     * \brief Interface for registering the user's subclasses with the framework.
     *
     * As detailed in the FX Plugin tutorial, users must implement this, ISharedState, IEngine, and optionally IEditor, and register their subclasses with
     * the framework to create an audio plugin. These will also be created if using the provided `create.py` script.
     *
     * Handles specifying the various subclasses the framework should create for ISharedState, IEngine and IEditor.
     */
    struct IPluginEntry {
        virtual ~IPluginEntry() noexcept = default;

        /**
         * Override this to create an instance of your Shared State class, deriving from ISharedState.
         * \param context A SharedStateContext instance, to be forwarded to ISharedState's constructor.
         * \return A unique_ptr containing an instance of your SharedState class.
         */
        virtual std::unique_ptr<ISharedState> createState(SharedStateContext&& context) = 0;

        /**
         * Override this to create an instance of your Engine class, deriving from IEngine.
         * \param sharedState A pointer to the object returned from createState, to optionally pass to your IEngine subclass' constructor.
         * \return A unique_ptr containing an instance of your Engine class.
         */
        virtual std::unique_ptr<IEngine> createEngine(ISharedState* sharedState) = 0;

        /**
         * Override this to specify whether or not your plugin provides a gui.
         * \return true if you've provided a gui, false otherwise.
         */
        [[nodiscard]] virtual bool hasGui() const noexcept = 0;

        /**
         * Override this to create an instance of your Editor class, deriving from IEditor (or descendents).
         * If you've chosen not to provide a gui (and returned false from hasGui), you can return a nullptr here.
         * \param sharedState A pointer to the object returned from createState, to optionally pass to your IEditor subclass' constructor.
         * \return A unique_ptr containing an instance of your Editor class.
         */
        virtual std::unique_ptr<IEditor> createEditor(ISharedState* sharedState) = 0;
    };
} // namespace mostly_harmless::core

/**
 * \brief Macro to register your IPluginEntry subclass with the framework.
 *
 * Call this from your IPluginEntry subclass' source file.
 */
#define MH_REGISTER_PLUGIN_ENTRY(PluginEntryType)                    \
    namespace mostly_harmless::core {                                \
        std::unique_ptr<IPluginEntry> createPluginEntry() noexcept { \
            return std::make_unique<PluginEntryType>();              \
        }                                                            \
    }

#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_IPLUGINENTRY_H
