TEMPLATE = app

QT += widgets webkitwidgets

TARGET = CBrowser

DEPENDPATH += .

MOC_DIR = .moc

QMAKE_CXXFLAGS += -std=c++11

CONFIG += debug

# Input
SOURCES += \
CBrowserAddress.cpp \
CBrowserArea.cpp \
CBrowserBaseFont.cpp \
CBrowserBlockQuote.cpp \
CBrowserBody.cpp \
CBrowserBox.cpp \
CBrowserBreak.cpp \
CBrowserCanvas.cpp \
CBrowserCenter.cpp \
CBrowserCode.cpp \
CBrowserColor.cpp \
CBrowser.cpp \
CBrowserCSSStyle.cpp \
CBrowserCSSTree.cpp \
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
CBrowserLayout.cpp \
CBrowserLine.cpp \
CBrowserLink.cpp \
CBrowserList.cpp \
CBrowserListItem.cpp \
CBrowserMain.cpp \
CBrowserMeta.cpp \
CBrowserMisc.cpp \
CBrowserNamedImage.cpp \
CBrowserNoBr.cpp \
CBrowserObject.cpp \
CBrowserObjProp.cpp \
CBrowserOutput.cpp \
CBrowserOutputTag.cpp \
CBrowserParagraph.cpp \
CBrowserPre.cpp \
CBrowserRenderer.cpp \
CBrowserRule.cpp \
CBrowserSamp.cpp \
CBrowserScript.cpp \
CBrowserScrolledWindow.cpp \
CBrowserStyle.cpp \
CBrowserStyleData.cpp \
CBrowserTable.cpp \
CBrowserText.cpp \
CBrowserTitle.cpp \
CBrowserTT.cpp \
CBrowserWebView.cpp \
CBrowserWindow.cpp \
CBrowserWindowWidget.cpp \
CBrowserWord.cpp \
CBrowserXmp.cpp \
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
CQJColorEdit.cpp \
CQJComboBox.cpp \
CQJDialog.cpp \
CQJDocument.cpp \
CQJEvent.cpp \
CQJForm.cpp \
CQJHtmlCollection.cpp \
CQJHtmlObj.cpp \
CQJImage.cpp \
CQJInput.cpp \
CQJIntervalFunction.cpp \
CQJLineEdit.cpp \
CQJLocation.cpp \
CQJObject.cpp \
CQJRequestAnimationFrame.cpp \
CQJSlider.cpp \
CQJWindow.cpp \
CQJWindowTimer.cpp \
\
CQHistoryLineEdit.cpp \
\
CRomanNumber.cpp \
\
CPrint.cpp \

HEADERS += \
CBrowserAddress.h \
CBrowserArea.h \
CBrowserBackground.h \
CBrowserBaseFont.h \
CBrowserBlockQuote.h \
CBrowserBody.h \
CBrowserBorder.h \
CBrowserBox.h \
CBrowserBreak.h \
CBrowserCanvas.h \
CBrowserCenter.h \
CBrowserClear.h \
CBrowserCode.h \
CBrowserColor.h \
CBrowserCSSData.h \
CBrowserCSSStyle.h \
CBrowserCSSTree.h \
CBrowserData.h \
CBrowserDiv.h \
CBrowserDocument.h \
CBrowserFile.h \
CBrowserFloat.h \
CBrowserFont.h \
CBrowserForm.h \
CBrowserGraphics.h \
CBrowserHeader.h \
CBrowserHead.h \
CBrowserHistory.h \
CBrowserHtml.h \
CBrowserIFace.h \
CBrowserImage.h \
CBrowserKbd.h \
CBrowserLayout.h \
CBrowserLine.h \
CBrowserLink.h \
CBrowserList.h \
CBrowserListItem.h \
CBrowserMain.h \
CBrowserMargin.h \
CBrowserMeta.h \
CBrowserMisc.h \
CBrowserNamedImage.h \
CBrowserNoBr.h \
CBrowserObject.h \
CBrowserObjProp.h \
CBrowserOutput.h \
CBrowserOutputTag.h \
CBrowserPadding.h \
CBrowserParagraph.h \
CBrowserPixmaps.h \
CBrowserPre.h \
CBrowserProperty.h \
CBrowserRegion.h \
CBrowserRenderer.h \
CBrowserRule.h \
CBrowserSamp.h \
CBrowserScript.h \
CBrowserScrolledWindow.h \
CBrowserStyleData.h \
CBrowserStyle.h \
CBrowserTable.h \
CBrowserText.h \
CBrowserTextProp.h \
CBrowserTitle.h \
CBrowserTT.h \
CBrowserTypes.h \
CBrowserUnitValue.h \
CBrowserWebView.h \
CBrowserWindow.h \
CBrowserWindowWidget.h \
CBrowserWord.h \
CBrowserXmp.h \
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
CQJColorEdit.h \
CQJComboBox.h \
CQJDialog.h \
CQJDocument.h \
CQJEvent.h \
CQJForm.h \
CQJHtmlCollection.h \
CQJHtmlObj.h \
CQJImage.h \
CQJInput.h \
CQJIntervalFunction.h \
CQJLineEdit.h \
CQJLocation.h \
CQJObject.h \
CQJRequestAnimationFrame.h \
CQJSlider.h \
CQJUtil.h \
CQJWindow.h \
CQJWindowTimer.h \
\
CQHistoryLineEdit.h \
\
CRomanNumber.h \
\
CPrint.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. \
../include \
../../CJavaScript/include \
../../CJson/include \
../../CHtml/include \
../../CCSS/include \
../../CWebGet/include \
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
-L../../CJavaScript/lib \
-L../../CJson/lib \
-L../../CHtml/lib \
-L../../CCSS/lib \
-L../../CWebGet/lib \
-L../../CHtml/lib \
-L../../CXML/lib \
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
-L../../CThread/lib \
-L../../CUtil/lib \
-L../../COS/lib \
-L../../CRGBName/lib \
-lCJavaScript -lCJson -lCHtml -lCCSS -lCXML -lCWebGet \
-lCQUtil -lCImageLib -lCFont -lCCeil -lCArgs -lCConfig -lCReadLine \
-lCFile -lCFileUtil -lCStrUtil -lCGlob -lCRegExp -lCRGBName -lCUtil -lCOS \
-lCThread -ljpeg -lpng -lcurses -ltre
