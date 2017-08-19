project(klook)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package(Qt5 REQUIRED COMPONENTS Core Gui Quick Qml)
find_package(KF5 REQUIRED COMPONENTS KDELibs4Support Declarative I18n)
find_package(Phonon4Qt5 REQUIRED)
include_directories(${PHONON_INCLUDE_DIR})

include(FeatureSummary)
include(KDECMakeSettings)
include(KDEInstallDirs)
include(ECMInstallIcons)
include(CheckIncludeFiles)


#include( ${QT_USE_FILE} )
set(CMAKE_AUTOMOC ON)
add_definitions( ${QT_DEFINITIONS})

add_subdirectory( icons )

set(EXIV2_MIN_VERSION "0.19")
find_package(Exiv2 REQUIRED)
set_package_properties("Exiv2" PROPERTIES
                       DESCRIPTION "Provides image metadata support"
                       URL "http://www.exiv2.org"
                       TYPE REQUIRED)

qt5_add_resources( RESOURCES src/resources.qrc )

file( GLOB qml_files "${CMAKE_CURRENT_SOURCE_DIR}/src/qml/*.qml" )

set( klook_SRCS src/main.cpp
                src/declarativeviewer.cpp
                src/video.cpp
                src/text.cpp
                src/file.cpp
                src/listitem.cpp
                src/previewgenerator.cpp
                src/previewprovider.cpp
                src/filemodel.cpp
                src/klookapp.cpp
                src/audio.cpp
                src/mimeprovider.cpp
                src/kpartsdeclarativeitem.cpp
                src/kpartswidget.cpp
                src/listitemcontent.cpp
                src/exifimageprovider.cpp
                src/rotatedimage.cpp
                ${qml_files} )

set_source_files_properties(src/rotatedimage.cpp PROPERTIES COMPILE_FLAGS ${KDE4_ENABLE_EXCEPTIONS})

kde4_add_app_icon( klook_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/icons/hi*-apps-klook.png" )
kde4_add_executable( klook ${klook_SRCS} ${QRC_SOURCES} )

target_link_libraries(klook
                           kparts
                           Exiv2
                           KF5::Declarative
                           KF5::KDELibs4Support
                           Qt5::Widgets
                      )


install( TARGETS klook ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${qml_files} DESTINATION ${DATA_INSTALL_DIR}/${PROJECT_NAME}/ )

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/qml/images/
                 DESTINATION ${DATA_INSTALL_DIR}/${PROJECT_NAME}/images )

#Translations
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/locale)
  find_package( Gettext REQUIRED )
  add_subdirectory( locale )
endif()

feature_summary(WHAT ALL)
