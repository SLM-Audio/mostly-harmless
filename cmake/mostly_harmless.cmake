
# USAGE mostly_harmless_add_binary_data(YourTarget BINARY_SOURCES ...)
function(mostly_harmless_add_binary_data pluginTarget)
    set(LIST_ARGS
            BINARY_SOURCES
    )
    cmake_parse_arguments(PLUGIN "" "" "${LIST_ARGS}" ${ARGN})
    if (NOT pluginTarget)
        message(FATAL_ERROR " mostly_harmless_add_binary_data called with invalid target!")
    endif ()
    if (NOT PLUGIN_BINARY_SOURCES)
        message(FATAL_ERROR "mostly_harmless_add_binary_data called with no sources, aborting!")
    endif ()
    # set an output dir for the resulting files to live in..
    set(BINARY_RESOURCE_DEST "${CMAKE_CURRENT_BINARY_DIR}/binary_data")
    # and create it if necessary.
    file(MAKE_DIRECTORY ${BINARY_RESOURCE_DEST})
    set(BINARY_INPUT_SOURCES ${BINARY_RESOURCE_DEST}/BinaryData.cpp)
    # Now iterate over the sources the user provided..
    foreach (RESOURCE IN LISTS PLUGIN_BINARY_SOURCES)
        # and get the filename without path or extension.
        cmake_path(GET RESOURCE STEM CURRENT_STEM)
        cmake_path(GET RESOURCE EXTENSION LAST_ONLY CURRENT_EXTENSION)
        # strip the "."
        string(REPLACE "." "" STRIPPED_EXTENSION ${CURRENT_EXTENSION})
        # Reconstitute the path to be where we want our compiled cpp to live..
        set(CURRENT "${BINARY_RESOURCE_DEST}/${CURRENT_STEM}_${STRIPPED_EXTENSION}.cpp")
        # And add that path to the list of sources.
        list(APPEND BINARY_INPUT_SOURCES ${CURRENT})
        # Tell cmake that this source is generated at compile time.
        set_property(SOURCE ${CURRENT} PROPERTY GENERATED 1)
    endforeach ()
    if (NOT TARGET serialiser)
        # Compile our serialiser..
        add_executable(serialiser ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/serialise.cpp)
        # and link it to its deps.
        target_link_libraries(serialiser PRIVATE fmt::fmt-header-only)
    endif ()
    # Seem to need a custom command to only recompile the binary data if files change - not sure why..
    add_custom_command(
            OUTPUT ${BINARY_RESOURCE_DEST}/BinaryData.h
            COMMAND echo recompiling binary data..
            COMMAND serialiser ${BINARY_RESOURCE_DEST} "BinaryData" ${PLUGIN_BINARY_SOURCES}
            DEPENDS ${PLUGIN_BINARY_SOURCES}
    )
    # Add a custom target, which checks for changes in our binary sources, and runs the serialiser if any of them have changed..
    add_custom_target(binary-data-intermediate
            DEPENDS ${BINARY_RESOURCE_DEST}/BinaryData.h
    )
    # Now add a static library for our compiled deps to live in..
    add_library(binary-data STATIC)
    # and set the sources to be the generated ones from earlier.
    target_sources(binary-data PRIVATE ${BINARY_INPUT_SOURCES})
    # Ensure the intermediate target has run before compiling this target
    add_dependencies(binary-data binary-data-intermediate)
    # Include the dir with the resulting header in the shared code target (publicly, as it needs to propagate)
    target_include_directories(${pluginTarget} PUBLIC ${BINARY_RESOURCE_DEST})
    # Link our shared code target to the binary-data lib.
    target_link_libraries(${pluginTarget} PRIVATE binary-data)

endfunction()

