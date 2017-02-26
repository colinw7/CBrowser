#include <CBrowserOutputTag.h>

CBrowserOutputTagMgr *
CBrowserOutputTagMgr::
instance()
{
  static CBrowserOutputTagMgr *inst;

  if (! inst)
    inst = new CBrowserOutputTagMgr;

  return inst;
}

bool
CBrowserOutputTagMgr::
initialized() const
{
  return ! tagOutputData_.empty();
}

CBrowserOutputTagBase *
CBrowserOutputTagMgr::
getTag(CHtmlTagId id) const
{
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

CBrowserOutputTagBase *
CBrowserOutputTagMgr::
getTag(CHtmlTag *tag)
{
  const CHtmlTagDef &data = tag->getTagDef();

  CHtmlTagId id = data.getId();

  return getTag(id);
}

void
CBrowserOutputTagMgr::
init()
{
  addTag<CBrowserOutputATag         >();
  addTag<CBrowserOutputAbbrTag      >();
  addTag<CBrowserOutputAcronymTag   >();
  addTag<CBrowserOutputAddressTag   >();
  addTag<CBrowserOutputAppletTag    >();
  addTag<CBrowserOutputAreaTag      >();
  addTag<CBrowserOutputArticleTag   >();
  addTag<CBrowserOutputASideTag     >();
  addTag<CBrowserOutputAudioTag     >();
  addTag<CBrowserOutputBTag         >();
  addTag<CBrowserOutputBaseTag      >();
  addTag<CBrowserOutputBasefontTag  >();
  addTag<CBrowserOutputBdiTag       >();
  addTag<CBrowserOutputBdoTag       >();
  addTag<CBrowserOutputBigTag       >();
  addTag<CBrowserOutputBlinkTag     >();
  addTag<CBrowserOutputBlockquoteTag>();
  addTag<CBrowserOutputBodyTag      >();
  addTag<CBrowserOutputBrTag        >();
  addTag<CBrowserOutputButtonTag    >();
  addTag<CBrowserOutputCanvasTag    >();
  addTag<CBrowserOutputCaptionTag   >();
  addTag<CBrowserOutputCenterTag    >();
  addTag<CBrowserOutputCiteTag      >();
  addTag<CBrowserOutputCodeTag      >();
  addTag<CBrowserOutputDdTag        >();
  addTag<CBrowserOutputDelTag       >();
  addTag<CBrowserOutputDetailsTag   >();
  addTag<CBrowserOutputDfnTag       >();
  addTag<CBrowserOutputDirTag       >();
  addTag<CBrowserOutputDivTag       >();
  addTag<CBrowserOutputDlTag        >();
  addTag<CBrowserOutputDtTag        >();
  addTag<CBrowserOutputEmTag        >();
  addTag<CBrowserOutputFieldSetTag  >();
  addTag<CBrowserOutputFontTag      >();
  addTag<CBrowserOutputFooterTag    >();
  addTag<CBrowserOutputFormTag      >();
  addTag<CBrowserOutputH1Tag        >();
  addTag<CBrowserOutputH2Tag        >();
  addTag<CBrowserOutputH3Tag        >();
  addTag<CBrowserOutputH4Tag        >();
  addTag<CBrowserOutputH5Tag        >();
  addTag<CBrowserOutputH6Tag        >();
  addTag<CBrowserOutputHeadTag      >();
  addTag<CBrowserOutputHeaderTag    >();
  addTag<CBrowserOutputHGroupTag    >();
  addTag<CBrowserOutputHrTag        >();
  addTag<CBrowserOutputHtmlTag      >();
  addTag<CBrowserOutputITag         >();
  addTag<CBrowserOutputIFrameTag    >();
  addTag<CBrowserOutputImgTag       >();
  addTag<CBrowserOutputInputTag     >();
  addTag<CBrowserOutputInsTag       >();
  addTag<CBrowserOutputKbdTag       >();
  addTag<CBrowserOutputLabelTag     >();
  addTag<CBrowserOutputLegendTag    >();
  addTag<CBrowserOutputLiTag        >();
  addTag<CBrowserOutputLinkTag      >();
  addTag<CBrowserOutputNavTag       >();
  addTag<CBrowserOutputMapTag       >();
  addTag<CBrowserOutputMenuTag      >();
  addTag<CBrowserOutputMetaTag      >();
  addTag<CBrowserOutputNobrTag      >();
  addTag<CBrowserOutputNoScriptTag  >();
  addTag<CBrowserOutputOlTag        >();
  addTag<CBrowserOutputOptionTag    >();
  addTag<CBrowserOutputOutputTag    >();
  addTag<CBrowserOutputPTag         >();
  addTag<CBrowserOutputPreTag       >();
  addTag<CBrowserOutputSampTag      >();
  addTag<CBrowserOutputScriptTag    >();
  addTag<CBrowserOutputSectionTag   >();
  addTag<CBrowserOutputSelectTag    >();
  addTag<CBrowserOutputSmallTag     >();
  addTag<CBrowserOutputSpanTag      >();
  addTag<CBrowserOutputStrikeTag    >();
  addTag<CBrowserOutputStrongTag    >();
  addTag<CBrowserOutputStyleTag     >();
  addTag<CBrowserOutputSubTag       >();
  addTag<CBrowserOutputSummaryTag   >();
  addTag<CBrowserOutputSupTag       >();
  addTag<CBrowserOutputSVGTag       >();
  addTag<CBrowserOutputTableTag     >();
  addTag<CBrowserOutputTBodyTag     >();
  addTag<CBrowserOutputTdTag        >();
  addTag<CBrowserOutputTextareaTag  >();
  addTag<CBrowserOutputTFootTag     >();
  addTag<CBrowserOutputTHeadTag     >();
  addTag<CBrowserOutputThTag        >();
  addTag<CBrowserOutputTimeTag      >();
  addTag<CBrowserOutputTitleTag     >();
  addTag<CBrowserOutputTrTag        >();
  addTag<CBrowserOutputTtTag        >();
  addTag<CBrowserOutputUTag         >();
  addTag<CBrowserOutputUlTag        >();
  addTag<CBrowserOutputVarTag       >();
  addTag<CBrowserOutputVideoTag     >();
  addTag<CBrowserOutputWbrTag       >();
  addTag<CBrowserOutputXmpTag       >();
}
