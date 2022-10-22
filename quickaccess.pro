#-------------------------------------------------
#
# Project created by QtCreator 2016-08-09T18:23:17
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = quickaccess
TEMPLATE = lib

SOURCES += \
    pluginmain.cpp \
    quickaccess.cpp \

INCLUDEPATH += pluginsdk

HEADERS += \
    pluginmain.h \
    quickaccess.h \
    pluginsdk/dbghelp/dbghelp.h \
    pluginsdk/DeviceNameResolver/DeviceNameResolver.h \
    pluginsdk/jansson/jansson.h \
    pluginsdk/jansson/jansson_config.h \
    pluginsdk/jansson/jansson_x64dbg.h \
    pluginsdk/lz4/lz4.h \
    pluginsdk/lz4/lz4file.h \
    pluginsdk/lz4/lz4hc.h \
    pluginsdk/TitanEngine/TitanEngine.h \
    pluginsdk/XEDParse/XEDParse.h \
    pluginsdk/_dbgfunctions.h \
    pluginsdk/_plugin_types.h \
    pluginsdk/_plugins.h \
    pluginsdk/_scriptapi.h \
    pluginsdk/_scriptapi_argument.h \
    pluginsdk/_scriptapi_assembler.h \
    pluginsdk/_scriptapi_bookmark.h \
    pluginsdk/_scriptapi_comment.h \
    pluginsdk/_scriptapi_debug.h \
    pluginsdk/_scriptapi_flag.h \
    pluginsdk/_scriptapi_function.h \
    pluginsdk/_scriptapi_gui.h \
    pluginsdk/_scriptapi_label.h \
    pluginsdk/_scriptapi_memory.h \
    pluginsdk/_scriptapi_misc.h \
    pluginsdk/_scriptapi_module.h \
    pluginsdk/_scriptapi_pattern.h \
    pluginsdk/_scriptapi_register.h \
    pluginsdk/_scriptapi_stack.h \
    pluginsdk/_scriptapi_symbol.h \
    pluginsdk/bridgegraph.h \
    pluginsdk/bridgelist.h \
    pluginsdk/bridgemain.h \

contains(QMAKE_HOST.arch, x86_64) {
    LIBS += -L$$PWD/pluginsdk/ -lx64bridge -lx64dbg
    TARGET_EXT = .dp64
} else {
    LIBS += -L$$PWD/pluginsdk/ -lx32bridge -lx32dbg
    TARGET_EXT = .dp32
}
