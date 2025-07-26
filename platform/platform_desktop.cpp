#include "platform_desktop.h"
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

DesktopPlatform::DesktopPlatform() : window(nullptr), glContext(nullptr), windowWidth(0), windowHeight(0) {
}

DesktopPlatform::~DesktopPlatform() {
    shutdown();
}

bool DesktopPlatform::initialize(int width, int height, const std::string& title) {
    windowWidth = width;
    windowHeight = height;
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Desktop Platform: SDL initialization failed: %s\n", SDL_GetError());
        return false;
    }
    
    // Set OpenGL 3.3 Core attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // Create window
    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        printf("Desktop Platform: Window creation failed: %s\n", SDL_GetError());
        return false;
    }
    
    // Create OpenGL context
    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        printf("Desktop Platform: OpenGL context creation failed: %s\n", SDL_GetError());
        return false;
    }
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        printf("Desktop Platform: GLEW initialization failed\n");
        return false;
    }
    
    setViewport(width, height);
    
    printf("Desktop Platform: Initialized successfully\n");
    return true;
}

void DesktopPlatform::shutdown() {
    if (glContext) {
        SDL_GL_DeleteContext(glContext);
        glContext = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void DesktopPlatform::swapBuffers() {
    if (window) {
        SDL_GL_SwapWindow(window);
    }
}

void DesktopPlatform::setViewport(int width, int height) {
    glViewport(0, 0, width, height);
}

void DesktopPlatform::pollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        handleEvent(event);
    }
}

bool DesktopPlatform::shouldQuit() const {
    return quitRequested;
}

void DesktopPlatform::setKeyHandler(std::function<void(int key)> handler) {
    keyHandler = handler;
}

void DesktopPlatform::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    if (window) {
        SDL_SetWindowSize(window, width, height);
        setViewport(width, height);
    }
}

SDL_Window* DesktopPlatform::getWindow() {
    return window;
}

std::string DesktopPlatform::getPlatformName() const {
    return "Desktop (OpenGL Core)";
}

bool DesktopPlatform::isWeb() const {
    return false;
}

void DesktopPlatform::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_QUIT:
            quitRequested = true;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                quitRequested = true;
            } else if (keyHandler) {
                keyHandler(event.key.keysym.sym);
            }
            break;
    }
}