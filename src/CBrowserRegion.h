#ifndef CBrowserRegion_H
#define CBrowserRegion_H

class CBrowserRegion {
 public:
  CBrowserRegion() { }

  CBrowserRegion(int width, int ascent, int descent) :
   set_(true), width_(width), ascent_(ascent), descent_(descent) {
  }

  bool isSet() const { return set_; }

  void reset() { set_ = false; x_ = 0; y_ = 0; resetSize(); }

  void resetSize() { width_ = 0; ascent_ = 0; descent_ = 0; }

  int x() const { return x_; }
  void setX(int x) { x_ = x; set_ = true; }

  int y() const { return y_; }
  void setY(int y) { y_ = y; set_ = true; }

  int width() const { return width_; }
  void setWidth(int w) { width_ = w; set_ = true; }

  int ascent() const { return ascent_; }
  void setAscent(int i) { ascent_ = i; set_ = true; }

  int descent() const { return descent_; }
  void setDescent(int i) { descent_ = i; set_ = true; }

  int height() const { return ascent() + descent(); }
  void setHeight(int h) { ascent_ = h; descent_ = 0; set_ = true; }

  int x1() const { return x()           ; }
  int y1() const { return y()           ; }
  int x2() const { return x() + width (); }
  int y2() const { return y() + height(); }

  CBrowserRegion &operator+=(const CBrowserRegion &r) {
    if (! r.set_) return *this;

    if (! set_) {
      set_ = true;

      x_       = r.x_;
      y_       = r.y_;
      width_   = r.width_;
      ascent_  = r.ascent_;
      descent_ = r.descent_;
    }
    else {
      auto xmin1 =   x_; auto xmax1 =   x_ +   width_;
      auto xmin2 = r.x_; auto xmax2 = r.x_ + r.width_;

      auto ymin1 =   y_ -   descent_; auto ymax1 =   y_ +   ascent_;
      auto ymin2 = r.y_ - r.descent_; auto ymax2 = r.y_ + r.ascent_;

      x_ = std::min(xmin1, xmin2);
      y_ = std::min(ymin1, ymin2);

      width_   = std::max(xmax1, xmax2) - x_;
      ascent_  = std::max(ymax1, ymax2) - y_;
      descent_ = 0;
    }

    return *this;
  }

 private:
  bool set_     { false };
  int  x_       { 0 };
  int  y_       { 0 };
  int  width_   { 0 };
  int  ascent_  { 0 };
  int  descent_ { 0 };
};

#endif
