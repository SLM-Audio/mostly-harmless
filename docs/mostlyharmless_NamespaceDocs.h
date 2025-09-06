//
// Created by Syl on 11/08/2024.
//

#ifndef MOSTLYHARMLESS_MOSTLYHARMLESS_NAMESPACEDOCS_H
#define MOSTLYHARMLESS_MOSTLYHARMLESS_NAMESPACEDOCS_H
/**
 * \brief The top-level namespace, contains all things plugin-related
 */
namespace mostly_harmless {
    /**
     * \brief Framework-Developer-Only documentation, for specific mechanisms within the framework
     */
     namespace internal {

     }
    /**
     * \brief Contains core functionality of the framework
     */
    namespace core {

    }
    /**
     * \brief Contains GUI related classes & functions
     */
    namespace gui {
        /**
         * \brief Contains helper functions for gui handling
         */
        namespace helpers {
            /**
             * \brief macOS specific gui helper functions
             */
            namespace macos {

            }
        } // namespace helpers
    } // namespace gui
    /**
     * \brief Contains general purpose utility classes & functions
     */
    namespace utils {
        /**
         * \brief Contains filesystem helpers
         */
        namespace directories {

        }
        /**
        \brief Serialisation and Deserialisation utils for FlatBuffers
        */
        namespace flat_buffers {

        }

    }
    /**
     * \brief Contains classes and functions related to data management
     */
    namespace data {

    }
    /**
     * \brief Contains classes and functions related to events
     */
    namespace events {

    }

    /**
     * \brief Contains audio helpers and utilities
     */
    namespace audio {

    }
} // namespace mostly_harmless
#endif // MOSTLYHARMLESS_MOSTLYHARMLESS_NAMESPACEDOCS_H
