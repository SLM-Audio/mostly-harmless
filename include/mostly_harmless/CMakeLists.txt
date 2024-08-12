if (APPLE)
    set(PLATFORM_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/gui/platform/mostlyharmless_GuiHelpersMacOS.h)
elseif (WIN32)

endif ()
set(MOSTLYHARMLESS_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_BusConfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_Descriptor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_Plugin.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_EventContext.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_Parameters.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/mostlyharmless_IEditor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/mostlyharmless_WebviewEditor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_TaskThread.h
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_Timer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/events/mostlyharmless_ParamEvent.h
        ${PLATFORM_HEADERS}
        PARENT_SCOPE)