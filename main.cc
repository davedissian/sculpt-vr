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
  SDL_Renderer *renderer;
  Shader shModel;
};


SculptVR::SculptVR()
  : window(nullptr)
  , renderer(nullptr)
  , running(false)
  , shModel("model")
{
}


void SculptVR::Init()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("Cannot initialise SDL vide.");
  }
  
  // GL hints
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  
  if (!(window = SDL_CreateWindow(
      "SVR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480,
      SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL))) 
  {
    throw std::runtime_error("Cannot create SDL window.");
  }
  if (!(renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
  {
    throw std::runtime_error("Cannot create SDL renderer.");
  }
#ifndef __APPLE__
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
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
  shModel.compile("shader/model.fs", Shader::Type::FRAG);
  shModel.compile("Shader/model.vs", Shader::Type::VERT);
  shModel.link();
}


void SculptVR::GLRender()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
