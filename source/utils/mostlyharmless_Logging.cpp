//
// Created by sylmo on 05/10/2025.
//
#include <mostly_harmless/utils/mostlyharmless_Logging.h>
#include <mostly_harmless/utils/mostlyharmless_OnScopeExit.h>

#include <utility>
#include <fstream>

namespace mostly_harmless::utils {
    Logging::Logging(std::filesystem::path logFile) : m_logFile(std::move(logFile)) {

        auto writeMode = std::ios::out;
        if(std::filesystem::exists(m_logFile)) {
            writeMode |= std::ios::app;
        }
        std::ofstream outputFile{ m_logFile, writeMode };
        outputFile << "------ START LOG ------ \n";
        outputFile.flush();
    }

    Logging::~Logging() noexcept {
        dump("------ END LOG ------ \n");
    }

    auto Logging::dump(std::string_view toDump) const noexcept -> void {
        try {
            std::ofstream outStream{ m_logFile, std::ios::app | std::ios::out };
            outStream << toDump << "\n";
            outStream.flush();
        } catch (...) {
        }
    }


} // namespace mostly_harmless::utils