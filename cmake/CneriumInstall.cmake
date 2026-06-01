# ========================
# Cnerium install
# ========================

if(CNERIUM_INSTALL)
    install(
        TARGETS cnerium
        EXPORT cneriumTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(
        DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )

    install(
        EXPORT cneriumTargets
        FILE cneriumTargets.cmake
        NAMESPACE cnerium::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cnerium"
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/cneriumConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CneriumConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cneriumConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cnerium"
        PATH_VARS CMAKE_INSTALL_LIBDIR
    )

    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/cneriumConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/cneriumConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cnerium"
    )

    install(
        FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
            "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
            "${CMAKE_CURRENT_SOURCE_DIR}/CHANGELOG.md"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/cnerium"
        OPTIONAL
    )
endif()
