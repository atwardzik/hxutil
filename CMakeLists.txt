cmake_minimum_required(VERSION 3.19)
project(hxutil LANGUAGES CXX)


################################# LANGUAGE STANDARDS #################################
set(CMAKE_CXX_STANDARD 23)
add_compile_definitions(__cpp_lib_format=201907L)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/Users/arturtwardzik/Qt/6.8.3/macos/lib/cmake")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror \
                     -Wno-error=unused-variable -Wno-error=unused-parameter -Wno-error=unused-private-field \
                     -Wno-error=c++20-extensions -Wno-c++20-designator \
                     -fsanitize=address")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
#set(CMAKE_CXX_SCAN_FOR_MODULES ON)


################################# ICON #################################
set(MACOSX_BUNDLE_ICON_FILE icon.icns)
set(app_icon_macos "${CMAKE_SOURCE_DIR}/Contents/Resources/icon.icns")
set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")


################################# INCLUDES #################################
include_directories(${CMAKE_SOURCE_DIR} ui include src)
link_directories(${CMAKE_SOURCE_DIR} ui include src)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

find_package(Qt6 6.8.3 REQUIRED COMPONENTS WebEngineWidgets)
#target_link_libraries(hxutil PRIVATE Qt6::WebEngineWidgets)

qt_standard_project_setup()


################################# SOURCES #################################
qt_add_executable(hxutil
        MACOSX_BUNDLE
        ${app_icon_macos}
        include/color_palette.h
        include/lanugage_specifics.h
        src/main.cpp
        src/mainwindow.cpp include/mainwindow.h ui/mainwindow.ui
        src/settings.cpp include/settings.h ui/settings.ui
        src/code_editor.cpp include/code_editor.h
        include/tab_widget.h src/tab_widget.cpp
        src/file_modifier.cpp include/file_modifier.h
        src/highlighter/highlighter.cpp include/highlighter.h
        src/highlighter/ARMv6_ASM_highlighter.cpp
        include/file_viewer.h
        #        src/binary_file_handler.cppm
)

#file(GLOB_RECURSE MODULE_INTERFACE_UNITS "*.cppm")
#target_sources(hxutil
#        PUBLIC
#        FILE_SET modules TYPE CXX_MODULES FILES
#        ${MODULE_INTERFACE_UNITS})


################################# RUST SOURCES #################################
set(CARGO_CMD cargo build --lib --manifest-path include/asmstatic/Cargo.toml)
include_directories(include/asmstatic/include)
set(TARGET_DIR "debug")

set(LIB_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_DIR}/libASMformatter.a")
set(LIB_HEADER_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/include/asmstatic/include")

add_custom_command(OUTPUT ${LIB_FILE}
        COMMENT "Compiling rust module"
        COMMAND CARGO_TARGET_DIR=${CMAKE_CURRENT_BINARY_DIR} ${CARGO_CMD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(ASMformatter_lib_target DEPENDS ${LIB_FILE})

add_library(ASMformatter_lib STATIC IMPORTED GLOBAL)
add_dependencies(ASMformatter_lib ASMformatter_lib_target)
set_target_properties(ASMformatter_lib
        PROPERTIES
        IMPORTED_LOCATION ${LIB_FILE}
        INTERFACE_INCLUDE_DIRECTORIES ${LIB_HEADER_FOLDER})

target_compile_options(hxutil PRIVATE -fno-omit-frame-pointer -fsanitize=address)
target_link_libraries(hxutil PRIVATE ASMformatter_lib ${CMAKE_DL_LIBS} -fno-omit-frame-pointer -fsanitize=address)


################################# QT #################################
target_link_libraries(hxutil
        PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::WebEngineWidgets
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
