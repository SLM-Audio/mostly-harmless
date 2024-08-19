set(CMAKE_CXX_STANDARD 20)

mostly_harmless_add_plugin(delay
        FORMATS CLAP VST3 Standalone
        ID "delay"
)

target_sources(delay PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source/Plugin.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/PluginEditor.cpp)
target_include_directories(delay PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)