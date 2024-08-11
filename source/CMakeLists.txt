if (APPLE)
    set(PLATFORM_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/platform/mostlyharmless_GuiHelpersMacOS.mm
    )
elseif (WIN32)
endif ()
set(MOSTLYHARMLESS_SOURCES
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_Entry.cpp
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_Plugin.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/mostlyharmless_WebviewEditor.cpp
        ${PLATFORM_SOURCES}
        PARENT_SCOPE)
message(STATUS ${MOSTLYHARMLESS_PLATFORM_SOURCES})