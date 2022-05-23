#include "pch.h"
#include "FrameBuffer.h"

#include "GLError.h"

#include <glad/glad.h>

FrameBuffer::FrameBuffer(int width, int height)
    : m_width(width), m_height(height) 
{
    GL_CALL(glGenFramebuffers(1, &m_id));
}

FrameBuffer::FrameBuffer(FrameBuffer&& fbo)
{
    m_id = fbo.m_id;
    fbo.m_id = 0;
}

FrameBuffer::~FrameBuffer()
{
    GL_CALL(glDeleteFramebuffers(1, &m_id));
}

void FrameBuffer::attach_texture(AttachmentType attachment)
{
    switch (attachment)
    {
        case AttachmentType::Colour:
        {
            GL_CALL(glGenTextures(1, &m_colour_attachment));
            GL_CALL(glBindTexture(GL_TEXTURE_2D, m_colour_attachment));
            GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_INT, NULL));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colour_attachment, 0));
            break;   
        }

        case AttachmentType::Depth:
        {
            GL_CALL(glGenTextures(1, &m_depth_attachment));
            GL_CALL(glBindTexture(GL_TEXTURE_2D, m_depth_attachment));
            GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment, 0));
            break;   
        }

        case AttachmentType::Stencil:
        {
            GL_CALL(glGenTextures(1, &m_stencil_attachment));
            GL_CALL(glBindTexture(GL_TEXTURE_2D, m_stencil_attachment));
            GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, m_width, m_height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_INT, NULL));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_stencil_attachment, 0));
            break;   
        }
    
    default:
        break;
    }
}

void FrameBuffer::attach_renderbuffer(AttachmentType attachment)
{
    switch (attachment)
    {
        case AttachmentType::Colour:
        {
            GL_CALL(glGenRenderbuffers(1, &m_colour_attachment));
            GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, m_colour_attachment));
            GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, m_width, m_height));
            GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colour_attachment));
            break;   
        }

        case AttachmentType::Depth:
        {
            GL_CALL(glGenRenderbuffers(1, &m_depth_attachment));
            GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, m_depth_attachment));
            GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height));
            GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_attachment));
            break;   
        }

        case AttachmentType::Stencil:
        {
            GL_CALL(glGenRenderbuffers(1, &m_stencil_attachment));
            GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, m_stencil_attachment));
            GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, m_width, m_height));
            GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencil_attachment));
            break;   
        }
    
    default:
        break;
    }
}

void FrameBuffer::bind() const
{
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
}

void FrameBuffer::unbind() const
{
    // go back to using default frame buffer
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

// fbo needs to be bound first
bool FrameBuffer::is_complete() const
{
    return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}
