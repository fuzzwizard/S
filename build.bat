set CommonCompilerFlags=-Fo:/output/
set CommonCompilerFlags=-DPOM_SLOW=1 -DPOM_MSVC=1 -DPOM_WINDOWS=1 %CommonCompilerFlags%
set CommonLinkerFlags=

cl %CommonCompilerFlags% -D_CRT_SECURE_NO_WARNINGS ..\src\application.cpp /link %CommonLinkerFlags%
