#include "text_renderer.h"
#include <iostream>

TextRenderer::TextRenderer(GraphicsAPI* graphics) 
    : graphics(graphics), font(nullptr), VBO(0), textTexture(0), screenWidth(0), screenHeight(0) {
    textColor[0] = 1.0f; // Default to white
    textColor[1] = 1.0f;
    textColor[2] = 1.0f;
}

TextRenderer::~TextRenderer() {
    cleanup();
}

bool TextRenderer::initialize(const std::string& fontPath, int fontSize, int windowWidth, int windowHeight) {
    screenWidth = windowWidth;
    screenHeight = windowHeight;
    
    // Load font
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return false;
    }
    printf("Font loaded successfully\n");
    
    // Create shader using graphics abstraction
    textShader = std::make_unique<Shader>(graphics);
    
    // Load appropriate shaders based on graphics API
    std::string vertexPath = graphics->getVertexShaderPath("text");
    std::string fragmentPath = graphics->getFragmentShaderPath("text");
    
    if (!textShader->loadFromFiles(vertexPath, fragmentPath)) {
        printf("Failed to load text shaders\n");
        return false;
    }
    
    // Create OpenGL resources using graphics API
    VBO = graphics->createBuffer();
    textTexture = graphics->createTexture();
    
    // Enable blending for text rendering
    graphics->enable(GL_BLEND);
    graphics->blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    printf("Text renderer initialized successfully\n");
    return true;
}

void TextRenderer::renderText(const std::string& text, float x, float y) {
    if (!font) {
        printf("Font not loaded\n");
        return;
    }
    
    SDL_Color color = {255, 255, 255, 255}; // White text in SDL surface
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        printf("Failed to render text: %s\n", TTF_GetError());
        return;
    }
    
    // Convert surface to proper format
    SDL_Surface* rgba_surface = convertSurface(surface);
    if (!rgba_surface) {
        SDL_FreeSurface(surface);
        return;
    }
    
    // Upload texture using graphics API
    graphics->bindTexture(GL_TEXTURE_2D, textTexture);
    graphics->texImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgba_surface->w, rgba_surface->h, GL_RGBA, GL_UNSIGNED_BYTE, rgba_surface->pixels);
    graphics->texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    graphics->texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    graphics->texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    graphics->texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Calculate normalized coordinates
    float w = (float)surface->w / screenWidth * 2.0f;
    float h = (float)surface->h / screenHeight * 2.0f;
    float x_norm = x / screenWidth * 2.0f - 1.0f;
    float y_norm = 1.0f - y / screenHeight * 2.0f;
    
    // Create quad vertices
    float vertices[] = {
        x_norm,     y_norm - h, 0.0f, 1.0f,
        x_norm + w, y_norm - h, 1.0f, 1.0f,
        x_norm + w, y_norm,     1.0f, 0.0f,
        
        x_norm,     y_norm - h, 0.0f, 1.0f,
        x_norm + w, y_norm,     1.0f, 0.0f,
        x_norm,     y_norm,     0.0f, 0.0f
    };
    
    // Use shader and setup rendering
    textShader->use();
    
    // Setup vertex array using graphics API abstraction
    graphics->setupVertexArray(textShader->program, VBO);
    
    // Update vertex buffer
    graphics->bindBuffer(GL_ARRAY_BUFFER, VBO);
    graphics->bufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    // Setup vertex attributes using graphics API abstraction
    graphics->enableVertexAttribute(textShader->program, "aPosition", 2, GL_FLOAT, 4 * sizeof(float), 0);
    graphics->enableVertexAttribute(textShader->program, "aTexCoord", 2, GL_FLOAT, 4 * sizeof(float), 2 * sizeof(float));
    
    // Set uniforms
    textShader->setVec3("uColor", textColor[0], textColor[1], textColor[2]);
    textShader->setInt("uTexture", 0);
    
    // Bind texture and draw
    graphics->activeTexture(GL_TEXTURE0);
    graphics->bindTexture(GL_TEXTURE_2D, textTexture);
    
    graphics->drawArrays(GL_TRIANGLES, 0, 6);
    
    // Cleanup
    graphics->disableVertexAttribute(textShader->program, "aPosition");
    graphics->disableVertexAttribute(textShader->program, "aTexCoord");
    
    // Free surfaces
    if (rgba_surface != surface) {
        SDL_FreeSurface(rgba_surface);
    }
    SDL_FreeSurface(surface);
}

void TextRenderer::setColor(float r, float g, float b) {
    textColor[0] = r;
    textColor[1] = g;
    textColor[2] = b;
}

void TextRenderer::cleanup() {
    if (VBO && graphics) {
        graphics->deleteBuffer(VBO);
        VBO = 0;
    }
    if (textTexture && graphics) {
        graphics->deleteTexture(textTexture);
        textTexture = 0;
    }
    
    textShader.reset();
    
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

SDL_Surface* TextRenderer::convertSurface(SDL_Surface* surface) {
    if (surface->format->format != SDL_PIXELFORMAT_RGBA32) {
        SDL_Surface* rgba_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
        if (!rgba_surface) {
            printf("Failed to convert surface format: %s\n", SDL_GetError());
            return nullptr;
        }
        return rgba_surface;
    }
    return surface;
}