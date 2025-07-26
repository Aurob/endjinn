#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(GraphicsAPI* graphics) : program(0), graphics(graphics) {
}

Shader::~Shader() {
    if (program && graphics) {
        graphics->deleteProgram(program);
    }
}

bool Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    // Load source code from files
    std::string vertexSource = loadFile(vertexPath);
    std::string fragmentSource = loadFile(fragmentPath);
    
    if (vertexSource.empty() || fragmentSource.empty()) {
        printf("Failed to load shader files\n");
        return false;
    }
    
    // Compile shaders using graphics API
    GLuint vertexShader = graphics->compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = graphics->compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    
    if (!vertexShader || !fragmentShader) {
        printf("Failed to compile shaders\n");
        return false;
    }
    
    // Create program using graphics API
    program = graphics->createProgram(vertexShader, fragmentShader);
    if (!program) {
        printf("Failed to create shader program\n");
        return false;
    }
    
    printf("Shader program created successfully: %u\n", program);
    return true;
}

void Shader::use() {
    if (graphics) {
        graphics->useProgram(program);
    }
}

void Shader::setFloat(const std::string& name, float value) {
    if (graphics) {
        graphics->setUniform1f(program, name, value);
    }
}

void Shader::setVec3(const std::string& name, float x, float y, float z) {
    if (graphics) {
        graphics->setUniform3f(program, name, x, y, z);
    }
}

void Shader::setInt(const std::string& name, int value) {
    if (graphics) {
        graphics->setUniform1i(program, name, value);
    }
}

std::string Shader::loadFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        printf("Failed to open file: %s\n", filepath.c_str());
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}