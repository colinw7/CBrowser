#define FORM_METHOD_GET  1
#define FORM_METHOD_POST 2

extern void  HtmlStartForm
              (CBrowserDocument *, const std::string &, int, const std::string &);
extern void  HtmlEndForm
              (CBrowserDocument *);
extern void  CBrowserFormSetInputOnBlur
              (const std::string &);
extern void  CBrowserFormSetInputOnClick
              (const std::string &);
extern void  CBrowserFormSetInputOnChange
              (const std::string &);
extern void  CBrowserFormSetInputOnFocus
              (const std::string &);
extern void  CBrowserFormStartSelect
              (CBrowserDocument *, const std::string &, int, int);
extern void  CBrowserFormEndSelect
              (CBrowserDocument *);
extern void  CBrowserFormStartOption
              (const std::string &, int);
extern void  CBrowserFormEndOption
              (const std::string &);
extern void  CBrowserFormStartTextarea
              (CBrowserDocument *, const std::string &, int, int, const std::string &);
extern void  CBrowserFormEndTextarea
              (CBrowserDocument *, const std::string &);
extern void  CBrowserFormAddButtonInput
              (CBrowserDocument *, const std::string &, const std::string &);
extern void  CBrowserFormAddCheckboxInput
              (CBrowserDocument *, const std::string &, const std::string &, int);
extern void  CBrowserFormAddFileInput
              (CBrowserDocument *, const std::string &, const std::string &, int, int);
extern void  CBrowserFormAddHiddenInput
              (CBrowserDocument *, const std::string &, const std::string &);
extern void  CBrowserFormAddImageInput
              (CBrowserDocument *, const std::string &, const std::string &, const std::string &);
extern void  CBrowserFormAddPasswordInput
              (CBrowserDocument *, const std::string &, int, int);
extern void  CBrowserFormAddRadioInput
              (CBrowserDocument *, const std::string &, const std::string &, int);
extern void  CBrowserFormAddResetInput
              (CBrowserDocument *, const std::string &, const std::string &);
extern void  CBrowserFormAddSubmitInput
              (CBrowserDocument *, const std::string &, const std::string &);
extern void  CBrowserFormAddTextInput
              (CBrowserDocument *, const std::string &, const std::string &, int, int);
