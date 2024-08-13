set(CMAKE_CXX_STANDARD 20)
# set(ENABLE_EXPORTS FALSE)
FetchContent_Declare(magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG master
        GIT_SHALLOW ON)
FetchContent_MakeAvailable(magic_enum)
mostly_harmless_add_plugin(
        ID "slma.gain"
        NAME "Gain"
        SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/Gain.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/GainEditor.cpp
        FEATURES "audio-effect"
)

target_include_directories(Gain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Gain PRIVATE magic_enum::magic_enum)
