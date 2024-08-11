cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)
project(GainDemo VERSION 0.0.1)
if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
include(FetchContent)
FetchContent_Declare(mostly_harmless
        GIT_REPOSITORY "https://github.com/SLM-Audio/mostly-harmless.git"
        GIT_TAG main
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(mostly_harmless)
mostly_harmless_add_plugin(
        ID "com.placeholder.placeholder"
        NAME "Placeholder"
        SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/PluginProcessor.cpp
        FEATURES "audio-effect"
)