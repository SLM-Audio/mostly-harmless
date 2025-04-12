//
// Created by Syl Morrison on 12/04/2025.
//
#include <mostly_harmless/utils/mostlyharmless_Directories.h>
#include <mostly_harmless/data/mostlyharmless_DatabaseState.h>
#include <catch2/catch_test_macros.hpp>

namespace mostly_harmless::testing {
    template <bool ShouldSucceed>
    auto tryCreateDatabase(const std::filesystem::path& destination, std::vector<std::pair<std::string, data::DatabaseValueVariant>>&& initialValues) {
        auto databaseOpt = data::DatabaseState::try_create(destination, std::move(initialValues));
        REQUIRE(databaseOpt.has_value() == ShouldSucceed);
        return std::move(databaseOpt);
    }

    TEST_CASE("Test DatabaseState") {
        auto tempDir = utils::directories::getDirectory(utils::directories::DirectoryType::Desktop);
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
            }
            // std::filesystem::remove(dbFile);
        }
        SECTION("Test Invalid Location") {
            tryCreateDatabase<false>("/iamthelordofthebongo", {});
        }
    }

} // namespace mostly_harmless::testing