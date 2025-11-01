
function(mostly_harmless_add_binary_data pluginTarget)
    set(ARGS
            TARGET_NAME
            ROOT
    )
    set(LIST_ARGS BINARY_SOURCES)
    cmake_parse_arguments(ARG "" "${ARGS}" "${LIST_ARGS}" ${ARGN})
    if (NOT DEFINED ARG_TARGET_NAME)
        message(FATAL_ERROR "TARGET_NAME is required!")
    endif ()
    if (NOT DEFINED ARG_ROOT)
        message(FATAL_ERROR "ROOT is required!")
    endif ()
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${ARG_TARGET_NAME})
    configure_file(
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/mostlyharmless_BinaryData.h
            ${CMAKE_CURRENT_BINARY_DIR}/${ARG_TARGET_NAME}/mostlyharmless_${ARG_TARGET_NAME}.h
    )
    configure_file(
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/mostlyharmless_BinaryData.cpp
            ${CMAKE_CURRENT_BINARY_DIR}/${ARG_TARGET_NAME}/mostlyharmless_${ARG_TARGET_NAME}.cpp
    )
    target_sources(${pluginTarget} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/${ARG_TARGET_NAME}/mostlyharmless_${ARG_TARGET_NAME}.cpp)
    target_include_directories(${pluginTarget} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/${ARG_TARGET_NAME})
    include(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/CMakeRC.cmake)
    cmrc_add_resource_library(${ARG_TARGET_NAME} WHENCE ${ARG_ROOT} ${ARG_BINARY_SOURCES})
    target_link_libraries(${pluginTarget} PRIVATE ${ARG_TARGET_NAME})
endfunction()

function(mostly_harmless_sign_target targetName artefactName signId)
    get_target_property(extension ${targetName} BUNDLE_EXTENSION)
    set(artefact "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${artefactName}.${extension}")
    add_custom_command(
            TARGET ${targetName} POST_BUILD
            COMMAND codesign --force -s "${signId}" -v "${artefact}" --deep --strict --options=runtime --timestamp VERBATIM
    )

