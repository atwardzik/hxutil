cmake_minimum_required(VERSION 3.19)
project(hxutil LANGUAGES CXX)


### LANGUAGE STANDARDS ###
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/Users/arturtwardzik/Qt/6.8.2/macos/lib/cmake")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
#set(CMAKE_CXX_SCAN_FOR_MODULES ON)


### ICON ###
set(MACOSX_BUNDLE_ICON_FILE icon.icns)
set(app_icon_macos "${CMAKE_SOURCE_DIR}/Contents/Resources/icon.icns")
set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")


### INCLUDES ###
include_directories(${CMAKE_SOURCE_DIR} ui include src)
link_directories(${CMAKE_SOURCE_DIR} ui include src)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()


### SOURCES ###
qt_add_executable(hxutil
        MACOSX_BUNDLE
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        ui/mainwindow.ui
        include/file_modifier.h
        include/highlighter.h
        src/highlighter/highlighter.cpp
        include/code_editor.h
        src/code_editor.cpp
        include/color_palette.h
        src/highlighter/ARMv6_ASM_highlighter.cpp
        include/open_files_list.h
        #        src/binary_file_handler.cppm
        ${app_icon_macos}
)

#file(GLOB_RECURSE MODULE_INTERFACE_UNITS "*.cppm")
#target_sources(hxutil
#        PUBLIC
#        FILE_SET modules TYPE CXX_MODULES FILES
#        ${MODULE_INTERFACE_UNITS})

target_link_libraries(hxutil
        PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS hxutil
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
        TARGET hxutil
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
