cmake_minimum_required(VERSION 3.24)
project(YourPlugin VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
FetchContent_Declare(
        mostly_harmless
        GIT_REPOSITORY https://github.com/SLM-Audio/mostly-harmless.git
        GIT_TAG main
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(mostly_harmless)
set(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/PluginProcessor.cpp)
mostly_harmless_add_plugin(
        ID "your.plugin"
        NAME "YourPlugin"
        SOURCES ${SOURCE}
        FEATURES "audio-effect"
)
target_include_directories(YourPlugin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

