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
        Gain
        ID "slma.gain"
        VENDOR "SLM Audio"
        FORMATS CLAP AU VST3 Standalone
        AU_BUNDLE_ID "com.yourcompany.gain"
        AU_BUNDLE_VERSION "1"
        MANUFACTURER_CODE "Exmp"
        SUBTYPE_CODE "Gpl1"
        AU_TYPE "aufx"
        FEATURES "audio-effect"
)

mostly_harmless_add_binary_data(Gain
        BINARY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources/placeholder.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/placeholder2.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/placeholder3.txt
)

target_sources(Gain PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Gain.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/GainEditor.cpp
)

target_include_directories(Gain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Gain PUBLIC magic_enum::magic_enum nlohmann_json)
