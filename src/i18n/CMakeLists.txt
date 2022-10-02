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

install(FILES ${TRS} DESTINATION i18n)
