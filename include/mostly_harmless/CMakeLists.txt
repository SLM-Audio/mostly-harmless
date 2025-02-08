if (APPLE)
    set(PLATFORM_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/gui/platform/mostlyharmless_GuiHelpersMacOS.h)
elseif (WIN32)

endif ()
set(MOSTLYHARMLESS_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_BusConfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_Descriptor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_PluginBase.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core/mostlyharmless_ISharedState.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core/mostlyharmless_IEngine.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core/mostlyharmless_IEditor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core/mostlyharmless_IPluginEntry.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_TransportState.h
        ${CMAKE_CURRENT_SOURCE_DIR}/events/mostlyharmless_InputEventContext.h
        ${CMAKE_CURRENT_SOURCE_DIR}/events/mostlyharmless_WebEvent.h
        ${CMAKE_CURRENT_SOURCE_DIR}/events/mostlyharmless_ParamEvent.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_Parameters.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/mostlyharmless_WebviewBase.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/mostlyharmless_WebviewEditor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/mostlyharmless_Colour.h
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_TaskThread.h
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_Timer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/audio/mostlyharmless_AudioHelpers.h
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_Directories.h
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_OnScopeExit.h
        ${PLATFORM_HEADERS}
        PARENT_SCOPE)