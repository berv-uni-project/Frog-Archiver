
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
get_target_property(qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${qmake_executable}" DIRECTORY)
if(WIN32 OR APPLE)
    if(WIN32)
        set(deployqtapp windeployqt)
    elseif(APPLE)
        set(deployqtapp macdeployqt)
    endif()
    find_program(PLATFORMDEPLOYQT ${deployqtapp} HINTS "${_qt_bin_dir}")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CXX_STANDARD 11)
add_subdirectory(i18n)
add_executable(frogarchiver WIN32 MACOSX_BUNDLE
  main.cpp
  mainwindow.cpp
  about.cpp
  huffmanencoding.cpp
  huffmandecoding.cpp
  license.cpp
  extractwindow.cpp
  compresswindow.cpp
  mainwindow.h
  about.h
  prioqueue.h
  huffmantree.h
  huffmanencoding.h
  huffmandecoding.h
  license.h
  extractwindow.h
  compresswindow.h
  mainwindow.ui
  about.ui
  license.ui
  extractwindow.ui
  compresswindow.ui
  FrogArchiver.qrc
)

if(APPLE)
    set_target_properties(frogarchiver PROPERTIES
      MACOSX_BUNDLE_BUNDLE_VERSION ${CMAKE_PROJECT_VERSION}
    )
    install(TARGETS frogarchiver BUNDLE DESTINATION .)
    target_compile_options(frogarchiver PRIVATE -stdlib=libc++)
    add_custom_command(
        TARGET frogarchiver POST_BUILD
        COMMAND ${PLATFORMDEPLOYQT} frogarchiver.app
    )
elseif(UNIX AND NOT APPLE)
    install(TARGETS frogarchiver RUNTIME DESTINATION bin)
    install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION share/licenses/frogarchive/)
elseif(WIN32)
    install(TARGETS frogarchiver RUNTIME DESTINATION .)
    install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION .)
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
    set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
    include(InstallRequiredSystemLibraries)
    add_custom_command(
            TARGET frogarchiver POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/qtDeploy
            COMMAND ${PLATFORMDEPLOYQT} --no-compiler-runtime --no-system-d3d-compiler --no-quick-import -xml -qml -network --dir ${CMAKE_BINARY_DIR}/qtDeploy $<TARGET_FILE:frogarchiver>
    )
    install(
        DIRECTORY ${CMAKE_BINARY_DIR}/qtDeploy/
        DESTINATION .
        FILES_MATCHING PATTERN "*.*"
    )
endif()

target_link_libraries(frogarchiver PRIVATE Qt::Core Qt::Gui Qt::Widgets)

#NOT SURE WHERE THESE SHOULD GO
install (
  FILES
    config/config.xml
    packages/org.berviantoleo.frogarchiver/packages.xml
  DESTINATION .
)

#PACKAGING
set(CPACK_STRIP_FILES TRUE)
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_NAME "Frog Archiver")
set(CPACK_PACKAGE_VENDOR "berv-uni-project")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/berv-uni-project/Frog-Archiver")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CMAKE_PROJECT_DESCRIPTION})

if(NOT CPACK_PACKAGE_VERSION)
    set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(CPACK_GENERATOR "ZIP")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(CPACK_DMG_VOLUME_NAME "Frog-Archiver")
    set(CPACK_GENERATOR "DragNDrop")
else()
  set(CPACK_GENERATOR "TGZ")
endif()

include (CPack)

