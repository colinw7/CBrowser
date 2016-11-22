#ifndef CBrowserForm_H
#define CBrowserForm_H

#include <CBrowserObject.h>
#include <CImageLib.h>
#include <QWidget>

#define FORM_METHOD_GET  1
#define FORM_METHOD_POST 2

enum class CBrowserFormInputType {
  NONE,
  BUTTON,
  CHECKBOX,
  FILE,
  HIDDEN,
  IMAGE,
  PASSWORD_TEXT,
  RADIO_BUTTON,
  RANGE,
  RESET_BUTTON,
  SELECT,
  SUBMIT_BUTTON,
  TEXT,
  TEXTAREA
};

enum class CBrowserFormTextAreaWrapType {
  OFF,
  VIRTUAL,
  PHYSICAL
};

//------

class CBrowserFormMgr {
 public:
  CBrowserFormMgr(CBrowserDocument *document);

  CBrowserForm *currentForm() const { return current_form_; }
  void setCurrentForm(CBrowserForm *form) { current_form_ = form; }

  CBrowserFormSelect *currentSelect() const { return current_select_; }
  void setCurrentSelect(CBrowserFormSelect *select) { current_select_ = select; }

  CBrowserFormOption *currentOption() const { return current_option_; }
  void setCurrentOption(CBrowserFormOption *option) { current_option_ = option; }

  CBrowserFormTextarea *currentTextarea() const { return current_textarea_; }
  void setCurrentTextarea(CBrowserFormTextarea *textarea) { current_textarea_ = textarea; }

  const std::string &inputOnBlur() const { return input_onblur_; }
  void setInputOnBlur(const std::string &onblur) { input_onblur_ = onblur; }

  const std::string &inputOnChange() const { return input_onchange_; }
  void setInputOnChange(const std::string &onchange) { input_onchange_ = onchange; }

  const std::string &inputOnClick() const { return input_onclick_; }
  void setInputOnClick(const std::string &onclick) { input_onclick_ = onclick; }

  const std::string &inputOnFocus() const { return input_onfocus_; }
  void setInputOnFocus(const std::string &onfocus) { input_onfocus_ = onfocus; }

  void startForm(const std::string &name, int method, const std::string &action);
  void endForm();

  CBrowserFormSelect *startSelect(const std::string &, int, int);
  void endSelect();

  void startOption(const std::string &, int);
  void endOption(const std::string &);

  CBrowserFormTextarea *startTextarea(const std::string &, int, int, const std::string &);
  void endTextarea(const std::string &);

  CBrowserFormButton*     addButtonInput(const std::string &, const std::string &);
  CBrowserFormCheckBox*   addCheckboxInput(const std::string &, const std::string &, int);
  CBrowserFormFileUpload* addFileInput(const std::string &, const std::string &, int, int);
  CBrowserFormHidden*     addHiddenInput(const std::string &, const std::string &);
  CBrowserFormImage*      addImageInput(const std::string &, const std::string &,
                                        const std::string &);
  CBrowserFormPassword*   addPasswordInput(const std::string &, int, int);
  CBrowserFormRadio*      addRadioInput(const std::string &, const std::string &, int);
  CBrowserFormRange*      addRangeInput(const std::string &, const std::string &,
                                        const std::string &, const std::string &,
                                        const std::string &);
  CBrowserFormReset*      addResetInput(const std::string &, const std::string &);
  CBrowserFormSubmit*     addSubmitInput(const std::string &, const std::string &);
  CBrowserFormText*       addTextInput(const std::string &, const std::string &,
                                       const std::string &, int, int, const std::string &);

 private:
  CBrowserDocument*     document_         { nullptr };
  CBrowserForm*         current_form_     { nullptr };
  CBrowserFormSelect*   current_select_   { nullptr };
  CBrowserFormOption*   current_option_   { nullptr };
  CBrowserFormTextarea* current_textarea_ { nullptr };
  std::string           input_onblur_     { "" };
  std::string           input_onclick_    { "" };
  std::string           input_onchange_   { "" };
  std::string           input_onfocus_    { "" };
};

//------

class CBrowserForm : public QObject {
 public:
  CBrowserForm(const std::string &name, CBrowserDocument *document,
               int method, const std::string &action);

  std::string getName() const { return name_; }

  CBrowserDocument *getDocument() const { return document_; }

  int         getMethod() const { return method_; }
  std::string getAction() const { return action_; }
  std::string getTarget() const { return target_; }

  std::string getEncoding() const { return encoding_; }

  void addInput(CBrowserFormInput *input) { input_list_.push_back(input); }

  int getNumInputs() const { return input_list_.size(); }

  CBrowserFormInput *getInput(int i) const { return input_list_[i]; }

 public slots:
  void resetProc();
  void submitProc();

 private:
  typedef std::vector<CBrowserFormInput *> FormInputs;

  std::string       name_;
  CBrowserDocument *document_ { nullptr };
  int               method_ { 0 };
  std::string       action_;
  std::string       target_;
  std::string       encoding_;
  FormInputs        input_list_;
};

//------

class CBrowserFormOption {
 public:
  CBrowserFormOption(const std::string &value, int selected);

  std::string getValue() const { return value_; }

  int getSelected() const { return selected_; }

  std::string getText() const { return text_; }

  void setText(const std::string &text) { text_ = text; }

 private:
  std::string value_;
  int         selected_ { 0 };
  std::string text_;
};

//------

class CBrowserFormInput : public CBrowserObject {
 public:
  CBrowserFormInput(CBrowserDocument *document, CBrowserFormInputType type,
                    const std::string &name, const std::string &value);

