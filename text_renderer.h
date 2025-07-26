#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>
#include "shader.h"
#include "graphics/graphics_api.h"

class TextRenderer {
public:
    TextRenderer(GraphicsAPI* graphics);
    ~TextRenderer();
    
    // Initialize the text renderer
    bool initialize(const std::string& fontPath, int fontSize, int windowWidth, int windowHeight);
    
    // Render text at specified position
    void renderText(const std::string& text, float x, float y);
    
    // Set text color
    void setColor(float r, float g, float b);
    
    // Cleanup resources
    void cleanup();
    
private:
    GraphicsAPI* graphics;
    TTF_Font* font;
    std::unique_ptr<Shader> textShader;
    GLuint VBO;
    GLuint textTexture;
    int screenWidth, screenHeight;
    float textColor[3];
    
    // Convert SDL surface to proper format
    SDL_Surface* convertSurface(SDL_Surface* surface);
};