@echo off

set CommonCompilerFlags=-Fe:output/
set CommonCompilerFlags=-DCS_SLOW=1 -DCS_MSVC=1 -DCS_WINDOWS=1 %CommonCompilerFlags%
set CommonLinkerFlags=

cl %CommonCompilerFlags% -D_CRT_SECURE_NO_WARNINGS src\application.cpp /link %CommonLinkerFlags%
