#ifndef CBrowserLine_H
#define CBrowserLine_H

#include <CBrowserWord.h>
#include <CAlignType.h>

class CBrowserWindowIFace;

class CBrowserLine {
 public:
  struct PosWord {
    int          x = 0;
    int          y = 0;
    CBrowserWord word;

    PosWord(int x1, int y1, const CBrowserWord &word1) :
     x(x1), y(y1), word(word1) {
    }
  };

 public:
  CBrowserLine() { }

  void addWord(int x, int y, const CBrowserWord &word);

  void draw(CBrowserWindowIFace *window, int width, const CHAlignType &halign);

  bool isEmpty() const;

  void clear();

  int ascent() const { return ascent_; }
  int descent() const { return descent_; }

  int height() const { return ascent_ + descent_; }

 private:
  typedef std::vector<PosWord> Words;

  Words words_;
  bool  empty_ { true };
  int   width_  { 0 };
  int   ascent_ { 0 };
  int   descent_ { 0 };
};

#endif