endfunction()
# USAGE
# mostly_harmless_add_plugin(YourTarget FORMATS ... ID ..)
function(mostly_harmless_add_plugin targetName)
    set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/${targetName}_artefacts)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
    set(ARGS
            ID # REQUIRED
            VENDOR # OPTIONAL
            URL # OPTIONAL
            MANUAL_URL # OPTIONAL
            SUPPORT_URL # OPTIONAL
            VERSION # OPTIONAL
            DESCRIPTION # OPTIONAL

            NEEDS_NOTE_IN_BUS # OPTIONAL, DEFAULT FALSE
            NEEDS_NOTE_OUT_BUS # OPTIONAL, DEFAULT FALSE
            NEEDS_AUDIO_IN_BUS # OPTIONAL, DEFAULT TRUE
            NEEDS_AUDIO_OUT_BUS # OPTIONAL, DEFAULT TRUE

            AU_BUNDLE_ID # REQUIRED IF AU
            AU_BUNDLE_VERSION # REQUIRED IF AU
            MANUFACTURER_CODE # REQUIRED IF AU
            SUBTYPE_CODE # REQUIRED IF AU
            AU_TYPE # REQUIRED IF AU

            SIGN_ID # OPTIONAL, if provided, will codesign
    )

    set(LIST_ARGS
            FORMATS # REQURED
            FEATURES # REQUIRED
    )

    cmake_parse_arguments(
            PLUGIN
            " "
            "${ARGS}"
            "${LIST_ARGS}"
            ${ARGN}
    )
    set(PLUGIN_NAME ${targetName})
    if (NOT DEFINED PLUGIN_FORMATS)
        message(FATAL_ERROR "No formats provided! Options are: CLAP VST3 AU Standalone")
    endif ()
    if (NOT DEFINED PLUGIN_NEEDS_NOTE_IN_BUS)
        set(PLUGIN_NEEDS_NOTE_IN_BUS FALSE)
    endif ()
    if (NOT DEFINED PLUGIN_NEEDS_NOTE_OUT_BUS)
        set(PLUGIN_NEEDS_NOTE_OUT_BUS FALSE)
    endif ()
    if (NOT DEFINED PLUGIN_NEEDS_AUDIO_IN_BUS)
        set(PLUGIN_NEEDS_AUDIO_IN_BUS TRUE)
    endif ()
    if (NOT DEFINED PLUGIN_NEEDS_AUDIO_OUT_BUS)
        set(PLUGIN_NEEDS_AUDIO_OUT_BUS TRUE)
    endif ()
    if (NOT DEFINED PLUGIN_VERSION)
        set(PLUGIN_VERSION "0.0.1")
    endif ()
    # NOTE BUSSES
    if (${PLUGIN_NEEDS_NOTE_IN_BUS})
        if (${PLUGIN_NEEDS_NOTE_OUT_BUS})
            set(PLUGIN_NOTE_BUS_CONFIG "BusConfig::InputOutput")
        else ()
            set(PLUGIN_NOTE_BUS_CONFIG "BusConfig::InputOnly")
        endif ()
    else ()
        if (${PLUGIN_NEEDS_NOTE_OUT_BUS})
            set(PLUGIN_NOTE_BUS_CONFIG "BusConfig::OutputOnly")
        else ()
            set(PLUGIN_NOTE_BUS_CONFIG "BusConfig::None")
        endif ()
    endif ()
    if (NOT ${PLUGIN_NEEDS_AUDIO_OUT_BUS})
        message(FATAL_ERROR "Plugins that don't support audio output are not supported yet!")
    endif ()
    # AUDIO BUSSES
    if (${PLUGIN_NEEDS_AUDIO_IN_BUS})
        if (${PLUGIN_NEEDS_AUDIO_OUT_BUS})
            set(PLUGIN_AUDIO_BUS_CONFIG "BusConfig::InputOutput")
        else ()
            set(PLUGIN_AUDIO_BUS_CONFIG "BusConfig::InputOnly")
        endif ()
    else ()
        if (${PLUGIN_NEEDS_AUDIO_OUT_BUS})
            set(PLUGIN_AUDIO_BUS_CONFIG "BusConfig::OutputOnly")
        else ()
            set(PLUGIN_AUDIO_BUS_CONFIG "BusConfig::None")
        endif ()
    endif ()

    list(TRANSFORM PLUGIN_FEATURES REPLACE "(.+)" "\"\\1\"" OUTPUT_VARIABLE PLUGIN_FEATURES)
    list(JOIN PLUGIN_FEATURES ", " PLUGIN_FEATURES)

    if (NOT DEFINED PLUGIN_ID)
        message(FATAL_ERROR "An ID is required")
    endif ()
    if (NOT DEFINED PLUGIN_NAME)
        message(FATAL_ERROR "A name is required")
    endif ()
    if (NOT DEFINED PLUGIN_FEATURES)
        message(FATAL_ERROR "At least one feature is required")
    endif ()
    configure_file(
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/mostlyharmless_Descriptor.cpp
            ${CMAKE_CURRENT_BINARY_DIR}/mostlyharmless_Descriptor.cpp
    )
    add_library(${PLUGIN_NAME} STATIC)
    target_sources(${PLUGIN_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/mostlyharmless_Descriptor.cpp)
    target_link_libraries(${PLUGIN_NAME} PUBLIC MostlyHarmless)
    set_target_properties(${PLUGIN_NAME} PROPERTIES OUTPUT_NAME ${PLUGIN_NAME}_SharedCode)

    list(FIND PLUGIN_FORMATS "CLAP" INDEX)
    if (${INDEX} GREATER -1)
        add_library(${PLUGIN_NAME}_CLAP MODULE)
        target_link_libraries(${PLUGIN_NAME}_CLAP PUBLIC ${PLUGIN_NAME})
        if (APPLE)
            set_target_properties(${PLUGIN_NAME}_CLAP PROPERTIES
                    OUTPUT_NAME ${PLUGIN_NAME}
                    LIBRARY_OUTPUT_NAME ${PLUGIN_NAME}
                    BUNDLE True
                    BUNDLE_EXTENSION clap
                    MACOSX_BUNDLE_GUI_IDENTIFIER "com.slm-audio.${PLUGIN_NAME}"
                    MACOSX_BUNDLE_BUNDLE_NAME ${PLUGIN_NAME}
                    MACOSX_BUNDLE_BUNDLE_VERSION ${PLUGIN_VERSION}
                    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PLUGIN_VERSION}
                    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/mostly_harmless.plist.in
            )
            if (NOT ${PLUGIN_SIGN_ID} STREQUAL "")
                mostly_harmless_sign_target(${PLUGIN_NAME}_CLAP ${PLUGIN_NAME} ${PLUGIN_SIGN_ID})
            endif ()

        elseif (WIN32)
            set_target_properties(${PLUGIN_NAME}_CLAP
                    PROPERTIES
                    OUTPUT_NAME ${PLUGIN_NAME}

                    PREFIX ""
                    SUFFIX ".clap")
        else ()
            message(FATAL_ERROR "Platform unsupported")
        endif ()
    endif ()
    list(FIND PLUGIN_FORMATS "VST3" INDEX)
    if (${INDEX} GREATER -1)
        add_library(${PLUGIN_NAME}_VST3 MODULE)
        target_add_vst3_wrapper(
                TARGET ${PLUGIN_NAME}_VST3
                OUTPUT_NAME ${PLUGIN_NAME}
        )
        if (APPLE)
            set_target_properties(${PLUGIN_NAME}_VST3 PROPERTIES
                    OUTPUT_NAME ${PLUGIN_NAME}
                    BUNDLE True
                    BUNDLE_EXTENSION vst3
                    MACOSX_BUNDLE_GUI_IDENTIFIER "com.slm-audio.${PLUGIN_NAME}"
                    MACOSX_BUNDLE_BUNDLE_NAME ${PLUGIN_NAME}
                    MACOSX_BUNDLE_BUNDLE_VERSION ${PLUGIN_VERSION}
                    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PLUGIN_VERSION}
                    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/mostly_harmless.plist.in
            )
            if (NOT ${PLUGIN_SIGN_ID} STREQUAL "")
                mostly_harmless_sign_target(${PLUGIN_NAME}_VST3 ${PLUGIN_NAME} ${PLUGIN_SIGN_ID})
            endif ()

        else ()
        endif ()
        target_link_libraries(${PLUGIN_NAME}_VST3 PUBLIC ${PLUGIN_NAME})
    endif ()
    list(FIND PLUGIN_FORMATS "AU" INDEX)
    if (${INDEX} GREATER -1)
        if (APPLE)
            if (NOT DEFINED PLUGIN_AU_BUNDLE_ID)
                message(FATAL_ERROR "AU requires a bundle id!")
            endif ()
            if (NOT DEFINED PLUGIN_AU_BUNDLE_VERSION)
                message(FATAL_ERROR "AU requires a bundle version!")
            endif ()
            if (NOT DEFINED PLUGIN_VENDOR)
                message(FATAL_ERROR "AU requires you specify a vendor!")
            endif ()
            if (NOT DEFINED PLUGIN_MANUFACTURER_CODE)
                message(FATAL_ERROR "AU requires a manufacturer code!")
            endif ()
            if (NOT DEFINED PLUGIN_SUBTYPE_CODE)
                message(FATAL_ERROR "AU requires a plugin subtype code!")
            endif ()
            if (NOT DEFINED PLUGIN_AU_TYPE)
                message(FATAL_ERROR "AU requires a type")
            endif ()
            add_library(${PLUGIN_NAME}_AU MODULE)
            target_sources(${PLUGIN_NAME}_AU PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/mostlyharmless_Descriptor.cpp)
            target_link_libraries(${PLUGIN_NAME}_AU PUBLIC ${PLUGIN_NAME})
            add_dependencies(${PLUGIN_NAME}_AU ${PLUGIN_NAME}_CLAP)
            target_add_auv2_wrapper(
                    TARGET ${PLUGIN_NAME}_AU
                    CLAP_TARGET_FOR_CONFIG ${PLUGIN_NAME}_CLAP
                    OUTPUT_NAME ${PLUGIN_NAME}
                    BUNDLE_EXTENSION "component"
                    BUNDLE_IDENTIFIER ${PLUGIN_AU_BUNDLE_ID}
                    BUNDLE_VERSION ${PLUGIN_VERSION}
                    MANUFACTURER_NAME ${PLUGIN_VENDOR}
                    MANUFACTURER_CODE ${PLUGIN_MANUFACTURER_CODE}
                    SUBTYPE_CODE ${PLUGIN_SUBTYPE_CODE}
                    INSTRUMENT_TYPE ${PLUGIN_AU_TYPE}
            )
            if (NOT ${PLUGIN_SIGN_ID} STREQUAL "")
                mostly_harmless_sign_target(${PLUGIN_NAME}_AU ${PLUGIN_NAME} ${PLUGIN_SIGN_ID})
            endif ()
        endif ()
    endif ()

    list(FIND PLUGIN_FORMATS "Standalone" INDEX)
    if (${INDEX} GREATER -1)
        message(STATUS "STANDALONE")
        add_executable(${PLUGIN_NAME}_Standalone)
        target_link_libraries(${PLUGIN_NAME}_Standalone PRIVATE ${PLUGIN_NAME})
        target_add_standalone_wrapper(TARGET ${PLUGIN_NAME}_Standalone
                OUTPUT_NAME ${PLUGIN_NAME}
                STATICALLY_LINKED_CLAP_ENTRY True
        )
    endif ()
endfunction()
