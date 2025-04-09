//
// Created by Syl Morrison on 09/04/2025.
//

#ifndef MOSTLYHARMLESS_DATABASESTATE_H
#define MOSTLYHARMLESS_DATABASESTATE_H

#include <mostly_harmless/mostlyharmless_Concepts.h>
#include <mostly_harmless/utils/mostlyharmless_Macros.h>
#include <sqlite3.h>
#include <fmt/core.h>
#include <string>
#include <filesystem>

namespace mostly_harmless::data {

    /**
     * @private
     */
    namespace {
        enum Index : size_t {
            TextIndex = 1,
            BoolIndex = 2,
            IntIndex = 3,
            FloatIndex = 4,
            DoubleIndex = 5
        };


        template <DatabaseStorageType T>
        auto databaseQueryCallback(void* ud, int count, char** data, char** /*columns*/) -> int {
            auto* result = static_cast<std::optional<T>*>(ud);
            if (count != 6) {
                return SQLITE_FAIL;
            }
            if constexpr (std::same_as<T, std::string>) {
                *result = data[Index::TextIndex];
            } else if constexpr (std::same_as<T, bool>) {
                *result = static_cast<T>(std::stoi(data[Index::BoolIndex]));
            } else if constexpr (std::is_integral_v<T>) {
                *result = static_cast<T>(std::stoi(data[Index::IntIndex]));
            } else if constexpr (std::same_as<T, float>) {
                *result = std::stof(data[Index::FloatIndex]);
            } else if constexpr (std::same_as<T, double>) {
                *result = std::stod(data[Index::DoubleIndex]);
            }
            return SQLITE_OK;
        }
    } // namespace

    /**
     * \brief Represents a connection to a sqlite database.
     *
     * The rationale behind this class' existence is to provide a unified "standard" layout for a plugin's global state - things like settings, statistics or anything else you might need to share between all instances.
     * It doesn't directly support any kind of IPC notifiers, and is assumed that if a user wants synchronisation between instances using this class, they'll either poll the values they need on a timer, or set up their own
     * filewatching mechanism.
     *
     * The database schema itself is fairly generic, a single `DATA` table, with `NAME`, `TEXT_VALUE`, `BOOL_VALUE`, `INT_VALUE`, `FLOAT_VALUE` and `DOUBLE_VALUE fields.
     * get and set are templated, and will get or set the field associated with the given template type - so it's important to ensure you retrieve the correct value type for a field you've previously set.
     * The fields default to `""`, `false`, `0`, `0.0f` and `0.0` respectively.
     * The connection is set to WAL mode, to allow concurrent access.
     */
    class DatabaseState final {
    public:
        /**
         * On construction, will attempt to open the database passed to location, and establish a connection to it if it exists.
         * If it doesn't exist, creates the database, and a table to store user data in.
         *
         * \param location A path to the database to create or open.
         */
        explicit DatabaseState(const std::filesystem::path& location) {
            const auto checkResult = [](int response) -> void {
                if (response != SQLITE_OK) {
                    throw std::exception{};
                }
            };
            try {
                auto resultCode = sqlite3_open(location.string().c_str(), &m_databaseHandle);
                checkResult(resultCode);
                const std::string enableWalCommand{ "PRAGMA journal_mode=WAL" };
                resultCode = sqlite3_exec(m_databaseHandle, enableWalCommand.c_str(), nullptr, nullptr, nullptr);
                checkResult(resultCode);
                const std::string command{
                    "CREATE TABLE IF NOT EXISTS DATA (NAME text UNIQUE, TEXT_VALUE text, BOOL_VALUE bool, INT_VALUE int, FLOAT_VALUE float, DOUBLE_VALUE double);"
                };
                resultCode = sqlite3_exec(m_databaseHandle, command.c_str(), nullptr, nullptr, nullptr);
                checkResult(resultCode);
            } catch (...) {
                MH_LOG(sqlite3_errmsg(m_databaseHandle));
                assert(false);
            }
        }

        /**
         * @private
         */
        ~DatabaseState() noexcept {
            sqlite3_close(m_databaseHandle);
        }

        /**
         * Sets the value of a database entry with a matching name. If not found, will first create the entry.
         * @tparam T A std::string, bool, any int, float, or double
         * @param name The name of the field to set.
         * @param toSet The value to set.
         */
        template <DatabaseStorageType T>
        auto set(std::string_view name, T&& toSet) -> void {
            struct Properties {
                std::string textValue{};
                bool boolValue{ false };
                int intValue{ 0 };
                float floatValue{ 0.0f };
                double doubleValue{ 0.0 };
            } props;

            std::string updateStr;
            if constexpr (std::same_as<T, std::string>) {
                props.textValue = toSet;
                updateStr = fmt::format("TEXT_VALUE = \"{}\"", props.textValue);
            } else if constexpr (std::same_as<T, bool>) {
                props.boolValue = toSet;
                updateStr = fmt::format("BOOL_VALUE = {}", props.boolValue);
            } else if constexpr (std::is_integral_v<T>) {
                props.intValue = static_cast<int>(toSet);
                updateStr = fmt::format("INT_VALUE = {}", props.intValue);
            } else if constexpr (std::same_as<T, float>) {
                props.floatValue = toSet;
                updateStr = fmt::format("FLOAT_VALUE = {}", props.floatValue);
            } else if constexpr (std::same_as<T, double>) {
                props.doubleValue = toSet;
                updateStr = fmt::format("DOUBLE_VALUE = {}", props.doubleValue);
            }

            std::stringstream sstream;
            sstream << "INSERT INTO DATA (NAME, TEXT_VALUE, BOOL_VALUE, INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE)\n";
            sstream << "    VALUES ( \"" << name << "\", \"" << props.textValue << "\", " << props.boolValue << "," << props.intValue << ", " << props.floatValue << ", " << props.doubleValue << " )\n";
            sstream << "    ON CONFLICT(NAME) DO UPDATE SET " << updateStr << ";";
            const auto execResult = sqlite3_exec(m_databaseHandle, sstream.str().c_str(), nullptr, nullptr, nullptr);
            if (execResult != SQLITE_OK) {
                MH_LOG(sqlite3_errmsg(m_databaseHandle));
                assert(false);
            }
        }

        /**
         * Attempts to retrieve the value of a named field in the database.
         * @tparam T A std::string, bool, any int, float, or double.
         * @param name The name of the database field to retrieve
         * @return The value of the retrieved database field if found, nullopt otherwise.
         */
        template <DatabaseStorageType T>
        [[nodiscard]] auto get(std::string_view name) -> std::optional<T> {
            std::optional<T> result{};
            const auto execStr = fmt::format("SELECT * FROM DATA WHERE NAME = \"{}\"", name);
            const auto getRes = sqlite3_exec(m_databaseHandle,
                                             execStr.c_str(),
                                             databaseQueryCallback<T>,
                                             static_cast<void*>(&result),
                                             nullptr);
            if (getRes != SQLITE_OK) {
                MH_LOG(sqlite3_errmsg(m_databaseHandle));
                assert(false);
            }
            return result;
        }

    private:
        sqlite3* m_databaseHandle{ nullptr };
    };


} // namespace mostly_harmless::data

#endif // MOSTLYHARMLESS_DATABASESTATE_H
