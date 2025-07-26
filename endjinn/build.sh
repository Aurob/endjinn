#!/bin/bash

# Parse command line arguments
BUILD_WEB=true
BUILD_DESKTOP=true

while [[ $# -gt 0 ]]; do
    case $1 in
        --web-only)
            BUILD_WEB=true
            BUILD_DESKTOP=false
            shift
            ;;
        --desktop-only)
            BUILD_WEB=false
            BUILD_DESKTOP=true
            shift
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 [--web-only|--desktop-only]"
            echo "  --web-only      Build only web version"
            echo "  --desktop-only  Build only desktop version"
            echo "  (no flags)      Build both versions"
            exit 1
            ;;
    esac
done

echo "Build commenced at:" $(date)
start_time=$(date +%s)

# Web build
if [ "$BUILD_WEB" = true ]; then
    echo "Building web version..."
    em++ -std=c++17 main.cpp shader.cpp text_renderer.cpp \
      platform/platform_web.cpp platform/platform_factory.cpp \
      graphics/graphics_es.cpp graphics/graphics_factory.cpp \
      -s WASM=1 -s USE_SDL=2 -s USE_WEBGL2=1\
      -s USE_SDL_TTF=2\
      -lSDL\
      -s EXPORTED_FUNCTIONS="[_main, _malloc, _free]"\
      --preload-file DejaVuSansMono-Bold.ttf\
      --preload-file shaders\
      -o index.js\
      -O0
    
    if [ $? -eq 0 ]; then
        echo "Web build completed successfully"
    else
        echo "Web build failed"
        exit 1
    fi
fi

# Desktop build
if [ "$BUILD_DESKTOP" = true ]; then
    echo "Building desktop version..."
    
    # Output binary name
    OUT="app"
    
    # Compilation flags
    CXX="g++"
    CXXFLAGS="-std=c++17 -O2"
    INCLUDES="-I/opt/homebrew/include"
    LIBS="-L/opt/homebrew/lib -lSDL2 -lSDL2_ttf -lGLEW -framework OpenGL"
    
    # Source files
    SRC="main.cpp shader.cpp text_renderer.cpp platform/platform_desktop.cpp platform/platform_factory.cpp graphics/graphics_core.cpp graphics/graphics_factory.cpp"
    
    $CXX $CXXFLAGS $SRC $INCLUDES $LIBS -o $OUT
    
    if [ $? -eq 0 ]; then
        echo "Desktop build completed successfully"
    else
        echo "Desktop build failed"
        exit 1
    fi
fi

end_time=$(date +%s)
echo "Build finished at:" $(date)
echo "Total build time:" $(($end_time - $start_time)) "seconds"