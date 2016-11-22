TEMPLATE = app

QT += widgets

TARGET = CBrowser

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++11

CONFIG += debug

# Input
SOURCES += \
CBrowserBreak.cpp \
CBrowserCanvas.cpp \
CBrowserCanvasWidget.cpp \
CBrowserCeil.cpp \
CBrowser.cpp \
CBrowserDocument.cpp \
CBrowserFile.cpp \
CBrowserFont.cpp \
CBrowserForm.cpp \
CBrowserGraphics.cpp \
CBrowserHistory.cpp \
CBrowserIFace.cpp \
CBrowserImage.cpp \
CBrowserJS.cpp \
CBrowserLabel.cpp \
CBrowserLink.cpp \
CBrowserMain.cpp \
CBrowserMisc.cpp \
CBrowserNamedImage.cpp \
CBrowserObject.cpp \
CBrowserOutput.cpp \
CBrowserRule.cpp \
CBrowserSymbol.cpp \
CBrowserTable.cpp \
CBrowserText.cpp \
CBrowserWindow.cpp \
CBrowserWindowWidget.cpp \
\
CQJCanvasContext2D.cpp \
CQJCanvas.cpp \
CQJCanvasFontMetrics.cpp \
CQJCanvasImageDataArray.cpp \
CQJCanvasImageData.cpp \
CQJCanvasLinearGradient.cpp \
CQJCanvasPattern.cpp \
CQJCanvasRadialGradient.cpp \
CQJColorEdit.cpp \
CQJComboBox.cpp \
CQJDialog.cpp \
CQJDocument.cpp \
CQJEvent.cpp \
CQJHtmlObj.cpp \
CQJImage.cpp \
CQJInput.cpp \
CQJLineEdit.cpp \
CQJObject.cpp \
CQJSlider.cpp \
CQJWindow.cpp \
CQJWindowTimer.cpp \
\
CHtmlLayoutArea.cpp \
CHtmlLayoutBox.cpp \
CHtmlLayoutCell.cpp \
CHtmlLayoutMgr.cpp \
CHtmlLayoutSubCell.cpp \
\
CQHistoryLineEdit.cpp \
\
CRomanNumber.cpp \
\
CPrint.cpp \
\
CUrl.cpp \

HEADERS += \
CBrowserBreak.h \
CBrowserCanvas.h \
CBrowserCanvasWidget.h \
CBrowserCeil.h \
CBrowserData.h \
CBrowserDocument.h \
CBrowserFile.h \
CBrowserFont.h \
CBrowserForm.h \
CBrowserGraphics.h \
CBrowserHistory.h \
CBrowserHtmlI.h \
CBrowserIFace.h \
CBrowserImage.h \
CBrowserJS.h \
CBrowserLabel.h \
CBrowserLink.h \
CBrowserMain.h \
CBrowserMisc.h \
CBrowserNamedImage.h \
CBrowserObject.h \
CBrowserOutput.h \
CBrowserPixmaps.h \
CBrowserRule.h \
CBrowserSymbol.h \
CBrowserTable.h \
CBrowserText.h \
CBrowserTypes.h \
CBrowserWindow.h \
CBrowserWindowWidget.h \
\
CQJCanvasContext2D.h \
CQJCanvasFontMetrics.h \
CQJCanvas.h \
CQJCanvasImageDataArray.h \
CQJCanvasImageData.h \
CQJCanvasLinearGradient.h \
CQJCanvasPattern.h \
CQJCanvasRadialGradient.h \
CQJColorEdit.h \
CQJComboBox.h \
CQJDialog.h \
CQJDocument.h \
CQJEvent.h \
CQJHtmlObj.h \
CQJImage.h \
CQJInput.h \
CQJLineEdit.h \
CQJObject.h \
CQJSlider.h \
CQJUtil.h \
CQJWindow.h \
CQJWindowTimer.h \
\
CHtmlLayoutArea.h \
CHtmlLayoutBox.h \
CHtmlLayoutCell.h \
CHtmlLayout.h \
CHtmlLayoutMgr.h \
CHtmlLayoutSubCell.h \
CHtmlLayoutTypes.h \
CHtmlLayoutVisitor.h \
\
CQHistoryLineEdit.h \
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
../../CJavaScript/include \
../../CJson/include \
../../CHtml/include \
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
-L../../CHtml/lib \
-L../../CJson/lib \
-L../../CJavaScript/lib \
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
-lCJavaScript -lCJson -lCHtml -lCQUtil -lCImageLib -lCFont -lCCeil -lCArgs -lCConfig -lCReadLine \
-lCFile -lCFileUtil -lCStrUtil -lCGlob -lCRegExp -lCRGBName -lCUtil -lCOS \
-ljpeg -lpng -lcurses -ltre
