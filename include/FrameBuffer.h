#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#define GL_GLEXT_PROTOTYPES

#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "Texture.h"
#include "Debug.h"

class FrameBuffer
{
private:

    std::vector<GLenum> drawBuffers;
    std::vector<Texture*> textures;
    GLuint numBuffers, object;
    int width, height;

    FrameBuffer(const FrameBuffer&) {}

public:

    FrameBuffer(int widht, int height);
    ~FrameBuffer();

    void Bind() const;
    void UnBind() const;

    Texture *GetTexture(GLenum target);
    void SetDrawingBuffers(int n, GLenum* bufs);

    GLuint GetObject() const { return object; }
};

#endif // FRAMEBUFFER_H
