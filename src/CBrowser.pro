TEMPLATE = app

QT += widgets webkitwidgets

TARGET = CBrowser

DEPENDPATH += .

MOC_DIR = .moc

QMAKE_CXXFLAGS += \
-std=c++17 \
-DCQUTIL_PEN \
-DCQUTIL_BRUSH \
-DCQUTIL_FONT \
-DCQUTIL_IMAGE \
-DCBRUSH_IMAGE \

CONFIG += debug

# Input
SOURCES += \
CBrowser.cpp \
\
CBrowserAddress.cpp \
CBrowserArea.cpp \
CBrowserBaseFont.cpp \
CBrowserBlockQuote.cpp \
CBrowserBody.cpp \
CBrowserBox.cpp \
CBrowserBreak.cpp \
CBrowserCanvas.cpp \
CBrowserCeil.cpp \
CBrowserCenter.cpp \
CBrowserCode.cpp \
CBrowserColor.cpp \
CBrowserCSSStyle.cpp \
CBrowserCSSTree.cpp \
CBrowserDiv.cpp \
CBrowserDocument.cpp \
CBrowserDomTree.cpp \
CBrowserFile.cpp \
CBrowserFont.cpp \
CBrowserForm.cpp \
CBrowserGraphics.cpp \
CBrowserHead.cpp \
CBrowserHeader.cpp \
CBrowserHistory.cpp \
CBrowserHtml.cpp \
CBrowserIFrame.cpp \
CBrowserImage.cpp \
CBrowserKbd.cpp \
CBrowserLayout.cpp \
CBrowserLine.cpp \
CBrowserLink.cpp \
CBrowserList.cpp \
CBrowserListItem.cpp \
CBrowserMain.cpp \
CBrowserMainWindow.cpp \
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
CBrowserPrintRenderer.cpp \
CBrowserRenderer.cpp \
CBrowserRule.cpp \
CBrowserSamp.cpp \
CBrowserScript.cpp \
CBrowserScrolledWindow.cpp \
CBrowserStyle.cpp \
CBrowserStyleData.cpp \
CBrowserSVG.cpp \
CBrowserTable.cpp \
CBrowserText.cpp \
CBrowserTitle.cpp \
CBrowserTT.cpp \
CBrowserVideo.cpp \
CBrowserWebView.cpp \
CBrowserWindow.cpp \
CBrowserWindowIFace.cpp \
CBrowserWindowWidget.cpp \
CBrowserWindowWidgetIFace.cpp \
CBrowserWord.cpp \
CBrowserXmp.cpp \
\
CQSVGRenderer.cpp \
CQSVGImageData.cpp \
CQHtmlTextPainter.cpp \
\
CRomanNumber.cpp \
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
CBrowserBoxSizing.h \
CBrowserBreak.h \
CBrowserCanvas.h \
CBrowserCeil.h \
CBrowserCenter.h \
CBrowserClear.h \
CBrowserCode.h \
CBrowserColor.h \
CBrowserCSSData.h \
CBrowserCSSStyle.h \
CBrowserCSSTree.h \
CBrowserCursor.h \
CBrowserData.h \
CBrowserDiv.h \
CBrowserDocument.h \
CBrowserDomTree.h \
CBrowserFile.h \
CBrowserFloat.h \
CBrowserFont.h \
CBrowserForm.h \
CBrowserGraphics.h \
CBrowserHeader.h \
CBrowserHead.h \
CBrowserHistory.h \
CBrowserHtml.h \
CBrowserIFrame.h \
CBrowserImage.h \
CBrowserKbd.h \
CBrowserLayout.h \
CBrowserLine.h \
CBrowserLink.h \
CBrowserList.h \
CBrowserListItem.h \
CBrowserMain.h \
CBrowserMainWindow.h \
CBrowserMargin.h \
CBrowserMeta.h \
CBrowserMisc.h \
CBrowserNamedImage.h \
CBrowserNoBr.h \
CBrowserObjectCSSTagData.h \
CBrowserObject.h \
CBrowserObjProp.h \
CBrowserOutline.h \
CBrowserOutput.h \
CBrowserOutputTag.h \
CBrowserOverflow.h \
CBrowserPadding.h \
CBrowserParagraph.h \
CBrowserPixmaps.h \
CBrowserPosition.h \
CBrowserPre.h \
CBrowserProperty.h \
CBrowserRegion.h \
CBrowserRenderer.h \
CBrowserRule.h \
CBrowserSamp.h \
CBrowserScript.h \
CBrowserScrolledWindow.h \
CBrowserShadow.h \
CBrowserSize.h \
CBrowserStyleData.h \
CBrowserStyle.h \
CBrowserSVG.h \
CBrowserTable.h \
CBrowserText.h \
CBrowserTextProp.h \
CBrowserTitle.h \
CBrowserTT.h \
CBrowserTypes.h \
CBrowserUnitValue.h \
CBrowserVideo.h \
CBrowserWebView.h \
CBrowserWindow.h \
CBrowserWindowIFace.h \
CBrowserWindowWidgetIFace.h \
CBrowserWindowWidget.h \
CBrowserWord.h \
CBrowserWordSpacing.h \
CBrowserXmp.h \
\
CQSVGRenderer.h \
CQSVGImageData.h \
CQHtmlTextPainter.h \
\
CTextBox.h \
CRomanNumber.h \
CPrint.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. \
../include \
../../CJavaScript/qinclude \
../../CJavaScript/include \
../../CJson/include \
../../CHtml/include \
../../CSVG/include \
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
-L../../CSVG/lib \
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
-L../../CMath/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../CGlob/lib \
-L../../CThread/lib \
-L../../CUtil/lib \
-L../../COS/lib \
-L../../CRGBName/lib \
-lCQJavaScript -lCJavaScript -lCJson -lCHtml -lCCSS -lCXML -lCWebGet -lCSVG \
-lCQUtil -lCImageLib -lCFont -lCCeil -lCArgs -lCConfig -lCReadLine \
-lCFile -lCFileUtil -lCMath -lCStrUtil -lCGlob -lCRegExp -lCRGBName -lCUtil -lCOS \
-lCThread -ljpeg -lpng -lcurses -ltre
