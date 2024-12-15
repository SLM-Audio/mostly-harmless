//
// Created by Syl Morrison on 15/12/2024.
//

#ifndef NIGHTSHELL_MOSTLYHARMLESS_DIRECTORIES_H
#define NIGHTSHELL_MOSTLYHARMLESS_DIRECTORIES_H

#include <filesystem>

namespace mostly_harmless::utils::directories {
    /**
     * \brief Represents a "special location type" on the user's system.
     */
    enum class DirectoryType {
        /**
         * The user desktop directory - on macOS, ~/Desktop
         */
        Desktop,
        /**
         * The user documents directory - on macOS, ~/Documents
         */
        Documents,
        /**
         * The user application data directory - on macOS, ~/Library
         */
        UserAppData,
        /**
         * The system temp directory.
         */
        Temp
    };

    /**
     * Attempts to retrieve the full path to a "special location" (to borrow JUCE's terminology) on the user's system.
     * \param type The DirectoryType to search for
     * \return an optional containing the std::filesystem::path to the requested directory if successful, nullopt otherwise.
     */
    [[nodiscard]] std::optional<std::filesystem::path> getDirectory(DirectoryType type);
} // namespace mostly_harmless::utils::directories

#endif // NIGHTSHELL_MOSTLYHARMLESS_DIRECTORIES_H
