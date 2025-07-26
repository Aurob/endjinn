#include <iostream>
#include <memory>
#include <SDL2/SDL_ttf.h>
#include "platform/platform_factory.h"
#include "graphics/graphics_factory.h"
#include "text_renderer.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;

// Window dimensions
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;

// Application state
struct AppState {
    std::unique_ptr<Platform> platform;
    std::unique_ptr<GraphicsAPI> graphics;
    std::unique_ptr<TextRenderer> textRenderer;
};

AppState app;

// Function declarations
void mainLoop();
void handleKeyPress(int key);
bool initialize();
void shutdown();

// Platform-agnostic main entry point
int main(int argc, char* argv[]) {
    printf("Starting Endjinn on %s platform\n", PlatformFactory::getPlatformName().c_str());
    
    if (!initialize()) {
        printf("Failed to initialize application\n");
        return -1;
    }
    
    // Main loop - completely platform agnostic
    if (PlatformFactory::isWebBuild()) {
        // Web build uses Emscripten's main loop
#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(mainLoop, 0, 1);
#endif
    } else {
        // Desktop build uses traditional main loop
        while (!app.platform->shouldQuit()) {
            mainLoop();
            SDL_Delay(16); // ~60 FPS
        }
    }
    
    shutdown();
    return 0;
}

bool initialize() {
    // Initialize SDL_TTF globally
    if (TTF_Init() == -1) {
        printf("SDL_ttf initialization failed: %s\n", TTF_GetError());
        return false;
    }
    
    // Create platform abstraction
    app.platform = PlatformFactory::create();
    if (!app.platform) {
        printf("Failed to create platform abstraction\n");
        return false;
    }
    
    // Initialize platform
    if (!app.platform->initialize(WINDOW_WIDTH, WINDOW_HEIGHT, "Endjinn - Platform Abstracted")) {
        printf("Failed to initialize platform\n");
        return false;
    }
    
    // Create graphics abstraction
    app.graphics = GraphicsFactory::create();
    if (!app.graphics) {
        printf("Failed to create graphics abstraction\n");
        return false;
    }
    
    // Create text renderer
    app.textRenderer = std::make_unique<TextRenderer>(app.graphics.get());
    if (!app.textRenderer->initialize("DejaVuSansMono-Bold.ttf", 24, WINDOW_WIDTH, WINDOW_HEIGHT)) {
        printf("Failed to initialize text renderer\n");
        return false;
    }
    
    // Set up input handling
    app.platform->setKeyHandler(handleKeyPress);
    
    // Fix scaling issues
    app.platform->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    app.platform->setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    printf("Application initialized successfully!\n");
    printf("Platform: %s\n", app.platform->getPlatformName().c_str());
    printf("Graphics: %s\n", app.graphics->getRendererName().c_str());
    
    return true;
}

void mainLoop() {
    // Handle events - platform abstracted
    app.platform->pollEvents();
    
    // Render frame - graphics abstracted
    app.graphics->clearColor(0.1f, 0.1f, 0.3f, 1.0f);
    app.graphics->clear(GL_COLOR_BUFFER_BIT);
    
    // Render text - completely abstracted
    app.textRenderer->setColor(1.0f, 1.0f, 0.0f); // Yellow
    app.textRenderer->renderText("Platform Abstraction Success!", 50, 400);
    app.textRenderer->renderText("No preprocessor directives!", 50, 500);
    app.textRenderer->renderText("Write once, run everywhere!", 50, 600);
    
    // Present frame - platform abstracted
    app.platform->swapBuffers();
}

void handleKeyPress(int key) {
    printf("Key pressed: %d\n", key);
    // Add your key handling logic here
    // This function is completely platform-agnostic
}

void shutdown() {
    // Clean shutdown - all platform abstracted
    if (app.textRenderer) {
        app.textRenderer->cleanup();
        app.textRenderer.reset();
    }
    
    if (app.graphics) {
        app.graphics.reset();
    }
    
    if (app.platform) {
        app.platform->shutdown();
        app.platform.reset();
    }
    
    TTF_Quit();
    printf("Application shut down cleanly\n");
}