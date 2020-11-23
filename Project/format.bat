@echo off

astyle.exe --style=ansi -z1 -c --suffix=none --indent-col1-comments --indent-preproc-block --attach-closing-while --pad-oper --pad-paren-in --unpad-paren --align-pointer=name --align-reference=name --convert-tabs  --recursive --formatted *.c

astyle.exe --style=ansi -z1 -c --suffix=none --indent-col1-comments --indent-preproc-block --attach-closing-while --pad-oper --pad-paren-in --unpad-paren --align-pointer=name --align-reference=name --convert-tabs  --recursive --formatted *.h
