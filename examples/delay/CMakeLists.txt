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

target_sources(delay PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source/delay_Entry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/delay_SharedState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/delay_Engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/delay_Editor.cpp
)

target_include_directories(delay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)
target_compile_definitions(delay PUBLIC HOT_RELOAD) # Comment this out to serve from RAM