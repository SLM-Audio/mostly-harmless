if (APPLE)
    set(PLATFORM_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/platform/mostlyharmless_GuiHelpersMacOS.mm
    )
endif ()
set(MOSTLYHARMLESS_SOURCES
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_Entry.cpp
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_Plugin.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/events/mostlyharmless_EventContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/mostlyharmless_IEditor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/mostlyharmless_WebviewEditor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_TaskThread.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_Timer.cpp
        ${PLATFORM_SOURCES}
        PARENT_SCOPE)
message(STATUS ${MOSTLYHARMLESS_PLATFORM_SOURCES})