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
            # Can be FX or Instrument
            TYPE
    )

    set(LIST_ARGS
            SOURCES
            BINARY_RESOURCES
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
    # Ok this is insane. If the user provided binary resources...
    if (PLUGIN_BINARY_RESOURCES)
        # set an output dir for the resulting files to live in..
        set(BINARY_RESOURCE_DEST "${CMAKE_CURRENT_BINARY_DIR}/binary_data")
        # and create it if necessary.
        file(MAKE_DIRECTORY ${BINARY_RESOURCE_DEST})
        # Now iterate over the sources the user provided..
        foreach (RESOURCE IN LISTS PLUGIN_BINARY_RESOURCES)
            # and get the filename without path or extension.
            cmake_path(GET RESOURCE STEM CURRENT_STEM)
            # Reconstitute the path to be where we want our compiled cpp to live..
            set(CURRENT "${BINARY_RESOURCE_DEST}/${CURRENT_STEM}.cpp")
            # And add that path to the list of sources.
            list(APPEND BINARY_SOURCES ${CURRENT})
            # Tell cmake that this source is generated at compile time.
            set_property(SOURCE ${CURRENT} PROPERTY GENERATED 1)
        endforeach ()
        # Compile our serialiser..
        add_executable(serialiser ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/serialise.cpp)
        # and link it to its deps.
        target_link_libraries(serialiser PRIVATE fmt::fmt-header-only)
        message(STATUS "GENERATOR: ${CMAKE_GENERATOR}")
        if (${CMAKE_GENERATOR} STREQUAL "Xcode")
            message(STATUS "USING XC")
            #depends on build type
            set(SERIALISER_EXE_PATH
                    $<IF:$<CONFIG:DEBUG>,${CMAKE_CURRENT_BINARY_DIR}/Debug,${CMAKE_CURRENT_BINARY_DIR}/Release>
            )

        else ()
            message(STATUS "NOT USING XC")
            set(SERIALISER_EXE_PATH ${CMAKE_CURRENT_BINARY_DIR})

        endif ()
        # Seem to need a custom command to only recompile the binary data if files change - not sure why..
        add_custom_command(
                OUTPUT ${BINARY_RESOURCE_DEST}/BinaryData.h
                COMMAND echo Recompiling binary data
                COMMAND ${SERIALISER_EXE_PATH}/serialiser ${BINARY_RESOURCE_DEST} "BinaryData" ${PLUGIN_BINARY_RESOURCES}
                #                COMMAND ${CMAKE_CURRENT_BINARY_DIR}/serialiser ${BINARY_RESOURCE_DEST} "BinaryData" ${PLUGIN_BINARY_RESOURCES}
                DEPENDS ${PLUGIN_BINARY_RESOURCES}
        )
        # Add a custom target, which checks for changes in our binary sources, and runs the serialiser if any of them have changed..
        add_custom_target(binary-data-intermediate
                DEPENDS ${BINARY_RESOURCE_DEST}/BinaryData.h
        )
        # Depend our intermediate target on the serialiser executable - so the serialiser gets built before this target..
        add_dependencies(binary-data-intermediate serialiser)
        # Now add a static library for our compiled deps to live in..
        add_library(binary-data STATIC)
        # and set the sources to be the generated ones from earlier.
        target_sources(binary-data PRIVATE ${BINARY_SOURCES})
        # Ensure the intermediate target has run before compiling this target
        add_dependencies(binary-data binary-data-intermediate)
        # Include the dir with the resulting header in the shared code target (publicly, as it needs to propagate)
        target_include_directories(${PLUGIN_NAME}_SharedCode PUBLIC ${BINARY_RESOURCE_DEST})
        # Link our shared code target to the binary-data lib.
        target_link_libraries(${PLUGIN_NAME}_SharedCode PRIVATE binary-data)
    endif ()

    target_link_libraries(${PLUGIN_NAME}_SharedCode PUBLIC MostlyHarmless)

    #    add_library(${PLUGIN_NAME}_CLAP MODULE ${CMAKE_SOURCE_DIR}/source/mostlyharmless_Entry.cpp)
    add_library(${PLUGIN_NAME}_CLAP MODULE)
    target_link_libraries(${PLUGIN_NAME}_CLAP PUBLIC ${PLUGIN_NAME}_SharedCode)
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

    add_library(${PLUGIN_NAME}_VST3 MODULE)
    target_add_vst3_wrapper(
            TARGET ${PLUGIN_NAME}_VST3
            OUTPUT_NAME ${PLUGIN_NAME}
    )
    target_link_libraries(${PLUGIN_NAME}_VST3 PUBLIC ${PLUGIN_NAME}_SharedCode)

    if (APPLE)
        #TODO: AU
        add_library(${PLUGIN_NAME}_AU MODULE)
        target_add_auv2_wrapper(
                TARGET ${PLUGIN_NAME}_AU
                OUTPUT_NAME ${PLUGIN_NAME}
                BUNDLE_IDENTIFIER "com.${PLUGIN_VENDOR}.${PLUGIN_NAME}"
                BUNDLE_VERSION "1"
                MANUFACTURER_NAME ${PLUGIN_VENDOR}
                MANUFACTURER_CODE "Slma"
                SUBTYPE_CODE "Test"
                INSTRUMENT_TYPE " aufx"
        )
        target_link_libraries(${PLUGIN_NAME}_AU PUBLIC ${PLUGIN_NAME}_SharedCode})
    endif ()

    add_executable(${PLUGIN_NAME}_Standalone)
    target_link_libraries(${PLUGIN_NAME}_Standalone PRIVATE ${PLUGIN_NAME}_SharedCode)
    target_add_standalone_wrapper(TARGET ${PLUGIN_NAME}_Standalone
            OUTPUT_NAME ${PLUGIN_NAME}
            STATICALLY_LINKED_CLAP_ENTRY True
    )
endfunction()
