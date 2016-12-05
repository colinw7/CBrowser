#ifndef CBrowserData_H
#define CBrowserData_H

#include <CRGBA.h>

struct CBrowserCanvasData {
  std::string id;
  int         width  = -1;
  int         height = -1;
};

//---

struct CBrowserImageData {
  std::string        src;
  CBrowserImageAlign align  = CBrowserImageAlign::BOTTOM;
  int                border = 2;
  int                width  = -1;
  int                height = -1;
  std::string        usemap;
  int                hspace = 2;
  int                vspace = 2;
  std::string        alt    = "";
};

//---

struct CBrowserLinkData {
  std::string href;
  std::string id;
  std::string title;
};

//---

struct CBrowserTextData {
  CRGBA                 color     { 0, 0, 0, 0 };
  bool                  underline { false };
  bool                  strike    { false };
  CBrowserTextPlaceType place     { CBrowserTextPlaceType::NORMAL };
  bool                  breakup   { true };
  bool                  format    { true };
};

#endif
