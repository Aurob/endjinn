#include "graphics_core.h"
#include <iostream>

GraphicsCore::GraphicsCore() : currentVAO(0) {
}

GraphicsCore::~GraphicsCore() {
    if (currentVAO) {
        glDeleteVertexArrays(1, &currentVAO);
    }
}

GLuint GraphicsCore::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Core Shader compilation failed: %s\n", infoLog);
        return 0;
    }
    
    return shader;
}

GLuint GraphicsCore::createProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("Core Program linking failed: %s\n", infoLog);
        return 0;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

void GraphicsCore::useProgram(GLuint program) {
    glUseProgram(program);
}

void GraphicsCore::deleteProgram(GLuint program) {
    glDeleteProgram(program);
}

void GraphicsCore::setUniform1f(GLuint program, const std::string& name, float value) {
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniform1f(location, value);
}

void GraphicsCore::setUniform3f(GLuint program, const std::string& name, float x, float y, float z) {
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniform3f(location, x, y, z);
}

void GraphicsCore::setUniform1i(GLuint program, const std::string& name, int value) {
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniform1i(location, value);
}

GLuint GraphicsCore::createBuffer() {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

void GraphicsCore::bindBuffer(GLenum target, GLuint buffer) {
    glBindBuffer(target, buffer);
}

void GraphicsCore::bufferData(GLenum target, size_t size, const void* data, GLenum usage) {
    glBufferData(target, size, data, usage);
}

void GraphicsCore::deleteBuffer(GLuint buffer) {
    glDeleteBuffers(1, &buffer);
}

GLuint GraphicsCore::createTexture() {
    GLuint texture;
    glGenTextures(1, &texture);
    return texture;
}

void GraphicsCore::bindTexture(GLenum target, GLuint texture) {
    glBindTexture(target, texture);
}

void GraphicsCore::texImage2D(GLenum target, GLint level, GLint internalFormat, 
                             int width, int height, GLenum format, GLenum type, const void* data) {
    glTexImage2D(target, level, internalFormat, width, height, 0, format, type, data);
}

void GraphicsCore::texParameteri(GLenum target, GLenum pname, GLint param) {
    glTexParameteri(target, pname, param);
}

void GraphicsCore::deleteTexture(GLuint texture) {
    glDeleteTextures(1, &texture);
}

void GraphicsCore::activeTexture(GLenum texture) {
    glActiveTexture(texture);
}

void GraphicsCore::setupVertexArray(GLuint program, GLuint buffer) {
    if (!currentVAO) {
        glGenVertexArrays(1, &currentVAO);
    }
    glBindVertexArray(currentVAO);
    bindBuffer(GL_ARRAY_BUFFER, buffer);
}

void GraphicsCore::enableVertexAttribute(GLuint program, const std::string& name, 
                                        int size, GLenum type, int stride, int offset) {
    // Core profile uses layout locations, so we use hardcoded locations
    // This assumes the shader uses layout(location = N) in vec* name;
    GLuint location = 0;
    if (name == "aPosition") location = 0;
    else if (name == "aTexCoord") location = 1;
    // Add more as needed
    
    glVertexAttribPointer(location, size, type, GL_FALSE, stride, (void*)(intptr_t)offset);
    glEnableVertexAttribArray(location);
}

void GraphicsCore::disableVertexAttribute(GLuint program, const std::string& name) {
    GLuint location = 0;
    if (name == "aPosition") location = 0;
    else if (name == "aTexCoord") location = 1;
    
    glDisableVertexAttribArray(location);
}

void GraphicsCore::drawArrays(GLenum mode, GLint first, int count) {
    glDrawArrays(mode, first, count);
}

void GraphicsCore::enable(GLenum cap) {
    glEnable(cap);
}

void GraphicsCore::blendFunc(GLenum sfactor, GLenum dfactor) {
    glBlendFunc(sfactor, dfactor);
}

void GraphicsCore::clearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void GraphicsCore::clear(GLuint mask) {
    glClear(mask);
}

std::string GraphicsCore::getRendererName() const {
    return "OpenGL 3.3 Core";
}

bool GraphicsCore::supportsVertexArrays() const {
    return true;
}

std::string GraphicsCore::getVertexShaderPath(const std::string& baseName) const {
    return "shaders/" + baseName + "_vertex_core.glsl";
}

std::string GraphicsCore::getFragmentShaderPath(const std::string& baseName) const {
    return "shaders/" + baseName + "_fragment_core.glsl";
}