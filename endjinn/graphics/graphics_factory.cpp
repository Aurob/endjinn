#include "graphics_factory.h"

#ifdef __EMSCRIPTEN__
#include "graphics_es.h"
#else
#include "graphics_core.h"
#endif

std::unique_ptr<GraphicsAPI> GraphicsFactory::create() {
#ifdef __EMSCRIPTEN__
    return std::make_unique<GraphicsES>();
#else
    return std::make_unique<GraphicsCore>();
#endif
}

std::string GraphicsFactory::getRendererName() {
#ifdef __EMSCRIPTEN__
    return "OpenGL ES 2.0";
#else
    return "OpenGL 3.3 Core";
#endif
}