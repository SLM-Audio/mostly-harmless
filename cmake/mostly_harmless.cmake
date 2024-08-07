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

    message(STATUS ${PLUGIN_SOURCES})

    add_library(${PLUGIN_NAME} MODULE ${PLUGIN_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/mostlyharmless_Descriptor.cpp)
    target_link_libraries(${PLUGIN_NAME} PUBLIC MostlyHarmless)
    # target_link_options(${PLUGIN_NAME} PUBLIC "/INCLUDE:clap_entry")
    # if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    #     set_target_properties(${PLUGIN_NAME} PROPERTIES SUFFIX ".clap")
    # endif()
endfunction()
