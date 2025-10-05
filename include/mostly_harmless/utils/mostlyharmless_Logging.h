//
// Created by sylmo on 05/10/2025.
//

#ifndef MOSTLYHARMLESS_LOGGING_H
#define MOSTLYHARMLESS_LOGGING_H
#include <filesystem>
namespace mostly_harmless::utils {
    /***
     * \brief Helper class for logging to a file.
     */
    class Logging final {
    public:
        /**
         * Constructor - if logFile doesn't already exist, creates it. Writes a newline specifying that logging has been started, to distinguish between multiple sessions with this file
         * @param logFile The destination file for your logging output to be written to
         */
        explicit Logging(std::filesystem::path logFile);
        /**
         * Destructor - Writes a newline specifying that logging has been ended, to distinguish between multiple sessions with this file.
         */
        ~Logging() noexcept;

        /**
         * Writes some text to your file, followed by a newline.
         * @param toDump The text to dump to the log file
         */
        auto dump(std::string_view toDump) const noexcept -> void;
    private:
        std::filesystem::path m_logFile;
    };
}
#endif //MOSTLYHARMLESS_LOGGING_H
