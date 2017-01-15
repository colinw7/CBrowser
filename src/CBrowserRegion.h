#ifndef CBrowserRegion_H
#define CBrowserRegion_H

class CBrowserRegion {
 public:
  CBrowserRegion() { }

  CBrowserRegion(int width, int ascent, int descent) :
   width_(width), ascent_(ascent), descent_(descent) {
  }

  void reset() { x_ = 0; y_ = 0; resetSize(); }

  void resetSize() { width_ = 0; ascent_ = 0; descent_ = 0; }

  int x() const { return x_; }
  void setX(int x) { x_ = x; }

  int y() const { return y_; }
  void setY(int y) { y_ = y; }

  int width() const { return width_; }
  void setWidth(int w) { width_ = w; }

  int ascent() const { return ascent_; }
  void setAscent(int i) { ascent_ = i; }

  int descent() const { return descent_; }
  void setDescent(int i) { descent_ = i; }

  int height() const { return ascent() + descent(); }
  void setHeight(int h) { ascent_ = h; descent_ = 0; }

  int x1() const { return x()           ; }
  int y1() const { return y()           ; }
  int x2() const { return x() + width (); }
  int y2() const { return y() + height(); }

 private:
  int x_       { 0 };
  int y_       { 0 };
  int width_   { 0 };
  int ascent_  { 0 };
  int descent_ { 0 };
};

#endif
