#pragma once

#include <functional>
#include <string>

// Forward declarations
typedef union SDL_Event SDL_Event;
typedef struct SDL_Window SDL_Window;
typedef void* SDL_GLContext;

class Platform {
public:
    virtual ~Platform() = default;
    
    // Core platform operations
    virtual bool initialize(int width, int height, const std::string& title) = 0;
    virtual void shutdown() = 0;
    
    // Rendering
    virtual void swapBuffers() = 0;
    virtual void setViewport(int width, int height) = 0;
    
    // Events
    virtual void pollEvents() = 0;
    virtual bool shouldQuit() const = 0;
    
    // Input callbacks
    virtual void setKeyHandler(std::function<void(int key)> handler) = 0;
    
    // Window management
    virtual void setWindowSize(int width, int height) = 0;
    virtual SDL_Window* getWindow() = 0; // For compatibility during transition
    
    // Platform info
    virtual std::string getPlatformName() const = 0;
    virtual bool isWeb() const = 0;
    
protected:
    std::function<void(int key)> keyHandler;
    bool quitRequested = false;
};