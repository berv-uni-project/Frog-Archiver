set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM 1)

find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS
    Core
    LinguistTools
)

set ( app_TRS
    FrogArchiver_en.ts
    FrogArchiver_es.ts
    FrogArchiver_fr.ts
)

qt_create_translation(TRS ${CMAKE_SOURCE_DIR} ${app_TRS} OPTIONS -no-ui-lines -locations none)
add_custom_target(app_translations ALL DEPENDS ${TRS})

if(UNIX AND NOT APPLE)
    install(FILES ${TRS} DESTINATION share/frogarchiver/i18n)
elseif(WIN32)
    install(FILES ${TRS} DESTINATION i18n)
elseif(APPLE)
    set(MAC_LANG_PATH Frog_Archiver.app/Contents/MacOS)
    set(MAC_QT_LANG_PATH ${Qt${QT_DEFAULT_MAJOR_VERSION}_DIR}/../../../translations)
    install(FILES ${TRS} DESTINATION ${MAC_LANG_PATH}/i18n)
    install(FILES ${MAC_QT_LANG_PATH}/qtbase_en.qm DESTINATION ${MAC_LANG_PATH}/translations RENAME qt_en.qm)
    install(FILES ${MAC_QT_LANG_PATH}/qtbase_es.qm DESTINATION ${MAC_LANG_PATH}/translations RENAME qt_es.qm)
    install(FILES ${MAC_QT_LANG_PATH}/qtbase_fr.qm DESTINATION ${MAC_LANG_PATH}/translations RENAME qt_fr.qm)
endif()
