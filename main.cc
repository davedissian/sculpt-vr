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

private:
  volatile bool running;
  SDL_Window *window;
  SDL_GLContext context;

  Shader shPlane;
  Shader shModel;
  Plane  msGround;
};


SculptVR::SculptVR()
  : window(nullptr)
  , renderer(nullptr)
  , running(false)
  , shModel("model")
  , shPlane("plane")
  , msGround(20.0f, 20.0f, 40, 40)
{
}


void SculptVR::Init()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("Cannot initialise SDL video.");
  }
  
  // GL hints
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  
  // Creat the window
  window = SDL_CreateWindow(
      "SVR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480,
      SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL)
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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shModel.bind();
  shModel.uniform("u_proj", glm::perspective(
    45.0f, 640.0f / 480.0f, 0.1f, 100.0f));
  shModel.uniform("u_view", glm::lookAt(
      glm::vec3(10.0f, 10.0f, 10.0f), 
      glm::vec3(0.0f, 0.0f, 0.0f), 
      glm::vec3(0.0f, 1.0f, 0.0)));
  msGround.render(shModel);
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
  while (running) {
    SDL_WaitEvent(&evt);
    switch (evt.type) {
      case SDL_QUIT: {
        running = false;
        break;
      }
    }


    GLRender();
    SDL_RenderPresent(renderer);
  }
}


void SculptVR::Destroy()
{
  GLCleanup();
  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
  }
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
