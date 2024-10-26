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
        Gain2
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

set(GAIN_GUI_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/gain2-gui)
file(GLOB_RECURSE GAIN_GUI_SOURCES "${GAIN_GUI_ROOT}/*" FORCE)
set(GAIN_GUI_OUTPUT
        ${PROJECT_BINARY_DIR}/gain2-gui/index.html
        ${PROJECT_BINARY_DIR}/gain2-gui/index.css
        ${PROJECT_BINARY_DIR}/gain2-gui/index.js
)
if (WIN32)
    add_custom_command(
            OUTPUT ${GAIN_GUI_OUTPUT}
            COMMAND npm run build
            COMMAND powershell Remove-Item -Path ${PROJECT_BINARY_DIR}/gain2-gui -Recurse -Force
            COMMAND powershell Copy-Item -Force -Recurse "./dist" ${PROJECT_BINARY_DIR}/gain2-gui
            WORKING_DIRECTORY ${GAIN_GUI_ROOT}
            DEPENDS ${GAIN_GUI_SOURCES}
            USES_TERMINAL
    )
else ()
    add_custom_command(
            OUTPUT ${GAIN_GUI_OUTPUT}
            COMMAND npm run build
            COMMAND rm -rf ${PROJECT_BINARY_DIR}/gain2-gui
            COMMAND cp -r ./dist ${PROJECT_BINARY_DIR}/gain2-gui
            WORKING_DIRECTORY ${GAIN_GUI_ROOT}
            DEPENDS ${GAIN_GUI_SOURCES}
            USES_TERMINAL
    )
endif ()

add_custom_target(
        gain2-vite
        ALL
        DEPENDS ${GAIN_GUI_OUTPUT}
)
mostly_harmless_add_binary_data(Gain2
        TARGET_NAME Gain2WebResources
        ROOT ${PROJECT_BINARY_DIR}/gain2-gui
        BINARY_SOURCES ${GAIN_GUI_OUTPUT}
)
add_dependencies(Gain2WebResources gain2-vite)

target_sources(Gain2 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gain2_PluginEntry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gain2_SharedState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gain2_Engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gain2_Editor.cpp
)

target_include_directories(Gain2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Gain2 PUBLIC magic_enum::magic_enum nlohmann_json)
target_compile_definitions(Gain2 PUBLIC GAIN_HOT_RELOAD=1)
