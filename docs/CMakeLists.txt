find_package(Doxygen)
if (Doxygen_FOUND)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_EXTRACT_LOCAL_METHODS YES)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_SOURCE_DIR}/README.md)
    if (NOT APPLE)
        # include the platform specific stuff anyway..
        set(MOSTLYHARMLESS_EXTRA_DOC_HEADERS
                ${CMAKE_SOURCE_DIR}/include/mostly_harmless/gui/platform/mostlyharmless_GuiHelpersMacOS.h)
    endif ()
    doxygen_add_docs(
            mostly_harmless_docs
            ${CMAKE_SOURCE_DIR}/README.md
            ${CMAKE_CURRENT_SOURCE_DIR}/FXPluginTutorial.md
            ${MOSTLYHARMLESS_HEADERS}
            ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_NamespaceDocs.h
            ${MOSTLYHARMLESS_EXTRA_DOC_HEADERS}
            USE_STAMP_FILE
    )
endif ()
