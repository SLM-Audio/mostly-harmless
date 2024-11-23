set(CMAKE_CXX_STANDARD 20)

FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
        GIT_SHALLOW ON
)

FetchContent_MakeAvailable(json)
mostly_harmless_add_plugin(
        gain
        ID "your_company.gain"
        VENDOR "Your Company"
        FORMATS CLAP AU VST3 Standalone
        AU_BUNDLE_ID "com.your_company.gain"
        AU_BUNDLE_VERSION "1"
        MANUFACTURER_CODE "Exmp"
        SUBTYPE_CODE "Mhp1"
        AU_TYPE "aufx"
        FEATURES "audio-effect"
)

# TODO: Setup your frontend code here - see examples/delay for a React example
set(GAIN_GUI_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/gain-gui)
file(GLOB_RECURSE GAIN_GUI_SOURCES "${GAIN_GUI_ROOT}/*" FORCE)
set(GAIN_GUI_OUTPUT
        ${PROJECT_BINARY_DIR}/gain-gui/index.html
        ${PROJECT_BINARY_DIR}/gain-gui/index.css
        ${PROJECT_BINARY_DIR}/gain-gui/index.js
)

if (WIN32)
    add_custom_command(
            OUTPUT ${GAIN_GUI_OUTPUT}
            COMMAND npm run build
            COMMAND powershell Remove-Item -Path ${PROJECT_BINARY_DIR}/gain-gui -Recurse -Force
            COMMAND powershell Copy-Item -Force -Recurse "./dist" ${PROJECT_BINARY_DIR}/gain-gui
            WORKING_DIRECTORY ${GAIN_GUI_ROOT}
            DEPENDS ${GAIN_GUI_SOURCES}
            USES_TERMINAL
    )
else ()
    add_custom_command(
            OUTPUT ${GAIN_GUI_OUTPUT}
            COMMAND npm run build
            COMMAND rm -rf ${PROJECT_BINARY_DIR}/gain-gui
            COMMAND cp -r ./dist ${PROJECT_BINARY_DIR}/gain-gui
            WORKING_DIRECTORY ${GAIN_GUI_ROOT}
            DEPENDS ${GAIN_GUI_SOURCES}
            USES_TERMINAL
    )
endif ()

add_custom_target(
        gain-vite
        ALL
        DEPENDS ${GAIN_GUI_OUTPUT}
)

mostly_harmless_add_binary_data(gain
        TARGET_NAME GainWebResources
        ROOT ${PROJECT_BINARY_DIR}/gain-gui
        BINARY_SOURCES ${GAIN_GUI_OUTPUT}
)
add_dependencies(GainWebResources gain-vite)


target_sources(gain PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gain_Entry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gain_SharedState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gain_Engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/gain_Editor.cpp
)

target_include_directories(gain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)
target_compile_definitions(gain PUBLIC HOT_RELOAD) # Comment this out to serve from RAM