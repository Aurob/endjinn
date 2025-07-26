#include "platform_factory.h"

#ifdef __EMSCRIPTEN__
#include "platform_web.h"
#else
#include "platform_desktop.h"
#endif

std::unique_ptr<Platform> PlatformFactory::create() {
#ifdef __EMSCRIPTEN__
    return std::make_unique<WebPlatform>();
#else
    return std::make_unique<DesktopPlatform>();
#endif
}

std::string PlatformFactory::getPlatformName() {
#ifdef __EMSCRIPTEN__
    return "Web";
#else
    return "Desktop";
#endif
}

bool PlatformFactory::isWebBuild() {
#ifdef __EMSCRIPTEN__
    return true;
#else
    return false;
#endif
}