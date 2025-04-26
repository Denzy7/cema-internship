cmake_minimum_required(VERSION 3.16)

project(patientmgr_qt VERSION 0.1 LANGUAGES CXX)

set(mingw_copylibs_DIR "${patientmgr_qt_SOURCE_DIR}/../../deps/mingw-copylibs")
include(${mingw_copylibs_DIR}/mingw_copylibs.cmake)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(patientmgr_qt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET patientmgr_qt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(patientmgr_qt SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(patientmgr_qt
            ${PROJECT_SOURCES}
            programmes.h programmes.cpp programmes.ui
            request.h request.cpp
        )
    endif()
endif()

target_link_libraries(patientmgr_qt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.patientmgr_qt)
endif()
set_target_properties(patientmgr_qt PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS patientmgr_qt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(MINGW)

    mingw_copylibs_install(${CMAKE_INSTALL_BINDIR}/patientmgr_qt.exe)
    foreach(pfx ${CMAKE_SYSTEM_PREFIX_PATH})
        # assumes we have winpthread, may not be case but it's always there
        if(EXISTS "${pfx}/bin/libwinpthread-1.dll")
            if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
                set(pluginsdir
                    lib/qt6/plugins
                    share/qt6/plugins
                )
            else()
                set(pluginsdir 
                    lib/qt5/plugins
                    share/qt5/plugins
                    lib/qt/plugins
                    share/qt/plugins
                )
            endif()

            foreach(plugindir ${pluginsdir})
                if(EXISTS "${pfx}/${plugindir}")
                    install(DIRECTORY 
                        ${pfx}/${plugindir}/platforms
                        ${pfx}/${plugindir}/styles
                        DESTINATION ${CMAKE_INSTALL_BINDIR}/${plugindir}
                    )
                endif()
            endforeach()
        endif()
    endforeach()
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(patientmgr_qt)
endif()
