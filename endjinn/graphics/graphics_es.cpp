#include "graphics_es.h"
#include <iostream>

GraphicsES::GraphicsES() : currentProgram(0) {
}

GraphicsES::~GraphicsES() {
}

GLuint GraphicsES::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ES Shader compilation failed: %s\n", infoLog);
        return 0;
    }
    
    return shader;
}

GLuint GraphicsES::createProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("ES Program linking failed: %s\n", infoLog);
        return 0;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

void GraphicsES::useProgram(GLuint program) {
    currentProgram = program;
    glUseProgram(program);
}

void GraphicsES::deleteProgram(GLuint program) {
    glDeleteProgram(program);
}

void GraphicsES::setUniform1f(GLuint program, const std::string& name, float value) {
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniform1f(location, value);
}

void GraphicsES::setUniform3f(GLuint program, const std::string& name, float x, float y, float z) {
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniform3f(location, x, y, z);
}

void GraphicsES::setUniform1i(GLuint program, const std::string& name, int value) {
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniform1i(location, value);
}

GLuint GraphicsES::createBuffer() {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

void GraphicsES::bindBuffer(GLenum target, GLuint buffer) {
    glBindBuffer(target, buffer);
}

void GraphicsES::bufferData(GLenum target, size_t size, const void* data, GLenum usage) {
    glBufferData(target, size, data, usage);
}

void GraphicsES::deleteBuffer(GLuint buffer) {
    glDeleteBuffers(1, &buffer);
}

GLuint GraphicsES::createTexture() {
    GLuint texture;
    glGenTextures(1, &texture);
    return texture;
}

void GraphicsES::bindTexture(GLenum target, GLuint texture) {
    glBindTexture(target, texture);
}

void GraphicsES::texImage2D(GLenum target, GLint level, GLint internalFormat, 
                           int width, int height, GLenum format, GLenum type, const void* data) {
    glTexImage2D(target, level, internalFormat, width, height, 0, format, type, data);
}

void GraphicsES::texParameteri(GLenum target, GLenum pname, GLint param) {
    glTexParameteri(target, pname, param);
}

void GraphicsES::deleteTexture(GLuint texture) {
    glDeleteTextures(1, &texture);
}

void GraphicsES::activeTexture(GLenum texture) {
    glActiveTexture(texture);
}

void GraphicsES::setupVertexArray(GLuint program, GLuint buffer) {
    // ES doesn't have VAOs, just bind the buffer
    bindBuffer(GL_ARRAY_BUFFER, buffer);
}

void GraphicsES::enableVertexAttribute(GLuint program, const std::string& name, 
                                     int size, GLenum type, int stride, int offset) {
    GLint location = getAttributeLocation(program, name);
    if (location >= 0) {
        glVertexAttribPointer(location, size, type, GL_FALSE, stride, (void*)(intptr_t)offset);
        glEnableVertexAttribArray(location);
    }
}

void GraphicsES::disableVertexAttribute(GLuint program, const std::string& name) {
    GLint location = getAttributeLocation(program, name);
    if (location >= 0) {
        glDisableVertexAttribArray(location);
    }
}

void GraphicsES::drawArrays(GLenum mode, GLint first, int count) {
    glDrawArrays(mode, first, count);
}

void GraphicsES::enable(GLenum cap) {
    glEnable(cap);
}

void GraphicsES::blendFunc(GLenum sfactor, GLenum dfactor) {
    glBlendFunc(sfactor, dfactor);
}

void GraphicsES::clearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void GraphicsES::clear(GLuint mask) {
    glClear(mask);
}

std::string GraphicsES::getRendererName() const {
    return "OpenGL ES 2.0";
}

bool GraphicsES::supportsVertexArrays() const {
    return false;
}

std::string GraphicsES::getVertexShaderPath(const std::string& baseName) const {
    return "shaders/" + baseName + "_vertex_es.glsl";
}

std::string GraphicsES::getFragmentShaderPath(const std::string& baseName) const {
    return "shaders/" + baseName + "_fragment_es.glsl";
}

GLint GraphicsES::getAttributeLocation(GLuint program, const std::string& name) {
    std::string key = std::to_string(program) + "_" + name;
    auto it = attributeCache.find(key);
    if (it != attributeCache.end()) {
        return it->second;
    }
    
    GLint location = glGetAttribLocation(program, name.c_str());
    attributeCache[key] = location;
    return location;
}