# Endjinn Project Context

## Project Overview
Cross-platform C++ application using SDL2, OpenGL, and SDL_ttf for graphics and text rendering. Supports both web (via Emscripten) and desktop builds.

## Build System
- **Build script**: `build.sh`
- **Usage**:
  - `./build.sh` - builds both web and desktop versions
  - `./build.sh --web-only` - builds web version only (Emscripten)
  - `./build.sh --desktop-only` - builds desktop version only (g++)

## Code Structure
- **Main file**: `main.cpp`
- **Global context struct**: Contains SDL window, OpenGL context, font, and runtime state
- **Cross-platform**: Uses `#ifdef __EMSCRIPTEN__` for platform-specific code

## Font Loading
- Local font: `DejaVuSansMono-Bold.ttf` (24pt size)

## Window Properties
- Size: 1000x1000 pixels
- OpenGL context: Core profile 3.3
- Features: Resizable, double-buffered

## Build Configuration
- C++17 standard
- Web: Emscripten with WASM, WebGL2, SDL2
- Desktop: g++ with Homebrew SDL2 libraries

# There should always be parity between the web and desktop builds.
