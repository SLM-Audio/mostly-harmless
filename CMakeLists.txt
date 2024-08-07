cmake_minimum_required(VERSION 3.24)
project(MostlyHarmless VERSION 0.0.1)
set(EXPORT_COMPILE_COMMANDS TRUE)
set(CXX_STANDARD_REQUIRED 20)
include(FetchContent)
FetchContent_Declare(
    clap 
    GIT_REPOSITORY https://github.com/free-audio/clap.git
    GIT_TAG main
)
FetchContent_MakeAvailable(clap)

FetchContent_Declare( 
    clap_helpers 
    GIT_REPOSITORY https://github.com/free-audio/clap-helpers.git
    GIT_TAG main
)
FetchContent_MakeAvailable(clap_helpers)


add_subdirectory(modules/marvin)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/mostly_harmless)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source)

message(STATUS ${MOSTLYHARMLESS_SOURCES}) 
add_library(MostlyHarmless STATIC)
target_sources(MostlyHarmless PUBLIC ${MOSTLYHARMLESS_SOURCES}) 
target_include_directories(MostlyHarmless PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/mostly_harmless)

target_link_libraries(MostlyHarmless 
    PUBLIC
    slma::marvin 
    clap 
    clap-helpers
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/mostly_harmless.cmake)

add_subdirectory(examples/gain)