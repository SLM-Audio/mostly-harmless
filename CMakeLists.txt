cmake_minimum_required(VERSION 3.28)
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()
project(MostlyHarmless VERSION 0.0.1 LANGUAGES CXX C)
set(EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED 20)
if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)
    enable_language(OBJCXX)
elseif (WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()
include(GNUInstallDirs)
set(MOSTLYHARMLESS_INSTALL_DEST "${CMAKE_INSTALL_LIBDIR}/cmake/mostly-harmless"
        CACHE STRING "Path where package files will be installed, relative to the install prefix"
)

include(FetchContent)
FetchContent_Declare(
        clap
        GIT_REPOSITORY https://github.com/free-audio/clap.git
        GIT_TAG main
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(clap)

FetchContent_Declare(
        clap_helpers
        GIT_REPOSITORY https://github.com/free-audio/clap-helpers.git
        GIT_TAG main
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(clap_helpers)

SET(CLAP_WRAPPER_CXX_STANDARD 20)
set(CLAP_WRAPPER_DOWNLOAD_DEPENDENCIES TRUE CACHE BOOL "Dowload dependencies")
FetchContent_Declare(
        clap-wrapper
        GIT_REPOSITORY https://github.com/free-audio/clap-wrapper.git
        GIT_TAG next
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(clap-wrapper)

FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 11.0.2
        GIT_SHALLOW ON)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
        sqlite3
        GIT_REPOSITORY https://github.com/SLM-Audio/sqlite3-cmake.git
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(sqlite3)

FetchContent_Declare(
        marvin
        GIT_REPOSITORY https://github.com/MeijisIrlnd/marvin.git
        GIT_TAG main
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(marvin)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/mostly_harmless)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source)

message(STATUS ${MOSTLYHARMLESS_SOURCES})
add_library(MostlyHarmless STATIC)
add_library(slma::MostlyHarmless ALIAS MostlyHarmless)

target_sources(MostlyHarmless PRIVATE ${MOSTLYHARMLESS_SOURCES} ${MOSTLYHARMLESS_INCLUDES})

target_include_directories(MostlyHarmless PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_include_directories(MostlyHarmless SYSTEM PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/modules>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/modules>"
)
# target_include_directories(MostlyHarmless SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules)
if (APPLE)
    set(PLATFORM_PREPRO MOSTLY_HARMLESS_MACOS)
    set(MOSTLY_HARMLESS_EXTRA_LIBS "-framework WebKit -framework AppKit")
elseif (WIN32)
    set(PLATFORM_PREPRO MOSTLY_HARMLESS_WINDOWS)
else ()
    #    message(FATAL_ERROR "Platform not supported!")
endif ()
target_compile_definitions(MostlyHarmless PUBLIC
        ${PLATFORM_PREPRO}
)

get_target_property(CLAP_HELPER_PROPS clap-helpers INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(clap-helpers SYSTEM INTERFACE ${CLAP_HELPER_PROPS})

target_link_libraries(MostlyHarmless
        PUBLIC
        slma::marvin
        clap
        clap-helpers
        fmt::fmt-header-only
        nlohmann_json
        sqlite3::sqlite3
        ${MOSTLY_HARMLESS_EXTRA_LIBS}
)


if (MSVC)
    target_compile_definitions(MostlyHarmless PUBLIC _CRT_SECURE_NO_WARNINGS)
    target_compile_options(MostlyHarmless PUBLIC
            /W4
            #            /WX
    )
else ()
    target_compile_options(MostlyHarmless PUBLIC
            -Wall
            -Wextra
            -Wpedantic
            #            -Werror
    )
endif ()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/mostly_harmless.cmake)
install(DIRECTORY "include/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING
        PATTERN "*.h"
)

install(
        TARGETS MostlyHarmless
        EXPORT MostlyHarmless
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT MostlyHarmless
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT MostlyHarmless
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# install( 
#     EXPORT MostlyHarmless 
#     NAMESPACE slma::
#     FILE Targets.cmake 
#     DESTINATION "${MOSTLYHARMLESS_INSTALL_DEST}"
#     COMPONENT MostlyHarmless
# )
if (TESTS)
    set(CMAKE_DEBUG_POSTFIX "")
    FetchContent_Declare(Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.0.1
            GIT_SHALLOW ON)
    FetchContent_MakeAvailable(Catch2)
    add_subdirectory(tests)
    add_executable(mostly-harmless-tests)
    target_sources(mostly-harmless-tests PRIVATE ${MOSTLYHARMLESS_TEST_SOURCE})
    target_link_libraries(mostly-harmless-tests PUBLIC MostlyHarmless Catch2::Catch2WithMain)

endif ()
if (${PROJECT_IS_TOP_LEVEL})
    add_subdirectory(docs)
endif ()
if (${EXAMPLES})
    add_subdirectory(examples/gain)
    add_subdirectory(examples/delay)
    add_subdirectory(examples/synth)
endif ()