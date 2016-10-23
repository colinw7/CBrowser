TEMPLATE = app

QT += widgets

TARGET = CBrowser

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++11

CONFIG += debug

# Input
SOURCES += \
CBrowserBreak.cpp \
CBrowser.cpp \
CBrowserDocument.cpp \
CBrowserFile.cpp \
CBrowserFont.cpp \
CBrowserForm.cpp \
CBrowserGraphics.cpp \
CBrowserHistory.cpp \
CBrowserIFace.cpp \
CBrowserImage.cpp \
CBrowserLink.cpp \
CBrowserMain.cpp \
CBrowserMisc.cpp \
CBrowserOutput.cpp \
CBrowserRule.cpp \
CBrowserScript.cpp \
CBrowserSymbol.cpp \
CBrowserTable.cpp \
CBrowserText.cpp \
CBrowserWindow.cpp \
\
CHtmlComment.cpp \
CHtml.cpp \
CHtmlLayoutArea.cpp \
CHtmlLayoutCell.cpp \
CHtmlLayoutMgr.cpp \
CHtmlNamedChar.cpp \
CHtmlParser.cpp \
CHtmlTag.cpp \
CHtmlTagDef.cpp \
CHtmlText.cpp \
CHtmlToken.cpp \
CHtmlUtil.cpp \
\
CRomanNumber.cpp \
\
CPrint.cpp \
\
CUrl.cpp \

HEADERS += \
CBrowserBreakI.h \
CBrowserDocumentI.h \
CBrowserFileI.h \
CBrowserFontI.h \
CBrowserFormI.h \
CBrowserGraphicsI.h \
CBrowserHistoryI.h \
CBrowserHtmlI.h \
CBrowserIFaceI.h \
CBrowserImageI.h \
CBrowserLinkI.h \
CBrowserMain.h \
CBrowserMiscI.h \
CBrowserOutputI.h \
CBrowserRuleI.h \
CBrowserScriptI.h \
CBrowserSymbolI.h \
CBrowserTableI.h \
CBrowserTextI.h \
CBrowserTypesI.h \
CBrowserWindowI.h \
\
CHtmlComment.h \
CHtml.h \
CHtmlI.h \
CHtmlLayoutArea.h \
CHtmlLayoutCell.h \
CHtmlLayout.h \
CHtmlLayoutMgr.h \
CHtmlLayoutTypes.h \
CHtmlLib.h \
CHtmlNamedChar.h \
CHtmlParser.h \
CHtmlTagDef.h \
CHtmlTag.h \
CHtmlText.h \
CHtmlToken.h \
CHtmlTypes.h \
CHtmlUtil.h \
\
CRomanNumber.h \
\
CPrint.h \
\
CUrl.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. \
../include \
../../CQUtil/include \
../../CImageLib/include \
../../CFont/include \
../../CCeil/include \
../../CArgs/include \
../../CFile/include \
../../CFileUtil/include \
../../COS/include \
../../CStrUtil/include \
../../CRegExp/include \
../../CUtil/include \
../../CMath/include \
../../CGlob/include \
../../CReadLine/include \
../../CRGBName/include \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CQUtil/lib \
-L../../CImageLib/lib \
-L../../CFont/lib \
-L../../CCeil/lib \
-L../../CArgs/lib \
-L../../CConfig/lib \
-L../../CReadLine/lib \
-L../../CFile/lib \
-L../../CFileUtil/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../CGlob/lib \
-L../../CUtil/lib \
-L../../COS/lib \
-L../../CRGBName/lib \
-lCQUtil -lCImageLib -lCFont -lCCeil -lCArgs -lCConfig -lCReadLine -lCFile \
-lCFileUtil -lCStrUtil -lCGlob -lCRegExp -lCRGBName -lCUtil -lCOS \
-ljpeg -lpng -lcurses -ltre
