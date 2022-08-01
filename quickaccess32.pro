#-------------------------------------------------
#
# Project created by QtCreator 2016-08-09T18:23:17
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = quickaccess
TARGET_EXT = .dp32
TEMPLATE = lib

DEFINES += QUICKACCESS_LIBRARY

SOURCES += quickaccess.cpp \
    pluginmain.cpp \
    helper.cpp
INCLUDEPATH += pluginsdk
HEADERS += quickaccess.h\
        quickaccess_global.h \
    pluginmain.h \
    pluginsdk/capstone/arm.h \
    pluginsdk/capstone/arm64.h \
    pluginsdk/capstone/capstone.h \
    pluginsdk/capstone/m68k.h \
    pluginsdk/capstone/mips.h \
    pluginsdk/capstone/platform.h \
    pluginsdk/capstone/ppc.h \
    pluginsdk/capstone/sparc.h \
    pluginsdk/capstone/systemz.h \
    pluginsdk/capstone/x86.h \
    pluginsdk/capstone/xcore.h \
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
    pluginsdk/yara/yara/ahocorasick.h \
    pluginsdk/yara/yara/arena.h \
    pluginsdk/yara/yara/atoms.h \
    pluginsdk/yara/yara/compiler.h \
    pluginsdk/yara/yara/elf.h \
    pluginsdk/yara/yara/error.h \
    pluginsdk/yara/yara/exec.h \
    pluginsdk/yara/yara/exefiles.h \
    pluginsdk/yara/yara/filemap.h \
    pluginsdk/yara/yara/globals.h \
    pluginsdk/yara/yara/hash.h \
    pluginsdk/yara/yara/hex_lexer.h \
    pluginsdk/yara/yara/lexer.h \
    pluginsdk/yara/yara/libyara.h \
    pluginsdk/yara/yara/limits.h \
    pluginsdk/yara/yara/mem.h \
    pluginsdk/yara/yara/modules.h \
    pluginsdk/yara/yara/object.h \
    pluginsdk/yara/yara/parser.h \
    pluginsdk/yara/yara/pe.h \
    pluginsdk/yara/yara/proc.h \
    pluginsdk/yara/yara/re.h \
    pluginsdk/yara/yara/re_lexer.h \
    pluginsdk/yara/yara/rules.h \
    pluginsdk/yara/yara/scan.h \
    pluginsdk/yara/yara/sizedstr.h \
    pluginsdk/yara/yara/stream.h \
    pluginsdk/yara/yara/strutils.h \
    pluginsdk/yara/yara/threading.h \
    pluginsdk/yara/yara/types.h \
    pluginsdk/yara/yara/utils.h \
    pluginsdk/yara/yara.h \
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
    helper.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/pluginsdk/ -lx64dbg
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/pluginsdk/ -lx64dbgd
else:unix: LIBS += -L$$PWD/pluginsdk/ -lx64dbg

INCLUDEPATH += $$PWD/pluginsdk
DEPENDPATH += $$PWD/pluginsdk
