if (APPLE)
    set(PLATFORM_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/gui/platform/mostlyharmless_GuiHelpersMacOS.mm
            ${CMAKE_CURRENT_SOURCE_DIR}/utils/platform/mostlyharmless_DirectoriesMacOS.mm
    )
else ()
    set(PLATFORM_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/utils/platform/mostlyharmless_DirectoriesWindows.cpp
    )
endif ()
set(MOSTLYHARMLESS_SOURCES
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_Entry.cpp
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_PluginBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_Parameters.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_TransportState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/mostlyharmless_IPluginEntry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/mostlyharmless_IEditor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/mostlyharmless_ISharedState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/mostlyharmless_SharedStateContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/audio/mostlyharmless_AudioHelpers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/events/mostlyharmless_InputEventContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/events/mostlyharmless_ParamEvent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/events/mostlyharmless_WebEvent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/mostlyharmless_WebviewBase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/mostlyharmless_WebviewEditor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/mostlyharmless_Colour.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_TaskThread.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_Timer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_OnScopeExit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_NoDenormals.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_Hash.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/mostlyharmless_Macros.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/data/mostlyharmless_DatabaseState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/data/mostlyharmless_DatabasePropertyListener.cpp
        ${PLATFORM_SOURCES}
        PARENT_SCOPE)
message(STATUS ${MOSTLYHARMLESS_PLATFORM_SOURCES})