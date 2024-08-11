if (APPLE)
    set(PLATFORM_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/gui/platform/mostlyharmless_GuiHelpersMacOS.h)
elseif (WIN32)

endif ()
set(MOSTLYHARMLESS_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_Descriptor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_Plugin.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/mostlyharmless_PluginEditor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gui/mostlyharmless_WebviewEditor.h
        ${PLATFORM_HEADERS}
        PARENT_SCOPE)