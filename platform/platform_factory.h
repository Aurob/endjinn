#pragma once

#include "platform.h"
#include <memory>

class PlatformFactory {
public:
    // Create the appropriate platform implementation for current build target
    static std::unique_ptr<Platform> create();
    
    // Get platform name without creating instance
    static std::string getPlatformName();
    
    // Check if current build target is web
    static bool isWebBuild();
};