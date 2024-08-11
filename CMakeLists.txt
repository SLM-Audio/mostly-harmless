cmake_minimum_required(VERSION 3.24)
  if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
  endif ()
project(MostlyHarmless VERSION 0.0.1 LANGUAGES CXX)
set(EXPORT_COMPILE_COMMANDS TRUE)
set(CXX_STANDARD_REQUIRED 20)
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)
    enable_language(OBJCXX)
elseif(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
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

add_subdirectory(modules/marvin)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/mostly_harmless)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source)

message(STATUS ${MOSTLYHARMLESS_SOURCES}) 
add_library(MostlyHarmless STATIC)
add_library(slma::MostlyHarmless ALIAS MostlyHarmless)

target_sources(MostlyHarmless PRIVATE ${MOSTLYHARMLESS_SOURCES}) 

target_include_directories(MostlyHarmless PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_include_directories(MostlyHarmless SYSTEM PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/modules>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/modules>
)
# target_include_directories(MostlyHarmless SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules)
if(APPLE) 
    set(PLATFORM_PREPRO MOSTLY_HARMLESS_MACOS)
    set(MOSTLY_HARMLESS_EXTRA_LIBS "-framework WebKit -framework AppKit")
elseif(WIN32)
    set(PLATFORM_PREPRO MOSTLY_HARMLESS_WINDOWS)
else()
    message(FATAL_ERROR "Platform not supported!")
endif()
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
    ${MOSTLY_HARMLESS_EXTRA_LIBS}
)


if(MSVC)
    target_compile_definitions(MostlyHarmless PUBLIC _CRT_SECURE_NO_WARNINGS)
    target_compile_options(MostlyHarmless PUBLIC
    /W4
    /WX)
else()
    target_compile_options(MostlyHarmless PUBLIC 
        -Wall 
        -Wextra 
        -Wpedantic
        -Werror
    )
endif()

SET(CLAP_WRAPPER_CXX_STANDARD 20)
set(CLAP_WRAPPER_DOWNLOAD_DEPENDENCIES TRUE CACHE BOOL "Dowload dependencies")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/clap-wrapper EXCLUDE_FROM_ALL)
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

add_subdirectory(docs)
add_subdirectory(examples/gain)