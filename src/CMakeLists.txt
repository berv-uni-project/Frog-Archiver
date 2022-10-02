
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CXX_STANDARD 11)
add_subdirectory(i18n)
add_executable(frogarchiver
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

target_link_libraries(frogarchiver PRIVATE Qt::Core Qt::Gui Qt::Widgets)

install (TARGETS frogarchiver RUNTIME DESTINATION .)
install (
  FILES
    config/config.xml
    packages/org.berviantoleo.frogarchiver/packages.xml
  DESTINATION .
)


#PACKAGING
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_NAME "Frog Archiver")
set(CPACK_PACKAGE_VENDOR "berv-uni-project")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/berv-uni-project/Frog-Archiver")

if(NOT CPACK_GENERATOR)
  set(CPACK_GENERATOR "TGZ")
endif()
include (CPack)
