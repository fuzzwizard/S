COMPILER_FLAGS="-DCS_SLOW -DCS_LLVM -DCS_OSX"

clang++ $COMPILER_FLAGS -std=c++14 src/application.cpp -o output/cscheme
