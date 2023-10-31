#ifndef CBrowserForm_H
#define CBrowserForm_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <CQJFormIFace.h>
#include <CQJFormInputIFace.h>
#include <CImageLib.h>
#include <QWidget>

class CBrowserForm : public QObject, public CBrowserObject {
  Q_OBJECT

 public:
  using FormInputs = std::vector<CBrowserFormInput *>;

 private:
  class IFace : public CQJFormIFace {
   public:
    IFace(CBrowserForm *form) :
     form_(form) {
    }

    Inputs inputs() const override;

   private:
    CBrowserForm *form_ { nullptr };
  };

 public:
  explicit CBrowserForm(CBrowserWindowIFace *window);
 ~CBrowserForm();

  void init() override;

  CQJFormIFace *iface() { return &iface_; }

  CBrowserFormMethodType getMethod() const { return data_.method; }
  std::string            getAction() const { return data_.action; }

  std::string getTarget  () const { return target_  ; }
  std::string getEncoding() const { return encoding_; }

  const FormInputs &inputs() const { return inputs_; }
  void addInput(CBrowserFormInput *input) { inputs_.push_back(input); }
  int getNumInputs() const { return inputs_.size(); }
  CBrowserFormInput *getInput(int i) const { return inputs_[i]; }

  void setNameValue(const std::string &name, const std::string &value) override;

  CQJHtmlObj *createJObj(CJavaScript *js) override;

 public slots:
  void resetProc();
  void submitProc();

 private:
  CBrowserFormData data_;
  IFace            iface_;
  std::string      target_;
  std::string      encoding_;
  FormInputs       inputs_;
};

//------

class CBrowserFormOption : public CBrowserObject {
 public:
  CBrowserFormOption(CBrowserWindowIFace *window, const CBrowserFormOptionData &data);

  std::string getValue() const { return data_.value; }

  bool getSelected() const { return data_.selected; }
  void setSelected(bool b) { data_.selected = b; }

  std::string getText() const { return text_; }
  void setText(const std::string &text) { text_ = text; }

  void initProcess() override;
  void termProcess() override;

 private:
  CBrowserFormOptionData data_;
  std::string            text_;
};

//------

class CBrowserFormInput : public QObject, public CBrowserObject {
  Q_OBJECT

 private:
  class IFace : public CQJFormInputIFace {
   public:
    IFace(CBrowserFormInput *input) :
     input_(input) {
    }

    std::string getName() const override { return input_->getName(); }

    CJValueP value(CJavaScript *js) const override;

    CJValueP numberValue(CJavaScript *js) const override;

   private:
    CBrowserFormInput *input_ { nullptr };
  };

 public:
  CBrowserFormInput(CBrowserWindowIFace *window, CHtmlTagId id, CBrowserFormInputType type,
                    const CBrowserFormInputData &data=CBrowserFormInputData());

  virtual ~CBrowserFormInput();

  void init() override;

  CBrowserFormInputType getType() const { return type_; }

  IFace *iface() { return &iface_; }

  QWidget *getWindowWidget() const;

  CBrowserForm *getForm() const;

  void setNameValue(const std::string &name, const std::string &value) override;
  bool getNameValue(const std::string &name, std::string &value) const;

  std::string getValue() const { return data_.value; }
  void setValue(const std::string &value) { data_.value = value; }

  std::string getOnBlur  () const { return data_.onblur  ; }
  std::string getOnClick () const { return data_.onclick ; }
  std::string getOnChange() const { return data_.onchange; }
  std::string getOnFocus () const { return data_.onfocus ; }

  const QWidget *getWidget() const { return widget_; }

  int getWidth () const { return width_ ; }
  int getHeight() const { return height_; }

  virtual void createWidget() const = 0;

  virtual void drawWidget(CBrowserWindowIFace *window, const CTextBox &);

  virtual void reset() { };

  virtual void submit(std::string & /*url*/) { };

  virtual void getInlineWords(Words &words) const override;

  void initProcess() override;
  void termProcess() override;

  void show() override;
  void hide() override;

  void draw(const CTextBox &) override;

  CQJHtmlObj *createJObj(CJavaScript *js) override;

  void print(std::ostream &os) const override { os << "input"; }

 public slots:
  void onClickProc ();
  void onChangeProc();

 protected:
  CBrowserFormInputType type_ { CBrowserFormInputType::NONE };
  CBrowserFormInputData data_;
  IFace                 iface_;
  CBrowserForm*         form_ { nullptr };
  std::string           value_;
  mutable QWidget*      widget_ { nullptr };
  mutable int           width_ { 0 };
  mutable int           height_ { 0 };
};

//------

class CBrowserFormButton : public CBrowserFormInput {
  Q_OBJECT

 public:
  explicit CBrowserFormButton(CBrowserWindowIFace *window);

