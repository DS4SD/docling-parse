#!/bin/sh
 
clear
ls -al

find . -iname \*.c -or -iname \*.h -exec emacs -nw -q {} --eval "(progn (mark-whole-buffer) (indent-region (point-min) (point-max) nil) (save-buffer))" --kill \;

find . -name '*~' -delete