cmake_minimum_required(VERSION 3.24)
project($PROJECT_NAME$ VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)
if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)
    enable_language(OBJCXX)
elseif(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

include(FetchContent)
FetchContent_Declare(
        mostly_harmless
        GIT_REPOSITORY https://github.com/SLM-Audio/mostly-harmless.git
        GIT_TAG main
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(mostly_harmless)

mostly_harmless_add_plugin(
        $PROJECT_NAME$
        ID "your_company.$LOWER_PROJECT_NAME$"
        VENDOR "Your Company"
        FORMATS CLAP AU VST3 Standalone
        AU_BUNDLE_ID "com.your_company.$LOWER_PROJECT_NAME$"
        AU_BUNDLE_VERSION "1"
        MANUFACTURER_CODE "Exmp"
        SUBTYPE_CODE "Mhp1"
        AU_TYPE "aufx"
        FEATURES "audio-effect"
)

# TODO: Setup your frontend code here - see examples/delay for a React example

target_sources($PROJECT_NAME$ PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source/$LOWER_PROJECT_NAME$_Entry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/$LOWER_PROJECT_NAME$_SharedState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/$LOWER_PROJECT_NAME$_Engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/$LOWER_PROJECT_NAME$_Editor.cpp
)

target_include_directories($PROJECT_NAME$ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)
target_compile_definitions($PROJECT_NAME$ PUBLIC HOT_RELOAD) # Comment this out to serve from RAM