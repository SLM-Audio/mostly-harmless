set(CMAKE_CXX_STANDARD 20)

FetchContent_Declare(magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG master
        GIT_SHALLOW ON)

FetchContent_MakeAvailable(magic_enum)

mostly_harmless_add_plugin(delay
        FORMATS CLAP VST3 Standalone
        ID "delay"
        FEATURES "audio-effect"
)

target_sources(delay PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source/Plugin.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Delay.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/PluginEditor.cpp)
target_include_directories(delay PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)
target_link_libraries(delay PRIVATE magic_enum::magic_enum)