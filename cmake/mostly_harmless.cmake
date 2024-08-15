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
    if (${INDEX} GREATER -1)
        set(PLUGIN_NOTE_BUS_CONFIG "BusConfig::None")
        set(PLUGIN_AUDIO_BUS_CONFIG "BusConfig::InputOutput")
    endif ()
    list(FIND PLUGIN_FEATURES "instrument" INDEX)
    if (${INDEX} GREATER -1)
        set(PLUGIN_NOTE_BUS_CONFIG "BusConfig::InputOnly")
        set(PLUGIN_AUDIO_BUS_CONFIG "BusConfig::OutputOnly")
    endif ()
    list(FIND PLUGIN_FEATURES "note-effect" INDEX)
    if (${INDEX} GREATER -1)
        set(PLUGIN_NOTE_BUS_CONFIG "BusConfig::InputOutput")
        set(PLUGIN_AUDIO_BUS_CONFIG "BusConfig::None")
    endif ()

    list(TRANSFORM PLUGIN_FEATURES REPLACE "(.+)" "\"\\1\"" OUTPUT_VARIABLE PLUGIN_FEATURES)
    list(JOIN PLUGIN_FEATURES ", " PLUGIN_FEATURES)

    if (NOT PLUGIN_ID)
        message(FATAL_ERROR "An ID is required")
    endif ()
    if (NOT PLUGIN_NAME)
        message(FATAL_ERROR "A name is required")
    endif ()
    if (NOT ${SOURCES})
        message(FATAL_ERROR "No sources passed to mostly_harmless_add_plugin")
    endif ()
    configure_file(
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/mostlyharmless_Descriptor.cpp
            ${CMAKE_CURRENT_BINARY_DIR}/mostlyharmless_Descriptor.cpp
    )
    list(APPEND PLUGIN_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/mostlyharmless_Descriptor.cpp)
    add_library(${PLUGIN_NAME}_SharedCode STATIC)
    target_sources(${PLUGIN_NAME}_SharedCode PUBLIC ${PLUGIN_SOURCES})
    target_link_libraries(${PLUGIN_NAME}_SharedCode PUBLIC MostlyHarmless)

    #    add_library(${PLUGIN_NAME}_CLAP MODULE ${CMAKE_SOURCE_DIR}/source/mostlyharmless_Entry.cpp)
    add_library(${PLUGIN_NAME}_CLAP MODULE)
    target_link_libraries(${PLUGIN_NAME}_CLAP PUBLIC ${PLUGIN_NAME}_SharedCode)
    if (APPLE)
        set_target_properties(${PLUGIN_NAME}_CLAP PROPERTIES
                OUTPUT_NAME ${PLUGIN_NAME}
                BUNDLE True
                BUNDLE_EXTENSION clap
                MACOSX_BUNDLE_GUI_IDENTIFIER "com.slm-audio.${PLUGIN_NAME}"
                MACOSX_BUNDLE_BUNDLE_NAME ${PLUGIN_NAME}
                MACOSX_BUNDLE_BUNDLE_VERSION "0.0.1"
                MACOSX_BUNDLE_SHORT_VERSION_STRING "0.0.1"
                MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/mostly_harmless.plist.in
        )
    elseif (WIN32)
        set_target_properties(${PLUGIN_NAME}_CLAP
                PROPERTIES
                OUTPUT_NAME ${PLUGIN_NAME}
                PREFIX ""
                SUFFIX ".clap")
    else ()
        message(FATAL_ERROR "Platform unsupported")
    endif ()

    add_library(${PLUGIN_NAME}_VST3 MODULE)
    target_add_vst3_wrapper(
            TARGET ${PLUGIN_NAME}_VST3
            OUTPUT_NAME ${PLUGIN_NAME}
    )
    target_link_libraries(${PLUGIN_NAME}_VST3 PUBLIC ${PLUGIN_NAME}_SharedCode)

    if (APPLE)
        #TODO: AU
        #        add_library(${PLUGIN_NAME}_AU MODULE)
        #        target_add_auv2_wrapper(
        #                TARGET ${PLUGIN_NAME}_AU
        #                OUTPUT_NAME ${PLUGIN_NAME}
        #                BUNDLE_IDENTIFIER "com.${PLUGIN_VENDOR}.${PLUGIN_NAME}"
        #                BUNDLE_VERSION "1"
        #
        #                MANUFACTURER_NAME ${PLUGIN_VENDOR}
        #                MANUFACTURER_CODE
        #        )
        #        target_link_libraries(${PLUGIN_NAME}_AU PUBLIC ${PLUGIN_NAME}_SharedCode})
    endif ()

    add_executable(${PLUGIN_NAME}_Standalone)
    target_link_libraries(${PLUGIN_NAME}_Standalone PRIVATE ${PLUGIN_NAME}_SharedCode)
    target_add_standalone_wrapper(TARGET ${PLUGIN_NAME}_Standalone
            OUTPUT_NAME ${PLUGIN_NAME}
            STATICALLY_LINKED_CLAP_ENTRY True
    )
endfunction()
