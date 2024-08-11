function(mostly_harmless_add_plugin) 
    set(ARGS 
        ID
        NAME 
        VENDOR 
        URL 
        MANUAL_URL
        SUPPORT_URL 
        VERSION 
        DESCRIPTION 
    )

    set(LIST_ARGS 
        SOURCES 
        FEATURES
    )

    cmake_parse_arguments(
        PLUGIN 
        "" 
        "${ARGS}"
        "${LIST_ARGS}"
        ${ARGN}
    )
    list(FIND PLUGIN_FEATURES "audio-effect" INDEX)
    if(${INDEX} GREATER -1) 
        set(PLUGIN_NOTE_BUS_CONFIG "BusConfig::None")
        set(PLUGIN_AUDIO_BUS_CONFIG "BusConfig::InputOutput")
    endif()
    list(FIND PLUGIN_FEATURES "instrument" INDEX)
    if(${INDEX} GREATER -1) 
        set(PLUGIN_NOTE_BUS_CONFIG "BusConfig::InputOnly")
        set(PLUGIN_AUDIO_BUS_CONFIG "BusConfig::OutputOnly")
    endif()
    list(FIND PLUGIN_FEATURES "note-effect" INDEX)
    if(${INDEX} GREATER -1)
        set(PLUGIN_NOTE_BUS_CONFIG "BusConfig::InputOutput")
        set(PLUGIN_AUDIO_BUS_CONFIG "BusConfig::None")
    endif()

    list(TRANSFORM PLUGIN_FEATURES REPLACE "(.+)" "\"\\1\"" OUTPUT_VARIABLE PLUGIN_FEATURES)
    list(JOIN PLUGIN_FEATURES ", " PLUGIN_FEATURES)

    if(NOT PLUGIN_ID) 
        message(FATAL_ERROR "An ID is required")
    endif() 
    if(NOT PLUGIN_NAME)
        message(FATAL_ERROR "A name is required")
    endif()
    if(NOT ${SOURCES})
        message(FATAL_ERROR "No sources passed to mostly_harmless_add_plugin")
    endif()
    configure_file(
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/mostlyharmless_Descriptor.cpp 
        ${CMAKE_CURRENT_BINARY_DIR}/mostlyharmless_Descriptor.cpp
    )
    add_library(${PLUGIN_NAME} MODULE ${PLUGIN_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/mostlyharmless_Descriptor.cpp)
    target_link_libraries(${PLUGIN_NAME} PRIVATE MostlyHarmless clap-wrapper-extensions)
    # target_link_options(${PLUGIN_NAME} PUBLIC "/INCLUDE:clap_entry")
    if(APPLE)
    message(STATUS ${CMAKE_SOURCE_DIR})
    set_target_properties(${PLUGIN_NAME} PROPERTIES 
        BUNDLE True 
        BUNDLE_EXTENSION clap 
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.slm-audio.${PLUGIN_NAME}"
        MACOSX_BUNDLE_BUNDLE_NAME ${PLUGIN_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION "0.0.1"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "0.0.1"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/mostly_harmless.plist.in
    )
    elseif(WIN32) 
        set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "" SUFFIX ".clap")
    else() 
        message(FATAL_ERROR "Platform unsupported")
    endif()
    set(VST3_TARGET ${PLUGIN_NAME}_vst3)
    add_library(${VST3_TARGET} MODULE)
    target_compile_features(${VST3_TARGET} PRIVATE cxx_std_20)
    target_add_vst3_wrapper(TARGET ${VST3_TARGET} 
        OUTPUT_NAME ${PLUGIN_NAME}
        SUPPORTS_ALL_NOTE_EXPRESSIONS TRUE 
    )
    set_property(TARGET ${VST3_TARGET} PROPERTY CXX_STANDARD 20)
    set(STANDALONE_TARGET ${PLUGIN_NAME}_standalone)
    add_executable(${STANDALONE_TARGET})
    target_add_standalone_wrapper(TARGET ${STANDALONE_TARGET}
    OUTPUT_NAME ${PLUGIN_NAME}
            HOSTED_CLAP_NAME ${PLUGIN_NAME}
    )


endfunction()
