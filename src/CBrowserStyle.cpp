#include <CBrowserStyle.h>
#include <CBrowserWindow.h>

CBrowserBStyle::
CBrowserBStyle(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::B), window_(window)
{
}

CBrowserBStyle::
~CBrowserBStyle()
{
}

void
CBrowserBStyle::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &)
{
}

//---

CBrowserBigStyle::
CBrowserBigStyle(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::BIG), window_(window)
{
}

CBrowserBigStyle::
~CBrowserBigStyle()
{
}

//---

CBrowserCiteStyle::
CBrowserCiteStyle(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::CITE), window_(window)
{
}

CBrowserCiteStyle::
~CBrowserCiteStyle()
{
}

//---

CBrowserBlinkStyle::
CBrowserBlinkStyle(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::BLINK), window_(window)
{
}

CBrowserBlinkStyle::
~CBrowserBlinkStyle()
{
}

//---

CBrowserEmStyle::
CBrowserEmStyle(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::EM), window_(window)
{
}

CBrowserEmStyle::
~CBrowserEmStyle()
{
}

//---

CBrowserIStyle::
CBrowserIStyle(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::I), window_(window)
{
}

CBrowserIStyle::
~CBrowserIStyle()
{
}

//---

CBrowserSmallStyle::
CBrowserSmallStyle(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::SMALL), window_(window)
{
}

CBrowserSmallStyle::
~CBrowserSmallStyle()
{
}

//---

CBrowserStrikeStyle::
CBrowserStrikeStyle(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::STRIKE), window_(window)
{
}

CBrowserStrikeStyle::
~CBrowserStrikeStyle()
{
}

//---

CBrowserStrongStyle::
CBrowserStrongStyle(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::STRONG), window_(window)
{
}

CBrowserStrongStyle::
~CBrowserStrongStyle()
{
}

//---

CBrowserSubStyle::
CBrowserSubStyle(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::SUB), window_(window)
{
}

CBrowserSubStyle::
~CBrowserSubStyle()
{
}

//---

CBrowserSupStyle::
CBrowserSupStyle(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::SUP), window_(window)
{
}

CBrowserSupStyle::
~CBrowserSupStyle()
{
}

//---

CBrowserUStyle::
CBrowserUStyle(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::U), window_(window)
{
}

CBrowserUStyle::
~CBrowserUStyle()
{
}

//---

CBrowserVarStyle::
CBrowserVarStyle(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::VAR), window_(window)
{
}

CBrowserVarStyle::
~CBrowserVarStyle()
{
}
