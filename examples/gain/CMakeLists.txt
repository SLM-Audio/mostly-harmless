mostly_harmless_add_plugin( 
    ID "slma.gain"
    NAME "Gain"
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/Gain.cpp 
    FEATURES "audio-effect"
)

target_include_directories(Gain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