# USAGE
# mostly_harmless_add_plugin(YourTarget FORMATS ... ID ..)
function(mostly_harmless_add_plugin targetName)
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
    )

    set(LIST_ARGS
            FORMATS # REQURED
            FEATURES # REQUIRED
    )

    cmake_parse_arguments(
            PLUGIN
            ""
            "${ARGS}"
            "${LIST_ARGS}"
            ${ARGN}
    )
    set(PLUGIN_NAME ${targetName})
    if (NOT PLUGIN_FORMATS)
        message(FATAL_ERROR "No formats provided! Options are: CLAP VST3 AU Standalone")
    endif ()
    if (NOT PLUGIN_NEEDS_NOTE_IN_BUS)
        set(PLUGIN_NEEDS_NOTE_IN_BUS FALSE)
    endif ()
    if (NOT PLUGIN_NEEDS_NOTE_OUT_BUS)
        set(PLUGIN_NEEDS_NOTE_OUT_BUS FALSE)
    endif ()
    if (NOT PLUGIN_NEEDS_AUDIO_IN_BUS)
        set(PLUGIN_NEEDS_AUDIO_IN_BUS TRUE)
    endif ()
    if (NOT PLUGIN_NEEDS_AUDIO_OUT_BUS)
        set(PLUGIN_NEEDS_AUDIO_OUT_BUS TRUE)
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

    if (NOT PLUGIN_ID)
        message(FATAL_ERROR " An ID is required")
    endif ()
    if (NOT PLUGIN_NAME)
        message(FATAL_ERROR "A name is required")
    endif ()
    if (NOT PLUGIN_FEATURES)
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
                    BUNDLE True
                    BUNDLE_EXTENSION clap
                    MACOSX_BUNDLE_GUI_IDENTIFIER " com.slm-audio.${PLUGIN_NAME}"
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
    endif ()
    list(FIND PLUGIN_FORMATS "VST3" INDEX)
    if (${INDEX} GREATER -1)
        add_library(${PLUGIN_NAME}_VST3 MODULE)
        target_add_vst3_wrapper(
                TARGET ${PLUGIN_NAME}_VST3
                OUTPUT_NAME ${PLUGIN_NAME}
        )
        target_link_libraries(${PLUGIN_NAME}_VST3 PUBLIC ${PLUGIN_NAME})
    endif ()
    list(FIND PLUGIN_FORMATS "AU" INDEX)
    if (${INDEX} GREATER -1)
        if (APPLE)
            if (NOT PLUGIN_AU_BUNDLE_ID)
                message(FATAL_ERROR "AU requires a bundle id!")
            endif ()
            if (NOT PLUGIN_AU_BUNDLE_VERSION)
                message(FATAL_ERROR "AU requires a bundle version!")
            endif ()
            if (NOT PLUGIN_VENDOR)
                message(FATAL_ERROR "AU requires you specify a vendor!")
            endif ()
            if (NOT PLUGIN_MANUFACTURER_CODE)
                message(FATAL_ERROR "AU requires a manufacturer code!")
            endif ()
            if (NOT PLUGIN_SUBTYPE_CODE)
                message(FATAL_ERROR "AU requires a plugin subtype code!")
            endif ()
            if (NOT PLUGIN_AU_TYPE)
                message(FATAL_ERROR "AU requires a type")
            endif ()
            add_library(${PLUGIN_NAME}_AU MODULE)
            target_add_auv2_wrapper(
                    TARGET ${PLUGIN_NAME}_AU
                    OUTPUT_NAME ${PLUGIN_NAME}
                    BUNDLE_IDENTIFIER ${PLUGIN_AU_BUNDLE_ID}
                    BUNDLE_VERSION ${PLUGIN_AU_BUNDLE_VERSION}
                    MANUFACTURER_NAME ${PLUGIN_VENDOR}
                    MANUFACTURER_CODE ${PLUGIN_MANUFACTURER_CODE}
                    SUBTYPE_CODE ${PLUGIN_SUBTYPE_CODE}
                    INSTRUMENT_TYPE ${PLUGIN_AU_TYPE}
            )
            target_link_libraries(${PLUGIN_NAME}_AU PUBLIC ${PLUGIN_NAME})
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
