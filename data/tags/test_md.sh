#!/bin/csh -f

CHtmlToMarkdown $* >! temp.md

CQMarkdown temp.md

exit 0
