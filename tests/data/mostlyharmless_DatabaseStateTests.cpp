//
// Created by Syl Morrison on 12/04/2025.
//
#include <mostly_harmless/utils/mostlyharmless_Directories.h>
#include <mostly_harmless/data/mostlyharmless_DatabaseState.h>
#include <mostly_harmless/data/mostlyharmless_DatabasePropertyListener.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace mostly_harmless::testing {
    template <bool ShouldSucceed>
    auto tryCreateDatabase(const std::filesystem::path& destination, const std::vector<std::pair<std::string, data::DatabaseValueVariant>>& initialValues) -> std::optional<data::DatabaseState> {
        auto databaseOpt = data::DatabaseState::tryCreate(destination, initialValues);
        if constexpr (!ShouldSucceed) {
            REQUIRE(!databaseOpt);
            return {};
        } else {
            REQUIRE(databaseOpt);
            return databaseOpt;
        }
    }

    TEST_CASE("Test DatabaseState") {
        auto tempDir = utils::directories::getDirectory(utils::directories::DirectoryType::Temp);
        if (!tempDir) {
            REQUIRE(false);
        }
        auto dbFile = *tempDir / "moha_test_db.sqlite";
        SECTION("Test Valid Location, with no initial values") {
            {
                auto databaseOpt = tryCreateDatabase<true>(dbFile, {});
                auto& database = *databaseOpt;
                REQUIRE_NOTHROW(database.set<std::string>("Hello", "World"));
                const auto retrieved = database.get<std::string>("Hello");
                REQUIRE(retrieved.has_value());
                REQUIRE(*retrieved == "World");
                REQUIRE(!database.get<int>("aaaaa"));
            }
            {
                std::vector<std::pair<std::string, data::DatabaseValueVariant>> initialValues;
                initialValues.emplace_back("IntTest", 10);
                initialValues.emplace_back("DoubleTest", 15.0);
                auto databaseOpt = tryCreateDatabase<true>(dbFile, initialValues);
                auto& database = *databaseOpt;
                auto retrievedDouble = database.get<double>("DoubleTest");
                REQUIRE(retrievedDouble.has_value());
                REQUIRE_THAT(retrievedDouble.value(), Catch::Matchers::WithinRel(15.0));
                database.set<double>("DoubleTest", 20.0);
                retrievedDouble = database.get<double>("DoubleTest");
                REQUIRE(retrievedDouble.has_value());
                REQUIRE_THAT(retrievedDouble.value(), Catch::Matchers::WithinRel(20.0));
                auto database2Opt = tryCreateDatabase<true>(dbFile, initialValues);
                auto& database2 = *database2Opt;
                retrievedDouble = database2.get<double>("DoubleTest");
                REQUIRE(retrievedDouble.has_value());
                REQUIRE_THAT(retrievedDouble.value(), Catch::Matchers::WithinRel(20.0));
            }

            std::filesystem::remove(dbFile);
        }

//        SECTION("Test Invalid Location") {
//            tryCreateDatabase<false>("INVALID LOCATION", {});
//        }

        SECTION("Test In-Memory") {
            tryCreateDatabase<true>(":memory:", {});
        }

        SECTION("Test Duplicate") {
            {
                auto connectionAOpt = tryCreateDatabase<true>(dbFile, { { "test", "aaaa" } });
                auto& databaseA = *connectionAOpt;
                auto connectionBOpt = databaseA.duplicate();
                REQUIRE(connectionBOpt.has_value());
                auto& databaseB = *connectionBOpt;
                auto retrievalOpt = databaseB.get<std::string>("test");
                REQUIRE(retrievalOpt.has_value());
                REQUIRE(*retrievalOpt == "aaaa");
            }
            std::filesystem::remove(dbFile);
        }

        SECTION("Test DatabasePropertyListener") {
            for (auto i = 0; i < 100; ++i) {
                {
                    auto databaseOpt = tryCreateDatabase<true>(dbFile, { { "test", 0 } });
                    auto& database = *databaseOpt;
                    std::atomic<bool> wasPropertyChanged{ false };
                    std::atomic<int> newValue{ 0 };
                    {
                        auto onPropertyChanged = [&wasPropertyChanged, &newValue](const auto& x) -> void {
                            wasPropertyChanged.store(true);
                            newValue = x;
                        };
                        auto listener = data::DatabasePropertyListener<int>::tryCreate(database, "test", 1, std::move(onPropertyChanged));
                        REQUIRE(listener);
                        database.set<int>("test", 10);
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                        REQUIRE(wasPropertyChanged.load());
                        database.set<int>("test", 20);
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                        REQUIRE(newValue == 20);
                        database.set<int>("test", 30);
                    }
                }
                std::filesystem::remove(dbFile);
            }
        }
    }

} // namespace mostly_harmless::testing