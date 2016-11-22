#ifndef CBrowserData_H
#define CBrowserData_H

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

#endif
