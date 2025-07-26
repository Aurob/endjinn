#pragma once

#include "graphics/graphics_api.h"
#include <string>
#include <memory>

class Shader {
public:
    GLuint program;
    
    Shader(GraphicsAPI* graphics);
    ~Shader();
    
    // Load and compile shaders from files
    bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    
    // Use the shader program
    void use();
    
    // Utility functions for setting uniforms
    void setFloat(const std::string& name, float value);
    void setVec3(const std::string& name, float x, float y, float z);
    void setInt(const std::string& name, int value);
    
private:
    GraphicsAPI* graphics;
    
    // Load file contents as string
    std::string loadFile(const std::string& filepath);
};