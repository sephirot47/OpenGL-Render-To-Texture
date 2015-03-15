#include "../include/FrameBuffer.h"

FrameBuffer::FrameBuffer(int width, int height)
{
    glGenFramebuffers(1, &object);
    numBuffers = 0;

    this->width = width;
    this->height = height;
}

FrameBuffer::~FrameBuffer()
{
    textures.clear();
    glDeleteFramebuffers(1, &object);
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, object);
    glDrawBuffers(numBuffers, &drawBuffers[0]);
}

void FrameBuffer::UnBind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture *FrameBuffer::GetTexture(GLenum target)
{
    for(int i = 0; i < numBuffers; ++i)
        if(drawBuffers[i] == target)
            return textures[i];
    return 0;
}

void FrameBuffer::SetDrawingBuffers(int n, GLenum* bufs)
{
    if(n <= 0) return;
    numBuffers = n;


    drawBuffers.clear(); //Copiamos los buffers a drawBuffers
    for(GLenum *buffer = bufs; buffer < (bufs + numBuffers); ++buffer)
        drawBuffers.push_back(*buffer);

    Bind(); //Aqui se hace el glDrawBuffers

    textures.clear();
    for(int i = 0; i < numBuffers; ++i)
    {
        if(drawBuffers[i] >= GL_COLOR_ATTACHMENT0 && drawBuffers[i] <= GL_COLOR_ATTACHMENT15)
        {
            Texture *texture = new Texture();
            texture->SetData(0, width, height, GL_RGB, GL_RGB, GL_FLOAT);
            texture->SetWrapMode(GL_CLAMP_TO_EDGE);
            texture->SetScaleMode(GL_LINEAR);
            textures.push_back(texture);
            glFramebufferTexture2D(GL_FRAMEBUFFER, bufs[i], GL_TEXTURE_2D, texture->GetObject(), 0);
        }
        else if(drawBuffers[i] == GL_DEPTH_ATTACHMENT)
        {
            Texture *texture = new Texture();
            texture->SetData(0, width, height, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT);
            texture->SetWrapMode(GL_CLAMP_TO_EDGE);
            texture->SetScaleMode(GL_NEAREST);
            textures.push_back(texture);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, texture->GetObject(), 0);
        }
    }

    UnBind();
}
