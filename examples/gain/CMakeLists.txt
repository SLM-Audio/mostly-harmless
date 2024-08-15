set(CMAKE_CXX_STANDARD 20)
# set(ENABLE_EXPORTS FALSE)
FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG master
        GIT_SHALLOW ON)
FetchContent_MakeAvailable(magic_enum)
mostly_harmless_add_plugin(
        ID "slma.gain"
        NAME "Gain"
        SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/Gain.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/GainEditor.cpp
        FEATURES "audio-effect"
)

target_include_directories(Gain_SharedCode PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Gain_SharedCode PUBLIC magic_enum::magic_enum nlohmann_json)
