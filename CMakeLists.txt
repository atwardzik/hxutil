cmake_minimum_required(VERSION 3.19)
project(hxdump LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/Users/arturtwardzik/Qt/6.8.2/macos/lib/cmake")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
#set(CMAKE_CXX_SCAN_FOR_MODULES ON)

include_directories(${CMAKE_SOURCE_DIR} ui include src)
link_directories(${CMAKE_SOURCE_DIR} ui include src)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(hxdump
        WIN32 MACOSX_BUNDLE
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        ui/mainwindow.ui
        include/hex_mod.h
        include/highlighter.h
        src/highlighter.cpp
        #        src/binary_file_handler.cppm
)

#file(GLOB_RECURSE MODULE_INTERFACE_UNITS "*.cppm")
#target_sources(hxdump
#        PUBLIC
#        FILE_SET modules TYPE CXX_MODULES FILES
#        ${MODULE_INTERFACE_UNITS})

target_link_libraries(hxdump
        PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS hxdump
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
        TARGET hxdump
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
