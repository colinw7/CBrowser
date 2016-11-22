#ifndef CHtmlLayoutTypes_H
#define CHtmlLayoutTypes_H

enum class CHtmlLayoutClearType {
  NONE,
  LEFT,
  RIGHT,
  ALL
};

class CHtmlLayoutMgr;

struct CHtmlLayoutRegion {
  int x       { 0 };
  int y       { 0 };
  int width   { 0 };
  int ascent  { 0 };
  int descent { 0 };

  void reset() { x = 0; y = 0; resetSize(); }

  void resetSize() { width = 0; ascent = 0; descent = 0; }

  int getX() const { return x; }
  void setX(int x1) { x = x1; }

  int getY() const { return y; }
  void setY(int y1) { y = y1; }

  int getWidth() const { return width; }
  void setWidth(int w) { width = w; }

  int getHeight() const { return ascent + descent; }
  void setHeight(int h) { ascent = h; descent = 0; }
};

#endif