  CBrowserFormButton(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  void createWidget() const override;

  void setLabel(const std::string &text);

  CBrowserRegion calcRegion() const override;

 protected:
  int dummy { 0 };
};

//------

class CBrowserFormCheckBox : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormCheckBox(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

 protected:
  int checked_ { 0 };
};

//------

class CBrowserFormFileUpload : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormFileUpload(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  void createWidget() const override;

  void drawWidget(CBrowserWindowIFace *window, const CTextBox &region) override;

  CBrowserRegion calcRegion() const override;

 public slots:
  void buttonProc();

 protected:
  int size_ { 0 };
  int maxlength_ { 0 };
};

//------

class CBrowserFormHidden : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormHidden(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  void createWidget() const override;

  void drawWidget(CBrowserWindowIFace *window, const CTextBox &region) override;

  void submit(std::string &url) override;

 protected:
  int dummy_ { 0 };
};

//------

class CBrowserFormImage : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormImage(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

 ~CBrowserFormImage() { }

  void init() override;

  void createWidget() const override;

  void drawWidget(CBrowserWindowIFace *window, const CTextBox &region) override;

  CBrowserRegion calcRegion() const override;

 protected:
  std::string        src_;
  CImagePtr          image_;
  CBrowserImageAlign align_;
};

//------

class CBrowserFormTel : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormTel(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

  void reset() override;

  void submit(std::string &url) override;

 protected:
  int size_ { 0 };
  int maxlength_ { 0 };
};

//------

class CBrowserFormMonth : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormMonth(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

  void reset() override;

  void submit(std::string &url) override;

 protected:
  int size_ { 0 };
  int maxlength_ { 0 };
};

//------

class CBrowserFormDate : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormDate(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

  void reset() override;

  void submit(std::string &url) override;

 protected:
  int size_ { 0 };
  int maxlength_ { 0 };
};

//------

class CBrowserFormSearch : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormSearch(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

  void reset() override;

  void submit(std::string &url) override;

 protected:
  int size_ { 0 };
  int maxlength_ { 0 };
};

//------

class CBrowserFormNumber : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormNumber(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

  void reset() override;

  void submit(std::string &url) override;

 protected:
  int size_ { 0 };
  int maxlength_ { 0 };
};

//------

class CBrowserFormEmail : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormEmail(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  std::string text() const;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

  void reset() override;

  void submit(std::string &url) override;

 protected:
  int size_ { 0 };
  int maxlength_ { 0 };
};

//------

class CBrowserFormPassword : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormPassword(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

  void reset() override;

  void submit(std::string &url) override;

 protected:
  int size_ { 0 };
  int maxlength_ { 0 };
};

//------

class CBrowserFormRadio : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormRadio(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

 public slots:
  void buttonProc();

 protected:
  int checked_ { 0 };
};

//------

class CBrowserFormRange : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormRange(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  double value() const;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

 protected:
  double min_ { 0.0 };
  double max_ { 100.0 };
  double step_ { 1.0 };
  double value_ { 0.0 };
};

//------

class CBrowserFormReset : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormReset(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

 private slots:
  void resetProc();

 protected:
  int dummy_ { 0 };
};

//------

class CBrowserFormSelect : public CBrowserFormInput {
  Q_OBJECT

 public:
  explicit CBrowserFormSelect(CBrowserWindowIFace *window);

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  std::string value() const;

  void addOption(CBrowserFormOption *option);

  int getNumOptions() const { return options_.size(); }

  CBrowserFormOption *getOption(int i) const { return options_[i]; }

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

  void print(std::ostream &os) const override { os << "input/select"; }

 protected:
  using Options = std::vector<CBrowserFormOption *>;

  CBrowserFormSelectData data_;
  Options                options_;
};

//------

class CBrowserFormSubmit : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormSubmit(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

 protected:
  int dummy_ { 0 };
};

//------

class CBrowserFormText : public CBrowserFormInput {
  Q_OBJECT

 public:
  CBrowserFormText(CBrowserWindowIFace *window, const CBrowserFormInputData &data);

  void init() override;

  std::string text() const;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

  void reset() override;

  void submit(std::string &url) override;

  void print(std::ostream &os) const override { os << "input/text"; }

 protected:
  std::string classStr_;
  int         size_ { 0 };
  int         maxlength_ { 0 };
};

//------

class CBrowserFormTextarea : public CBrowserFormInput {
  Q_OBJECT

 public:
  explicit CBrowserFormTextarea(CBrowserWindowIFace *window);

  const std::string &text() const { return text_; }
  void setText(const std::string &v) { text_ = v; }

  void init() override;

  void initLayout() override;
  void termLayout() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  void createWidget() const override;

  CBrowserRegion calcRegion() const override;

  void reset() override;

  void submit(std::string &url) override;

 protected:
  CBrowserFormTextareaData     data_;
  CBrowserFormTextAreaWrapType wrap_ { CBrowserFormTextAreaWrapType::OFF };
  std::string                  text_;
};

#endif
