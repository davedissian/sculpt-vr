#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class Framebuffer
{
public:
  /// Initialize an OpenGL Framebuffer object with color and depth.
  ///
  /// @param w  Buffer width
  /// @param h  Buffer height

  Framebuffer(int w, int h) : width(w), height(h)
  {
      const GLenum F = GL_FRAMEBUFFER;
      const GLenum T = GL_TEXTURE_2D;

      glGenTextures    (1, &color);
      glGenTextures    (1, &depth);
      glGenFramebuffers(1, &frame);

      // Initialize the color buffer.

      glBindTexture(T, color);

      glTexParameteri(T, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(T, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(T, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
      glTexParameteri(T, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

      glTexImage2D(T, 0, GL_RGBA8, w, h, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, 0);

      // Initialize the depth buffer.

      glBindTexture(T, depth);

      glTexParameteri(T, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(T, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(T, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
      glTexParameteri(T, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

      glTexImage2D(T, 0, GL_DEPTH_COMPONENT24, w, h, 0,
                         GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

      // Initialize the frame buffer.

      glBindFramebuffer(F, frame);

      glFramebufferTexture2D(F, GL_COLOR_ATTACHMENT0, T, color, 0);
      glFramebufferTexture2D(F, GL_DEPTH_ATTACHMENT,  T, depth, 0);

      check();

      // Unbind

      glBindFramebuffer(F, 0);
      glBindTexture    (T, 0);
  }

  /// Release all OpenGL resources.

  ~Framebuffer()
  {
      glDeleteFramebuffers(1, &frame);
      glDeleteTextures    (1, &color);
      glDeleteTextures    (1, &depth);
  }

  /// Query the Framebuffer status and throw any incompleteness error.

  void check() const
  {
      switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
      {
      case GL_FRAMEBUFFER_COMPLETE:
          break;
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
          throw std::runtime_error("Framebuffer incomplete attachment");
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
          throw std::runtime_error("Framebuffer missing attachment");
      case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
          throw std::runtime_error("Framebuffer draw buffer");
      case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
          throw std::runtime_error("Framebuffer read buffer");
      case GL_FRAMEBUFFER_UNSUPPORTED:
          throw std::runtime_error("Framebuffer unsupported");
      default:
          throw std::runtime_error("Framebuffer error");
      }
  }

  /// Bind hte Framebuffer and configure the viewport to fill it.

  void bind() const
  {
      glBindFramebuffer(GL_FRAMEBUFFER, frame);
      glViewport(0, 0, width, height);
  }

  GLsizei width;   ///< Buffer width
  GLsizei height;  ///< Buffer height
  GLuint  color;   ///< Color texture object
  GLuint  depth;   ///< Depth texture object
  GLuint  frame;   ///< Frame buffer object
};

#endif /* FRAMEBUFFER_H */
