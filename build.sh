emcc calculator.c tHTML_Tools.c -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' --preload-file assets -o calculator.js