  virtual ~CBrowserFormInput();

  CBrowserFormInputType getType() const { return type_; }

  CBrowserForm *getForm() const { return form_; }

  std::string getName() const { return name_; }

  std::string getValue() const { return value_; }

  void setValue(const std::string &value) { value_ = value; }

  std::string getOnChange() const { return onchange_; }
  std::string getOnClick () const { return onclick_; }
  std::string getOnFocus () const { return onfocus_; }

  QWidget *getWidget() const { return widget_; }

  int getWidth () const { return width_ ; }
  int getHeight() const { return height_; }

  virtual void createWidget(CBrowserWindow *window) = 0;

  virtual void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &) = 0;

  virtual void reset() { };

  virtual void submit(std::string & /*url*/) { };

  void format(CHtmlLayoutMgr *) override;
  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

  void onClickProc ();
  void onChangeProc();

  void print(std::ostream &os) const override { os << "input"; }

 protected:
  CBrowserDocument*     document_ { nullptr };
  CBrowserFormInputType type_ { CBrowserFormInputType::NONE };
  CBrowserForm*         form_ { nullptr };
  std::string           name_;
  std::string           value_;
  std::string           onblur_;
  std::string           onchange_;
  std::string           onclick_;
  std::string           onfocus_;
  QWidget*              widget_ { nullptr };
  int                   width_ { 0 };
  int                   height_ { 0 };
};

//------

class CBrowserFormButton : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormButton(CBrowserDocument *document, const std::string &name,
                     const std::string &value);

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region) override;

 protected:
  int dummy { 0 };
};

//------

class CBrowserFormCheckBox : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormCheckBox(CBrowserDocument *document, const std::string &name,
                       const std::string &value, int checked);

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region) override;

 protected:
  int checked_ { 0 };
};

//------

class CBrowserFormFileUpload : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormFileUpload(CBrowserDocument *document, const std::string &name,
                         const std::string &value, int size, int maxlength);

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region) override;

 public slots:
  void buttonProc();

 protected:
  int size_ { 0 };
  int maxlength_ { 0 };
};

//------

class CBrowserFormHidden : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormHidden(CBrowserDocument *document, const std::string &name,
                     const std::string &value);

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region) override;

  void submit(std::string &url);

 protected:
  int dummy_ { 0 };
};

//------

class CBrowserFormImage : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormImage(CBrowserDocument *document, const std::string &name,
                    const std::string &value, const std::string &src,
                    const CImagePtr &image, CBrowserImageAlign align);

 ~CBrowserFormImage() { }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region) override;

 protected:
  std::string        src_;
  CImagePtr          image_;
  CBrowserImageAlign align_;
};

//------

class CBrowserFormPassword : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormPassword(CBrowserDocument *document, const std::string &name,
                       const std::string &value, int size, int maxlength);

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region) override;

  void reset();

  void submit(std::string &url);

 protected:
  int size_ { 0 };
  int maxlength_ { 0 };
};

//------

class CBrowserFormRadio : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormRadio(CBrowserDocument *document, const std::string &name,
                    const std::string &value, int checked);

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region) override;

 public slots:
  void buttonProc();

 protected:
  int checked_ { 0 };
};

//------

class CBrowserFormRange : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormRange(CBrowserDocument *document, const std::string &name,
                    const std::string &value, const std::string &min,
                    const std::string &max, const std::string &step);

  double value() const;

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region) override;

 public slots:
  void buttonProc();

 protected:
  double min_ { 0.0 };
  double max_ { 100.0 };
  double step_ { 1.0 };
  double value_ { 0.0 };
};

//------

class CBrowserFormReset : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormReset(CBrowserDocument *document, const std::string &name,
                    const std::string &value);

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region) override;

 protected:
  int dummy_ { 0 };
};

//------

class CBrowserFormSelect : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormSelect(CBrowserDocument *document, const std::string &name,
                     const std::string &value, int size, int multiple);

  std::string value() const;

  void addOption(CBrowserFormOption *option) {
    options_.push_back(option);
  }

  int getNumOptions() const { return options_.size(); }

  CBrowserFormOption *getOption(int i) const { return options_[i]; }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region) override;

  void print(std::ostream &os) const override { os << "input/select"; }

 protected:
  typedef std::vector<CBrowserFormOption *> Options;

  int     size_ { 0 };
  int     multiple_ { 0 };
  Options options_;
};

//------

class CBrowserFormSubmit : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormSubmit(CBrowserDocument *document, const std::string &name,
                     const std::string &value);

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region) override;

 protected:
  int dummy_ { 0 };
};

//------

class CBrowserFormText : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormText(CBrowserDocument *document, const std::string &name, const std::string &value,
                   const std::string &classStr, int size, int maxlength,
                   const std::string &placeholder);

  std::string text() const;

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region) override;

  void reset();

  void submit(std::string &url);

  void print(std::ostream &os) const override { os << "input/text"; }

 protected:
  std::string classStr_;
  int         size_ { 0 };
  int         maxlength_ { 0 };
  std::string placeholder_;
};

//------

class CBrowserFormTextarea : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormTextarea(CBrowserDocument *document, const std::string &name,
                       const std::string &value, int rows, int cols,
                       CBrowserFormTextAreaWrapType wrap);

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region) override;

  void reset();

  void submit(std::string &url);

 protected:
  int                          rows_ { 0 };
  int                          cols_ { 0 };
  CBrowserFormTextAreaWrapType wrap_ { CBrowserFormTextAreaWrapType::OFF };
};

#endif
