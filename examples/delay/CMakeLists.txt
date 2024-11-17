set(CMAKE_CXX_STANDARD 20)

mostly_harmless_add_plugin(
        delay
        ID "your_company.delay"
        VENDOR "Your Company"
        FORMATS CLAP AU VST3 Standalone
        AU_BUNDLE_ID "com.your_company.delay"
        AU_BUNDLE_VERSION "1"
        MANUFACTURER_CODE "Exmp"
        SUBTYPE_CODE "Mhp1"
        AU_TYPE "aufx"
        FEATURES "audio-effect"
)

# TODO: Setup your frontend code here - see examples/delay for a React example
set(DELAY_GUI_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/delay-gui)
file(GLOB_RECURSE DELAY_GUI_SOURCES "${DELAY_GUI_ROOT}/*" FORCE)
set(DELAY_GUI_OUTPUT
        ${PROJECT_BINARY_DIR}/delay-gui/index.html
        ${PROJECT_BINARY_DIR}/delay-gui/index.css
        ${PROJECT_BINARY_DIR}/delay-gui/index.js
)

if (WIN32)
    add_custom_command(
            OUTPUT ${DELAY_GUI_OUTPUT}
            COMMAND npm run build
            COMMAND powershell Remove-Item -Path ${PROJECT_BINARY_DIR}/delay-gui -Recurse -Force
            COMMAND powershell Copy-Item -Force -Recurse "./dist" ${PROJECT_BINARY_DIR}/delay-gui
            WORKING_DIRECTORY ${DELAY_GUI_ROOT}
            DEPENDS ${DELAY_GUI_SOURCES}
            USES_TERMINAL
    )
else ()
    add_custom_command(
            OUTPUT ${DELAY_GUI_OUTPUT}
            COMMAND npm run build
            COMMAND rm -rf ${PROJECT_BINARY_DIR}/delay-gui
            COMMAND cp -r ./dist ${PROJECT_BINARY_DIR}/delay-gui
            WORKING_DIRECTORY ${DELAY_GUI_ROOT}
            DEPENDS ${DELAY_GUI_SOURCES}
            USES_TERMINAL
    )
endif ()

add_custom_target(
        delay-vite
        ALL
        DEPENDS ${DELAY_GUI_OUTPUT}
)

mostly_harmless_add_binary_data(delay
        TARGET_NAME DelayWebResources
        ROOT ${PROJECT_BINARY_DIR}/delay-gui
        BINARY_SOURCES ${DELAY_GUI_OUTPUT}
)
add_dependencies(DelayWebResources delay-vite)


target_sources(delay PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source/delay_Entry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/delay_SharedState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/delay_Engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/delay_Editor.cpp
)

target_include_directories(delay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)
#target_compile_definitions(delay PUBLIC HOT_RELOAD) # Comment this out to serve from RAM