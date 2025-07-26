#pragma once

#include "graphics_api.h"
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

class GraphicsCore : public GraphicsAPI {
public:
    GraphicsCore();
    ~GraphicsCore() override;
    
    GLuint compileShader(GLenum type, const std::string& source) override;
    GLuint createProgram(GLuint vertexShader, GLuint fragmentShader) override;
    void useProgram(GLuint program) override;
    void deleteProgram(GLuint program) override;
    
    void setUniform1f(GLuint program, const std::string& name, float value) override;
    void setUniform3f(GLuint program, const std::string& name, float x, float y, float z) override;
    void setUniform1i(GLuint program, const std::string& name, int value) override;
    
    GLuint createBuffer() override;
    void bindBuffer(GLenum target, GLuint buffer) override;
    void bufferData(GLenum target, size_t size, const void* data, GLenum usage) override;
    void deleteBuffer(GLuint buffer) override;
    
    GLuint createTexture() override;
    void bindTexture(GLenum target, GLuint texture) override;
    void texImage2D(GLenum target, GLint level, GLint internalFormat, 
                   int width, int height, GLenum format, GLenum type, const void* data) override;
    void texParameteri(GLenum target, GLenum pname, GLint param) override;
    void deleteTexture(GLuint texture) override;
    void activeTexture(GLenum texture) override;
    
    void setupVertexArray(GLuint program, GLuint buffer) override;
    void enableVertexAttribute(GLuint program, const std::string& name, 
                             int size, GLenum type, int stride, int offset) override;
    void disableVertexAttribute(GLuint program, const std::string& name) override;
    
    void drawArrays(GLenum mode, GLint first, int count) override;
    
    void enable(GLenum cap) override;
    void blendFunc(GLenum sfactor, GLenum dfactor) override;
    void clearColor(float r, float g, float b, float a) override;
    void clear(GLuint mask) override;
    
    std::string getRendererName() const override;
    bool supportsVertexArrays() const override;
    
    std::string getVertexShaderPath(const std::string& baseName) const override;
    std::string getFragmentShaderPath(const std::string& baseName) const override;

private:
    GLuint currentVAO;
};