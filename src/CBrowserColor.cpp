#include <CBrowserColor.h>
#include <CStrParse.h>
#include <CStrUtil.h>
#include <vector>

void
CBrowserColor::
init()
{
  type_ = Type::NONE;

  //---

  auto p = str_.find("linear-gradient(");

  if (p != std::string::npos) {
    gradient_.reset();

    CStrParse parse(str_);

    parse.setPos(p + 16);

    std::vector<std::string> words;

    std::string word;

    int brackets = 1;

    while (! parse.eof()) {
      if      (parse.isChar('(')) {
        ++brackets;

        word += parse.readChar();
      }
      else if (parse.isChar(')')) {
        --brackets;

        if (brackets == 0)
          break;

        word += parse.readChar();
      }
      else if (parse.isChar(',')) {
        if (brackets <= 1) {
          addGradientWord(word);

          word = "";

          parse.skipChar();
        }
        else
          word += parse.readChar();
      }
      else {
        word += parse.readChar();
      }
    }

    addGradientWord(word);

    type_ = Type::GRADIENT;
  }
  else {
    CRGBA c;

    if (! CRGBName::toHtmlRGBA(str_, c))
      return;

    color_ = c;
    type_  = Type::COLOR;
  }
}

void
CBrowserColor::
addGradientWord(const std::string &str)
{
  std::string sstr = CStrUtil::stripSpaces(str);

  if (sstr.substr(0, 3) == "to ") {
    uint direction = 0;

    std::vector<std::string> words;

    CStrUtil::toWords(sstr, words);

    for (std::size_t i = 1; i < words.size(); ++i) {
      std::string word = words[i];

      if      (word == "left")
        direction |= uint(CBrowserColorGradient::Direction::LEFT);
      else if (word == "right")
        direction |= uint(CBrowserColorGradient::Direction::RIGHT);
      else if (word == "top")
        direction |= uint(CBrowserColorGradient::Direction::TOP);
      else if (word == "bottom")
        direction |= uint(CBrowserColorGradient::Direction::BOTTOM);
      else
        std::cerr << "Invalid gradient direction '" << word << "'\n";
    }

    gradient_.setDirection(direction);
  }
  else {
    CRGBA c;

    if (! CRGBName::toHtmlRGBA(sstr, c)) {
      std::cerr << "Invalid gradient color '" << sstr << "'\n";
      return;
    }

    gradient_.addColor(c);
  }
}
