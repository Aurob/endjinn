#pragma once

#include "graphics_api.h"
#include <memory>

class GraphicsFactory {
public:
    // Create the appropriate graphics API implementation for current build target
    static std::unique_ptr<GraphicsAPI> create();
    
    // Get renderer name without creating instance
    static std::string getRendererName();
};