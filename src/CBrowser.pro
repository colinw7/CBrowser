TEMPLATE = app

QT += widgets

TARGET = CBrowser

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++11

CONFIG += debug

# Input
SOURCES += \
CBrowserAddress.cpp \
CBrowserBaseFont.cpp \
CBrowserBlockQuote.cpp \
CBrowserBody.cpp \
CBrowserBreak.cpp \
CBrowserCanvas.cpp \
CBrowserCenter.cpp \
CBrowserCode.cpp \
CBrowserWindowWidget.cpp \
CBrowserDiv.cpp \
CBrowserDocument.cpp \
CBrowserFile.cpp \
CBrowserFont.cpp \
CBrowserForm.cpp \
CBrowserGraphics.cpp \
CBrowserHead.cpp \
CBrowserHeader.cpp \
CBrowserHistory.cpp \
CBrowserHtml.cpp \
CBrowserIFace.cpp \
CBrowserImage.cpp \
CBrowserKbd.cpp \
CBrowserLabel.cpp \
CBrowserLink.cpp \
CBrowserList.cpp \
CBrowserMain.cpp \
CBrowserMeta.cpp \
CBrowserMisc.cpp \
CBrowserNamedImage.cpp \
CBrowserNoBr.cpp \
CBrowserObject.cpp \
CBrowserObjProp.cpp \
CBrowserOutput.cpp \
CBrowserOutputList.cpp \
CBrowserOutputTag.cpp \
CBrowserParagraph.cpp \
CBrowserPre.cpp \
CBrowserRule.cpp \
CBrowserSamp.cpp \
CBrowserScript.cpp \
CBrowserStyle.cpp \
CBrowserSymbol.cpp \
CBrowserTable.cpp \
CBrowserText.cpp \
CBrowserTitle.cpp \
CBrowserTT.cpp \
CBrowserXmp.cpp \
CBrowserWindow.cpp \
\
CBrowserDomTree.cpp \
\
CBrowserCeil.cpp \
CBrowserJS.cpp \
\
CQJCanvasContext2D.cpp \
CQJCanvas.cpp \
CQJCanvasFontMetrics.cpp \
CQJCanvasImageDataArray.cpp \
CQJCanvasImageData.cpp \
CQJCanvasLinearGradient.cpp \
CQJCanvasPattern.cpp \
CQJCanvasRadialGradient.cpp \
CQJCanvasWidget.cpp \
CQJDialog.cpp \
CQJDocument.cpp \
CQJEvent.cpp \
CQJHtmlObj.cpp \
CQJImage.cpp \
CQJInput.cpp \
CQJIntervalFunction.cpp \
CQJColorEdit.cpp \
CQJLineEdit.cpp \
CQJComboBox.cpp \
CQJSlider.cpp \
CQJObject.cpp \
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
CBrowserAddress.h \
CBrowserBaseFont.h \
CBrowserBlockQuote.h \
CBrowserBody.h \
CBrowserBreak.h \
CBrowserCanvas.h \
CBrowserCenter.h \
CBrowserDiv.cpp \
CBrowserDocument.h \
CBrowserFile.h \
CBrowserFont.h \
CBrowserForm.h \
CBrowserGraphics.h \
CBrowserHead.h \
CBrowserHeader.h \
CBrowserHistory.h \
CBrowserHtml.h \
CBrowserHtmlI.h \
CBrowserIFace.h \
CBrowserImage.h \
CBrowserKbd.h \
CBrowserLabel.h \
CBrowserLink.h \
CBrowserList.h \
CBrowserMain.h \
CBrowserMeta.h \
CBrowserMisc.h \
CBrowserNamedImage.h \
CBrowserNoBr.h \
CBrowserObject.h \
CBrowserObjProp.h \
CBrowserOutput.h \
CBrowserOutputList.h \
CBrowserParagraph.h \
CBrowserPre.h \
CBrowserRule.h \
CBrowserSamp.h \
CBrowserScript.h \
CBrowserStyle.h \
CBrowserSymbol.h \
CBrowserTable.h \
CBrowserText.h \
CBrowserTitle.h \
CBrowserTT.h \
CBrowserTypes.h \
CBrowserXmp.h \
CBrowserWindow.h \
CBrowserWindowWidget.h \
\
CBrowserDomTree.h \
\
CBrowserCeil.h \
CBrowserJS.h \
\
CQJCanvasContext2D.h \
CQJCanvasFontMetrics.h \
CQJCanvas.h \
CQJCanvasImageDataArray.h \
CQJCanvasImageData.h \
CQJCanvasLinearGradient.h \
CQJCanvasPattern.h \
CQJCanvasRadialGradient.h \
CQJCanvasWidget.h \
CQJDialog.h \
CQJDocument.h \
CQJEvent.h \
CQJHtmlObj.h \
CQJImage.h \
CQJInput.h \
CQJIntervalFunction.h \
CQJColorEdit.h \
CQJLineEdit.h \
CQJComboBox.h \
CQJSlider.h \
CQJObject.h \
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
