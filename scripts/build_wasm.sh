#!/bin/bash

# Configuration: The Path to your Portfolio Website
WEBSITE_PATH="/Users/miha/website"
WEBSITE_JS_DIR="$WEBSITE_PATH/static/js"

# Ensure emcc is in your path
if ! command -v emcc &> /dev/null
then
    echo "Error: emcc (Emscripten) could not be found."
    echo "Please install the Emscripten SDK (emsdk) first."
    exit
fi

# Ensure the website directory exists
if [ ! -d "$WEBSITE_PATH" ]; then
    echo "Error: Website directory not found at $WEBSITE_PATH"
    exit
fi

# Create the static/js directory if it doesn't exist
mkdir -p "$WEBSITE_JS_DIR"

echo "----------------------------------------------------"
echo "🚀 Building C++ Monte Carlo Engine to WebAssembly..."
echo "----------------------------------------------------"

# Compile with Emscripten
emcc -O3 -std=c++20 \
    src/wasm_api.cpp \
    src/PathGenerator.cpp \
    src/RandomGenerator.cpp \
    -Iinclude \
    -o "$WEBSITE_JS_DIR/mc_engine.js" \
    --bind \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s 'EXPORT_NAME="Module"' \
    -s MODULARIZE=1

if [ $? -eq 0 ]; then
    echo "----------------------------------------------------"
    echo "✅ Success! mc_engine.js and .wasm deployed to:"
    echo "   $WEBSITE_JS_DIR"
    echo "----------------------------------------------------"
else
    echo "❌ Build failed. Please check the errors above."
fi
