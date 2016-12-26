#ifndef CBrowserOutputTag_H
#define CBrowserOutputTag_H

#include <CBrowserSymbol.h>
#include <CHtmlLib.h>
#include <CRGBA.h>

class CBrowserOutputTagBase {
 public:
  CBrowserOutputTagBase(CHtmlTagId id) :
   id_(id) {
  }

  virtual ~CBrowserOutputTagBase() { }

  virtual bool isEmpty() const { return false; }

  const CHtmlTagId &id() const { return id_; }

  virtual CBrowserObject *start(CBrowserWindow *, CHtmlTag *) { return nullptr; }

  virtual void init(CBrowserWindow *, CBrowserObject *) { }

  virtual void term(CBrowserWindow *, CHtmlTag *) { }

  virtual void end(CBrowserWindow *, CBrowserObject *, CHtmlTag *) { }

 private:
  CHtmlTagId id_;
};

class CBrowserOutputATag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputATag() : CBrowserOutputTagBase(CHtmlTagId::A) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputAddressTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputAddressTag() : CBrowserOutputTagBase(CHtmlTagId::ADDRESS) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputBTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBTag() : CBrowserOutputTagBase(CHtmlTagId::B) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputBasefontTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBasefontTag() : CBrowserOutputTagBase(CHtmlTagId::BASEFONT) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputBigTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBigTag() : CBrowserOutputTagBase(CHtmlTagId::BIG) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputBlinkTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBlinkTag() : CBrowserOutputTagBase(CHtmlTagId::BLINK) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputBlockquoteTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBlockquoteTag() : CBrowserOutputTagBase(CHtmlTagId::BLOCKQUOTE) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputBodyTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBodyTag() : CBrowserOutputTagBase(CHtmlTagId::BODY) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputBrTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputBrTag() : CBrowserOutputTagBase(CHtmlTagId::BR) { }

  virtual bool isEmpty() const override { return true; }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputCanvasTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputCanvasTag() : CBrowserOutputTagBase(CHtmlTagId::CANVAS) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputCaptionTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputCaptionTag() : CBrowserOutputTagBase(CHtmlTagId::CAPTION) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputCenterTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputCenterTag() : CBrowserOutputTagBase(CHtmlTagId::CENTER) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputCiteTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputCiteTag() : CBrowserOutputTagBase(CHtmlTagId::CITE) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputCodeTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputCodeTag() : CBrowserOutputTagBase(CHtmlTagId::CODE) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputDdTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDdTag() : CBrowserOutputTagBase(CHtmlTagId::DD) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputDfnTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDfnTag() : CBrowserOutputTagBase(CHtmlTagId::DFN) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputDirTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDirTag() : CBrowserOutputTagBase(CHtmlTagId::DIR) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputDivTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDivTag() : CBrowserOutputTagBase(CHtmlTagId::DIV) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputDlTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDlTag() : CBrowserOutputTagBase(CHtmlTagId::DL) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;

  void term(CBrowserWindow *, CHtmlTag *);
};

class CBrowserOutputDtTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputDtTag() : CBrowserOutputTagBase(CHtmlTagId::DT) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputEmTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputEmTag() : CBrowserOutputTagBase(CHtmlTagId::EM) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputFontTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputFontTag() : CBrowserOutputTagBase(CHtmlTagId::FONT) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputFormTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputFormTag() : CBrowserOutputTagBase(CHtmlTagId::FORM) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputH1Tag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputH1Tag() : CBrowserOutputTagBase(CHtmlTagId::H1) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputH2Tag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputH2Tag() : CBrowserOutputTagBase(CHtmlTagId::H2) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputH3Tag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputH3Tag() : CBrowserOutputTagBase(CHtmlTagId::H3) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputH4Tag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputH4Tag() : CBrowserOutputTagBase(CHtmlTagId::H4) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputH5Tag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputH5Tag() : CBrowserOutputTagBase(CHtmlTagId::H5) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputH6Tag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputH6Tag() : CBrowserOutputTagBase(CHtmlTagId::H6) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputHeadTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputHeadTag() : CBrowserOutputTagBase(CHtmlTagId::HEAD) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputHrTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputHrTag() : CBrowserOutputTagBase(CHtmlTagId::HR) { }

  virtual bool isEmpty() const override { return true; }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputHtmlTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputHtmlTag() : CBrowserOutputTagBase(CHtmlTagId::HTML) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputITag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputITag() : CBrowserOutputTagBase(CHtmlTagId::I) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputImgTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputImgTag() : CBrowserOutputTagBase(CHtmlTagId::IMG) { }

  virtual bool isEmpty() const override { return true; }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputInputTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputInputTag() : CBrowserOutputTagBase(CHtmlTagId::INPUT) { }

  virtual bool isEmpty() const override { return true; }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputKbdTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputKbdTag() : CBrowserOutputTagBase(CHtmlTagId::KBD) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputLiTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputLiTag() : CBrowserOutputTagBase(CHtmlTagId::LI) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;

  void init(CBrowserWindow *, CBrowserObject *) override;
};

class CBrowserOutputMenuTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputMenuTag() : CBrowserOutputTagBase(CHtmlTagId::MENU) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputMetaTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputMetaTag() : CBrowserOutputTagBase(CHtmlTagId::META) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;

  virtual bool isEmpty() const override { return true; }
};

class CBrowserOutputNobrTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputNobrTag() : CBrowserOutputTagBase(CHtmlTagId::NOBR) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputOlTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputOlTag() : CBrowserOutputTagBase(CHtmlTagId::OL) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputOptionTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputOptionTag() : CBrowserOutputTagBase(CHtmlTagId::OPTION) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputPTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputPTag() : CBrowserOutputTagBase(CHtmlTagId::P) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputPreTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputPreTag() : CBrowserOutputTagBase(CHtmlTagId::PRE) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputSampTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSampTag() : CBrowserOutputTagBase(CHtmlTagId::SAMP) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputScriptTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputScriptTag() : CBrowserOutputTagBase(CHtmlTagId::SCRIPT) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputSelectTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSelectTag() : CBrowserOutputTagBase(CHtmlTagId::SELECT) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputSmallTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSmallTag() : CBrowserOutputTagBase(CHtmlTagId::SMALL) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputStrikeTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputStrikeTag() : CBrowserOutputTagBase(CHtmlTagId::STRIKE) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputStrongTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputStrongTag() : CBrowserOutputTagBase(CHtmlTagId::STRONG) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputSubTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSubTag() : CBrowserOutputTagBase(CHtmlTagId::SUB) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputSupTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputSupTag() : CBrowserOutputTagBase(CHtmlTagId::SUP) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputTableTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTableTag() : CBrowserOutputTagBase(CHtmlTagId::TABLE) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputTdTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTdTag() : CBrowserOutputTagBase(CHtmlTagId::TD) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputTextareaTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTextareaTag() : CBrowserOutputTagBase(CHtmlTagId::TEXTAREA) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputThTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputThTag() : CBrowserOutputTagBase(CHtmlTagId::TH) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputTitleTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTitleTag() : CBrowserOutputTagBase(CHtmlTagId::TITLE) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputTrTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTrTag() : CBrowserOutputTagBase(CHtmlTagId::TR) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputTtTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputTtTag() : CBrowserOutputTagBase(CHtmlTagId::TT) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputUTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputUTag() : CBrowserOutputTagBase(CHtmlTagId::U) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputUlTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputUlTag() : CBrowserOutputTagBase(CHtmlTagId::UL) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputVarTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputVarTag() : CBrowserOutputTagBase(CHtmlTagId::VAR) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputWbrTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputWbrTag() : CBrowserOutputTagBase(CHtmlTagId::WBR) { }

  bool isEmpty() const override { return true; }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

class CBrowserOutputXmpTag : public CBrowserOutputTagBase {
 public:
  CBrowserOutputXmpTag() : CBrowserOutputTagBase(CHtmlTagId::XMP) { }

  CBrowserObject *start(CBrowserWindow *, CHtmlTag *) override;
};

//------

#define CBrowserOutputTagMgrInst CBrowserOutputTagMgr::instance()

class CBrowserOutputTagMgr {
 public:
  static CBrowserOutputTagMgr *instance() {
    static CBrowserOutputTagMgr *inst;

    if (! inst)
      inst = new CBrowserOutputTagMgr;

    return inst;
  }

  bool initialized() const {
    return ! tagOutputData_.empty();
  }

  template<typename T>
  void addTag() {
    T *tag = new T();

    tagOutputData_[tag->id()] = tag;
  }

  CBrowserOutputTagBase *getTag(CHtmlTagId id) const {
    if (! initialized()) {
      CBrowserOutputTagMgr* th = const_cast<CBrowserOutputTagMgr *>(this);

      th->init();
    }

    //---

    auto p = tagOutputData_.find(id);

    if (p == tagOutputData_.end())
      return nullptr;

    return (*p).second;
  }

  CBrowserOutputTagBase *getTag(CHtmlTag *tag) {
    const CHtmlTagDef &data = tag->getTagDef();

    CHtmlTagId id = data.getId();

    return getTag(id);
  }

 private:
  void init();

 private:
  typedef std::map<CHtmlTagId,CBrowserOutputTagBase *> TagOutputData;

  TagOutputData tagOutputData_;
};

#endif