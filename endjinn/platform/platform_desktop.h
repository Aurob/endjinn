#pragma once

#include "platform.h"
#include <SDL2/SDL.h>

class DesktopPlatform : public Platform {
public:
    DesktopPlatform();
    ~DesktopPlatform() override;
    
    bool initialize(int width, int height, const std::string& title) override;
    void shutdown() override;
    
    void swapBuffers() override;
    void setViewport(int width, int height) override;
    
    void pollEvents() override;
    bool shouldQuit() const override;
    
    void setKeyHandler(std::function<void(int key)> handler) override;
    
    void setWindowSize(int width, int height) override;
    SDL_Window* getWindow() override;
    
    std::string getPlatformName() const override;
    bool isWeb() const override;
    
private:
    SDL_Window* window;
    SDL_GLContext glContext;
    int windowWidth, windowHeight;
    
    void handleEvent(const SDL_Event& event);
};