// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#include "common.h"

class SculptVR
{
public:
  SculptVR();

  ~SculptVR()
  {
    Destroy();
  }

  void Init();
  void Run();
  void Destroy();

private:
  void GLInit();
  void GLRender();
  void GLCleanup();
  void RotateModel();
  glm::vec3 GetArcballVector(const glm::ivec2& pos);

private:
  volatile bool running;
  SDL_Window *window;
  SDL_GLContext context;

  Shader shPlane;
  Shader shModel;
  Plane  msGround;

  int vpWidth = 640;
  int vpHeight = 480;
  bool mouseDown;
  glm::ivec2 mouseLast;
  glm::ivec2 mousePos;
  glm::quat viewQuat;
  glm::mat4 cameraMat;
};


SculptVR::SculptVR()
  : window(nullptr)
  , context(0)
  , running(false)
  , shModel("model")
  , shPlane("plane")
  , msGround(2.0f, 2.0f)
  , mouseDown(false)
  , viewQuat(0, 0, 0, 1)
  , cameraMat(glm::lookAt(
      glm::vec3(7.0f, 7.0f, 7.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f)))
{
}


void SculptVR::Init()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("Cannot initialise SDL video.");
  }
  
  // GL hints
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  
  // Creat the window
  window = SDL_CreateWindow(
      "SVR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480,
      SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  if (!window) 
  {
    throw std::runtime_error("Cannot create SDL window.");
  }

  // Create the GL context
  context = SDL_GL_CreateContext(window);
  SDL_GL_SetSwapInterval(1);

  // Set up GLEW
#ifndef __APPLE__
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    throw std::runtime_error("Cannot initialise GLEW.");
  }
#endif

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "OpenGL version supported " << version << std::endl;

  GLInit();
}


void SculptVR::GLInit()
{
  shPlane.compile("shader/plane.fs", Shader::Type::FRAG);
  shPlane.compile("shader/plane.vs", Shader::Type::VERT);
  shPlane.link();

  shModel.compile("shader/model.fs", Shader::Type::FRAG);
  shModel.compile("shader/model.vs", Shader::Type::VERT);
  shModel.link();

  msGround.create();
}


void SculptVR::GLRender()
{
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  shPlane.bind();
  shPlane.uniform("u_proj", glm::perspective(
      45.0f, 640.0f / 480.0f, 0.1f, 100.0f));
  shPlane.uniform("u_view", cameraMat);
  shPlane.uniform("u_model", glm::mat4_cast(viewQuat));
  msGround.render(shPlane);

  GLuint err = glGetError();
  if (err != 0)
  {
    std::runtime_error(std::to_string(err));
  }
}


void SculptVR::GLCleanup()
{
  shModel.destroy();
  shPlane.destroy();
  msGround.destroy();
}


void SculptVR::Run()
{
  SDL_Event evt;

  running = true;
  while (running)
  {
    while (SDL_PollEvent(&evt) != 0)
    {
      switch (evt.type)
      {
        case SDL_QUIT: {
          running = false;
          break;
        }
        case SDL_MOUSEBUTTONDOWN: {
          mouseLast = mousePos = glm::ivec2(evt.button.x, evt.button.y);
          mouseDown = true;
          break;
        }
        case SDL_MOUSEBUTTONUP: {
          mouseDown = false;
          break;
        }
        case SDL_MOUSEMOTION: {
          if (mouseDown) {
            mousePos = glm::ivec2(evt.motion.x, evt.motion.y);
            RotateModel();
          }
          break;
        }
        default: {
          break;
        }
      }
    }
    
    GLRender();
    SDL_GL_SwapWindow(window);
  }
}


glm::vec3 SculptVR::GetArcballVector(const glm::ivec2& pos)
{
  glm::vec3 p = glm::vec3(
      (float)pos.x / vpWidth * 2 - 1.0f,
      -(float)pos.y / vpHeight * 2 + 1.0f,
      0);

  float len = p.x * p.x + p.y * p.y;
  if (len <= 1.0f) {
    p.z = sqrt(1.0f - len);
  } else {
    p = glm::normalize(p);
  }
  return p;
}


void SculptVR::RotateModel()
{
  if (mouseLast == mousePos) {
    return;
  }

  glm::mat3 invCam = glm::mat3(glm::inverse(cameraMat));

  glm::vec3 va = GetArcballVector(mouseLast);
  glm::vec3 vb = GetArcballVector(mousePos);

  float angle = std::acos(std::min(1.0f, glm::dot(va, vb)));

  glm::vec3 axis = invCam * glm::cross(va, vb);
  if (glm::length(axis) >= 0.001f) {
    axis = glm::normalize(axis);
  }

  viewQuat = glm::angleAxis(2.0f * angle, axis) * viewQuat;
  mouseLast = mousePos;
}


void SculptVR::Destroy()
{
  GLCleanup();
  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }
}

int main()
{
  try {
    SculptVR vr;
    
    vr.Init();
    vr.Run();
    vr.Destroy();

    return EXIT_SUCCESS;
  } catch (std::exception& e) {
    std::cerr << "Unhandled exception: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
