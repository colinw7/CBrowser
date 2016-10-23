#define HEADER_CELL 1
#define DATA_CELL   2

extern void  HtmlSetTableBorder
              (bool border);
extern void  HtmlSetTableSpace
              (int, int);
extern void  HtmlSetTableAlign
              (CHAlignType halign, CVAlignType valign);
extern void  HtmlSetTableWidth
              (int, int);
extern void  HtmlSetTableCellPadding
              (int);
extern void  HtmlSetTableCellSpacing
              (int);
extern void  HtmlSetTableBackgroundColor
              (CBrowserWindow *, const std::string &);
extern void  HtmlSetTableBorderColors
              (CBrowserWindow *, const std::string &, const std::string &, const std::string &);
extern void  HtmlStartTable
              (CBrowserWindow *);
extern void  HtmlEndTable
              (CBrowserWindow *);
extern void  HtmlSetTableRowAlign
              (CHAlignType halign, CVAlignType valign);
extern void  HtmlSetTableRowBackgroundColor
              (CBrowserWindow *, const std::string &);
extern void  HtmlSetTableRowBorderColors
              (CBrowserWindow *, const std::string &, const std::string &, const std::string &);
extern void  HtmlStartTableRow
              (CBrowserWindow *);
extern void  HtmlEndTableRow
              (CBrowserWindow *);
extern void  HtmlSetTableCellType
              (int);
extern void  HtmlSetTableCellAlign
              (CHAlignType halign, CVAlignType valign);
extern void  HtmlSetTableCellWidth
              (int, int);
extern void  HtmlSetTableCellSpan
              (int, int);
extern void  HtmlSetTableCellWrap
              (bool wrap);
extern void  HtmlSetTableCellBackgroundColor
              (CBrowserWindow *, const std::string &);
extern void  HtmlSetTableCellBorderColors
              (CBrowserWindow *, const std::string &, const std::string &, const std::string &);
extern void  HtmlStartTableCell
              (CBrowserWindow *);
extern void  HtmlEndTableCell
              (CBrowserWindow *);
extern void  HtmlSetTableCaptionAlign
              (CHAlignType halign, CVAlignType valign);
extern void  HtmlStartTableCaption
              (CBrowserWindow *);
extern void  HtmlEndTableCaption
              (CBrowserWindow *);
