#include "platform_web.h"
#include <emscripten.h>
#include <SDL_opengles2.h>
#include <iostream>

WebPlatform::WebPlatform() : window(nullptr), glContext(nullptr), windowWidth(0), windowHeight(0) {
}

WebPlatform::~WebPlatform() {
    shutdown();
}

bool WebPlatform::initialize(int width, int height, const std::string& title) {
    windowWidth = width;
    windowHeight = height;
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Web Platform: SDL initialization failed: %s\n", SDL_GetError());
        return false;
    }
    
    // Set OpenGL ES 2.0 attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // Create window
    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        printf("Web Platform: Window creation failed: %s\n", SDL_GetError());
        return false;
    }
    
    // Create OpenGL context
    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        printf("Web Platform: OpenGL context creation failed: %s\n", SDL_GetError());
        return false;
    }
    
    setViewport(width, height);
    
    printf("Web Platform: Initialized successfully\n");
    return true;
}

void WebPlatform::shutdown() {
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

void WebPlatform::swapBuffers() {
    if (window) {
        SDL_GL_SwapWindow(window);
    }
}

void WebPlatform::setViewport(int width, int height) {
    glViewport(0, 0, width, height);
}

void WebPlatform::pollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        handleEvent(event);
    }
}

bool WebPlatform::shouldQuit() const {
    return quitRequested;
}

void WebPlatform::setKeyHandler(std::function<void(int key)> handler) {
    keyHandler = handler;
}

void WebPlatform::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    if (window) {
        SDL_SetWindowSize(window, width, height);
        setViewport(width, height);
    }
}

SDL_Window* WebPlatform::getWindow() {
    return window;
}

std::string WebPlatform::getPlatformName() const {
    return "Web (Emscripten)";
}

bool WebPlatform::isWeb() const {
    return true;
}

void WebPlatform::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            if (keyHandler) {
                keyHandler(event.key.keysym.sym);
            }
            break;
        // Web typically doesn't get SDL_QUIT events, but handle anyway
        case SDL_QUIT:
            quitRequested = true;
            break;
    }
}