#pragma once

#include <string>

// OpenGL types - platform independent
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef int GLint;
typedef float GLfloat;

// OpenGL constants - platform independent
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_BLEND                          0x0BE2
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_TEXTURE_2D                     0x0DE1
#define GL_RGBA                           0x1908
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_LINEAR                         0x2601
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_ARRAY_BUFFER                   0x8892
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_TRIANGLES                      0x0004
#define GL_FLOAT                          0x1406
#define GL_FALSE                          0
#define GL_TEXTURE0                       0x84C0

class GraphicsAPI {
public:
    virtual ~GraphicsAPI() = default;
    
    // Shader operations
    virtual GLuint compileShader(GLenum type, const std::string& source) = 0;
    virtual GLuint createProgram(GLuint vertexShader, GLuint fragmentShader) = 0;
    virtual void useProgram(GLuint program) = 0;
    virtual void deleteProgram(GLuint program) = 0;
    
    // Uniform operations
    virtual void setUniform1f(GLuint program, const std::string& name, float value) = 0;
    virtual void setUniform3f(GLuint program, const std::string& name, float x, float y, float z) = 0;
    virtual void setUniform1i(GLuint program, const std::string& name, int value) = 0;
    
    // Buffer operations
    virtual GLuint createBuffer() = 0;
    virtual void bindBuffer(GLenum target, GLuint buffer) = 0;
    virtual void bufferData(GLenum target, size_t size, const void* data, GLenum usage) = 0;
    virtual void deleteBuffer(GLuint buffer) = 0;
    
    // Texture operations
    virtual GLuint createTexture() = 0;
    virtual void bindTexture(GLenum target, GLuint texture) = 0;
    virtual void texImage2D(GLenum target, GLint level, GLint internalFormat, 
                           int width, int height, GLenum format, GLenum type, const void* data) = 0;
    virtual void texParameteri(GLenum target, GLenum pname, GLint param) = 0;
    virtual void deleteTexture(GLuint texture) = 0;
    virtual void activeTexture(GLenum texture) = 0;
    
    // Vertex array operations (abstracted for ES compatibility)
    virtual void setupVertexArray(GLuint program, GLuint buffer) = 0;
    virtual void enableVertexAttribute(GLuint program, const std::string& name, 
                                     int size, GLenum type, int stride, int offset) = 0;
    virtual void disableVertexAttribute(GLuint program, const std::string& name) = 0;
    
    // Drawing
    virtual void drawArrays(GLenum mode, GLint first, int count) = 0;
    
    // State management
    virtual void enable(GLenum cap) = 0;
    virtual void blendFunc(GLenum sfactor, GLenum dfactor) = 0;
    virtual void clearColor(float r, float g, float b, float a) = 0;
    virtual void clear(GLuint mask) = 0;
    
    // Platform info
    virtual std::string getRendererName() const = 0;
    virtual bool supportsVertexArrays() const = 0;
    
    // Shader path resolution
    virtual std::string getVertexShaderPath(const std::string& baseName) const = 0;
    virtual std::string getFragmentShaderPath(const std::string& baseName) const = 0;
};