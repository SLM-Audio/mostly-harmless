find_package(Doxygen)
if(Doxygen_FOUND)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_EXTRACT_LOCAL_METHODS YES)
    if(NOT APPLE)
        # include the platform specific stuff anyway..
        set(MOSTLYHARMLESS_EXTRA_DOC_HEADERS
                ${CMAKE_SOURCE_DIR}/include/mostly_harmless/gui/platform/mostlyharmless_GuiHelpersMacOS.h)
    endif()
    message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})

    add_subdirectory(${CMAKE_SOURCE_DIR}/modules/marvin/include ${CMAKE_CURRENT_BINARY_DIR}/marvin)
    doxygen_add_docs(
            mostly_harmless_docs
            ${MARVIN_HEADERS}
            ${CMAKE_SOURCE_DIR}/modules/marvin/docs/marvin_NamespaceDocs.h
            ${MOSTLYHARMLESS_HEADERS}
            ${CMAKE_CURRENT_SOURCE_DIR}/mostlyharmless_NamespaceDocs.h
            ${MOSTLYHARMLESS_EXTRA_DOC_HEADERS}
            USE_STAMP_FILE
    )
endif()
