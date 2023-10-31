#ifndef CBrowserOutputTag_H
#define CBrowserOutputTag_H

#include <CHtmlLib.h>

class CBrowserWindowIFace;
class CBrowserObject;

class CBrowserOutputTagBase {
 public:
  explicit CBrowserOutputTagBase(CHtmlTagId id) :
   id_(id) {
  }

  virtual ~CBrowserOutputTagBase() { }

  const CHtmlTagId &id() const { return id_; }

  virtual CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) { return nullptr; }

  virtual void term(CBrowserWindowIFace *, CHtmlTag *) { }

  virtual void end(CBrowserWindowIFace *, CBrowserObject *, CHtmlTag *) { }

 private:
  CHtmlTagId id_;
};

class CBrowserOutputATag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputATag() : CBrowserOutputTagBase(CHtmlTagId::A) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputAbbrTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputAbbrTag() : CBrowserOutputTagBase(CHtmlTagId::ABBR) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputAcronymTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputAcronymTag() : CBrowserOutputTagBase(CHtmlTagId::ACRONYM) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputAddressTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputAddressTag() : CBrowserOutputTagBase(CHtmlTagId::ADDRESS) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputAppletTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputAppletTag() : CBrowserOutputTagBase(CHtmlTagId::APPLET) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputAreaTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputAreaTag() : CBrowserOutputTagBase(CHtmlTagId::AREA) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputArticleTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputArticleTag() : CBrowserOutputTagBase(CHtmlTagId::ARTICLE) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputASideTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputASideTag() : CBrowserOutputTagBase(CHtmlTagId::ASIDE) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputAudioTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputAudioTag() : CBrowserOutputTagBase(CHtmlTagId::AUDIO) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputBTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBTag() : CBrowserOutputTagBase(CHtmlTagId::B) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputBaseTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBaseTag() : CBrowserOutputTagBase(CHtmlTagId::BASE) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputBasefontTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBasefontTag() : CBrowserOutputTagBase(CHtmlTagId::BASEFONT) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputBdiTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBdiTag() : CBrowserOutputTagBase(CHtmlTagId::BDI) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputBdoTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBdoTag() : CBrowserOutputTagBase(CHtmlTagId::BDO) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputBigTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBigTag() : CBrowserOutputTagBase(CHtmlTagId::BIG) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputBlinkTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBlinkTag() : CBrowserOutputTagBase(CHtmlTagId::BLINK) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputBlockquoteTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBlockquoteTag() : CBrowserOutputTagBase(CHtmlTagId::BLOCKQUOTE) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputBodyTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBodyTag() : CBrowserOutputTagBase(CHtmlTagId::BODY) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputBrTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBrTag() : CBrowserOutputTagBase(CHtmlTagId::BR) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputButtonTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputButtonTag() : CBrowserOutputTagBase(CHtmlTagId::BUTTON) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputCanvasTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputCanvasTag() : CBrowserOutputTagBase(CHtmlTagId::CANVAS) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputCaptionTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputCaptionTag() : CBrowserOutputTagBase(CHtmlTagId::CAPTION) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputCenterTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputCenterTag() : CBrowserOutputTagBase(CHtmlTagId::CENTER) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputCiteTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputCiteTag() : CBrowserOutputTagBase(CHtmlTagId::CITE) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputCodeTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputCodeTag() : CBrowserOutputTagBase(CHtmlTagId::CODE) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

// CHtmlTagId::COL
// CHtmlTagId::COLGROUP
// CHtmlTagId::DATALIST

class CBrowserOutputDdTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDdTag() : CBrowserOutputTagBase(CHtmlTagId::DD) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputDelTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDelTag() : CBrowserOutputTagBase(CHtmlTagId::DEL) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputDetailsTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDetailsTag() : CBrowserOutputTagBase(CHtmlTagId::DETAILS) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputDfnTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDfnTag() : CBrowserOutputTagBase(CHtmlTagId::DFN) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputDirTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDirTag() : CBrowserOutputTagBase(CHtmlTagId::DIR) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputDivTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDivTag() : CBrowserOutputTagBase(CHtmlTagId::DIV) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputDlTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDlTag() : CBrowserOutputTagBase(CHtmlTagId::DL) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;

  void term(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputDtTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDtTag() : CBrowserOutputTagBase(CHtmlTagId::DT) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputEmTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputEmTag() : CBrowserOutputTagBase(CHtmlTagId::EM) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

// CHtmlTagId::EMBED

class CBrowserOutputFieldSetTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputFieldSetTag() : CBrowserOutputTagBase(CHtmlTagId::FIELDSET) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

// CHtmlTagId::FIGCAPTION

// CHtmlTagId::FIGURE

class CBrowserOutputFontTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputFontTag() : CBrowserOutputTagBase(CHtmlTagId::FONT) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputFooterTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputFooterTag() : CBrowserOutputTagBase(CHtmlTagId::FOOTER) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputFormTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputFormTag() : CBrowserOutputTagBase(CHtmlTagId::FORM) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

// CHtmlTagId::FRAME
// CHtmlTagId::FRAMESET

class CBrowserOutputH1Tag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputH1Tag() : CBrowserOutputTagBase(CHtmlTagId::H1) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputH2Tag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputH2Tag() : CBrowserOutputTagBase(CHtmlTagId::H2) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputH3Tag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputH3Tag() : CBrowserOutputTagBase(CHtmlTagId::H3) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputH4Tag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputH4Tag() : CBrowserOutputTagBase(CHtmlTagId::H4) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputH5Tag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputH5Tag() : CBrowserOutputTagBase(CHtmlTagId::H5) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputH6Tag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputH6Tag() : CBrowserOutputTagBase(CHtmlTagId::H6) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputHeadTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputHeadTag() : CBrowserOutputTagBase(CHtmlTagId::HEAD) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputHeaderTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputHeaderTag() : CBrowserOutputTagBase(CHtmlTagId::HEADER) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputHGroupTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputHGroupTag() : CBrowserOutputTagBase(CHtmlTagId::HGROUP) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputHrTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputHrTag() : CBrowserOutputTagBase(CHtmlTagId::HR) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputHtmlTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputHtmlTag() : CBrowserOutputTagBase(CHtmlTagId::HTML) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputITag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputITag() : CBrowserOutputTagBase(CHtmlTagId::I) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputIFrameTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputIFrameTag() : CBrowserOutputTagBase(CHtmlTagId::IFRAME) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputImgTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputImgTag() : CBrowserOutputTagBase(CHtmlTagId::IMG) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputInputTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputInputTag() : CBrowserOutputTagBase(CHtmlTagId::INPUT) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputInsTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputInsTag() : CBrowserOutputTagBase(CHtmlTagId::INS) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputKbdTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputKbdTag() : CBrowserOutputTagBase(CHtmlTagId::KBD) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputLabelTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputLabelTag() : CBrowserOutputTagBase(CHtmlTagId::LABEL) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputLegendTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputLegendTag() : CBrowserOutputTagBase(CHtmlTagId::LEGEND) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputLiTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputLiTag() : CBrowserOutputTagBase(CHtmlTagId::LI) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputLinkTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputLinkTag() : CBrowserOutputTagBase(CHtmlTagId::LINK) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

// CHtmlTagId::MAIN

class CBrowserOutputMapTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputMapTag() : CBrowserOutputTagBase(CHtmlTagId::MAP) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

// CHtmlTagId::MARK

class CBrowserOutputMenuTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputMenuTag() : CBrowserOutputTagBase(CHtmlTagId::MENU) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputMetaTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputMetaTag() : CBrowserOutputTagBase(CHtmlTagId::META) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

// CHtmlTagId::METER

class CBrowserOutputNavTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputNavTag() : CBrowserOutputTagBase(CHtmlTagId::NAV) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputNobrTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputNobrTag() : CBrowserOutputTagBase(CHtmlTagId::NOBR) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

// CHtmlTagId::NOFRAMES

class CBrowserOutputNoScriptTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputNoScriptTag() : CBrowserOutputTagBase(CHtmlTagId::NOSCRIPT) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

// CHtmlTagId::OBJECT

class CBrowserOutputOlTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputOlTag() : CBrowserOutputTagBase(CHtmlTagId::OL) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

// CHtmlTagId::OPTGROUP

class CBrowserOutputOptionTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputOptionTag() : CBrowserOutputTagBase(CHtmlTagId::OPTION) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputOutputTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputOutputTag() : CBrowserOutputTagBase(CHtmlTagId::OUTPUT) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputPTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputPTag() : CBrowserOutputTagBase(CHtmlTagId::P) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

// CHtmlTagId::PARAM

// CHtmlTagId::PICTURE

class CBrowserOutputPreTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputPreTag() : CBrowserOutputTagBase(CHtmlTagId::PRE) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

// CHtmlTagId::PROGRESS

// CHtmlTagId::Q

// CHtmlTagId::RP

// CHtmlTagId::RT

// CHtmlTagId::RUBY

// CHtmlTagId::S

class CBrowserOutputSampTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSampTag() : CBrowserOutputTagBase(CHtmlTagId::SAMP) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputScriptTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputScriptTag() : CBrowserOutputTagBase(CHtmlTagId::SCRIPT) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputSectionTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSectionTag() : CBrowserOutputTagBase(CHtmlTagId::SECTION) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputSelectTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSelectTag() : CBrowserOutputTagBase(CHtmlTagId::SELECT) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputSmallTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSmallTag() : CBrowserOutputTagBase(CHtmlTagId::SMALL) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputSpanTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSpanTag() : CBrowserOutputTagBase(CHtmlTagId::SPAN) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputStrikeTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputStrikeTag() : CBrowserOutputTagBase(CHtmlTagId::STRIKE) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputStrongTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputStrongTag() : CBrowserOutputTagBase(CHtmlTagId::STRONG) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputStyleTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputStyleTag() : CBrowserOutputTagBase(CHtmlTagId::STYLE) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputSubTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSubTag() : CBrowserOutputTagBase(CHtmlTagId::SUB) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputSummaryTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSummaryTag() : CBrowserOutputTagBase(CHtmlTagId::SUMMARY) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputSupTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSupTag() : CBrowserOutputTagBase(CHtmlTagId::SUP) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputSVGTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSVGTag() : CBrowserOutputTagBase(CHtmlTagId::SVG) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputTableTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTableTag() : CBrowserOutputTagBase(CHtmlTagId::TABLE) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputTBodyTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTBodyTag() : CBrowserOutputTagBase(CHtmlTagId::TBODY) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputTdTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTdTag() : CBrowserOutputTagBase(CHtmlTagId::TD) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputTextareaTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTextareaTag() : CBrowserOutputTagBase(CHtmlTagId::TEXTAREA) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputTFootTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTFootTag() : CBrowserOutputTagBase(CHtmlTagId::TFOOT) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputThTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputThTag() : CBrowserOutputTagBase(CHtmlTagId::TH) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputTHeadTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTHeadTag() : CBrowserOutputTagBase(CHtmlTagId::THEAD) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputTimeTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTimeTag() : CBrowserOutputTagBase(CHtmlTagId::TIME) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputTitleTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTitleTag() : CBrowserOutputTagBase(CHtmlTagId::TITLE) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputTrTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTrTag() : CBrowserOutputTagBase(CHtmlTagId::TR) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

// CHtmlTagId::TRACK

class CBrowserOutputTtTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTtTag() : CBrowserOutputTagBase(CHtmlTagId::TT) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputUTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputUTag() : CBrowserOutputTagBase(CHtmlTagId::U) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputUlTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputUlTag() : CBrowserOutputTagBase(CHtmlTagId::UL) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputVarTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputVarTag() : CBrowserOutputTagBase(CHtmlTagId::VAR) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputVideoTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputVideoTag() : CBrowserOutputTagBase(CHtmlTagId::VIDEO) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputWbrTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputWbrTag() : CBrowserOutputTagBase(CHtmlTagId::WBR) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

class CBrowserOutputXmpTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputXmpTag() : CBrowserOutputTagBase(CHtmlTagId::XMP) { }

  CBrowserObject *start(CBrowserWindowIFace *, CHtmlTag *) override;
};

//------

#define CBrowserOutputTagMgrInst CBrowserOutputTagMgr::instance()

class CBrowserOutputTagMgr {
 public:
  static CBrowserOutputTagMgr *instance();

  bool initialized() const;

  template<typename T>
  void addTag() {
    T *tag = new T();

    tagOutputData_[tag->id()] = tag;
  }

  CBrowserOutputTagBase *getTag(CHtmlTagId id) const;

  CBrowserOutputTagBase *getTag(CHtmlTag *tag);

 private:
  void init();

 private:
  using TagOutputData = std::map<CHtmlTagId, CBrowserOutputTagBase *>;

  TagOutputData tagOutputData_;
};

#endif
