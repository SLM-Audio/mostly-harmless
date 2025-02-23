set(CMAKE_CXX_STANDARD 20)

FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
        GIT_SHALLOW ON
)

FetchContent_MakeAvailable(json)
mostly_harmless_add_plugin(
        synth
        ID "your_company.synth"
        VENDOR "Your Company"
        FORMATS CLAP AU VST3 Standalone
        AU_BUNDLE_ID "com.your_company.synth"
        AU_BUNDLE_VERSION "1"
        MANUFACTURER_CODE "Exmp"
        SUBTYPE_CODE "Mhp3"
        AU_TYPE "aumu"
        FEATURES "instrument"
)

# TODO: Setup your frontend code here - see examples/delay for a React example
set(SYNTH_GUI_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/synth-gui)
file(GLOB_RECURSE SYNTH_GUI_SOURCES "${SYNTH_GUI_ROOT}/*" FORCE)
set(SYNTH_GUI_OUTPUT
        ${PROJECT_BINARY_DIR}/synth-gui/index.html
        ${PROJECT_BINARY_DIR}/synth-gui/index.css
        ${PROJECT_BINARY_DIR}/synth-gui/index.js
)

if (WIN32)
    add_custom_command(
            OUTPUT ${SYNTH_GUI_OUTPUT}
            COMMAND npm run build
            COMMAND powershell Remove-Item -Path ${PROJECT_BINARY_DIR}/synth-gui -Recurse -Force
            COMMAND powershell Copy-Item -Force -Recurse "./dist" ${PROJECT_BINARY_DIR}/synth-gui
            WORKING_DIRECTORY ${SYNTH_GUI_ROOT}
            DEPENDS ${SYNTH_GUI_SOURCES}
            USES_TERMINAL
    )
else ()
    add_custom_command(
            OUTPUT ${SYNTH_GUI_OUTPUT}
            COMMAND npm run build
            COMMAND rm -rf ${PROJECT_BINARY_DIR}/synth-gui
            COMMAND cp -r ./dist ${PROJECT_BINARY_DIR}/synth-gui
            WORKING_DIRECTORY ${SYNTH_GUI_ROOT}
            DEPENDS ${SYNTH_GUI_SOURCES}
            USES_TERMINAL
    )
endif ()

add_custom_target(
        synth-vite
        ALL
        DEPENDS ${SYNTH_GUI_OUTPUT}
)

mostly_harmless_add_binary_data(synth
        TARGET_NAME SynthWebResources
        ROOT ${PROJECT_BINARY_DIR}/synth-gui
        BINARY_SOURCES ${SYNTH_GUI_OUTPUT}
)
add_dependencies(GainWebResources synth-vite)


target_sources(synth PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source/synth_Entry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/synth_SharedState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/synth_Engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/synth_Editor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/synth_Voice.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/synth_Envelope.cpp
)

target_include_directories(synth PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)
target_compile_definitions(synth PUBLIC HOT_RELOAD) # Comment this out to serve from RAM