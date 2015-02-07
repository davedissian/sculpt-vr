// This file is part of the Sculpt-VR Project.
// Licensing information can be found in the LICENSE file.
// (C) 2014 The Sculpt-VR Team. All rights reserved.

#include "common.h"
#include "framebuffer.h"

#include <../Src/OVR_CAPI_GL.h>

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
  void HMDInit();
  void HMDConf();
  void GLInit();
  void GLDrawScene();
  void GLRender();
  void GLCleanup();

private:
  volatile bool running;
  SDL_Window *window;
  SDL_GLContext context;

  Shader shPlane;
  Shader shModel;
  Plane  msGround;

  glm::mat4 view;
  glm::mat4 proj;

  // VR Stuff
  ovrHmd hmd;
  ovrVector3f offset[2];
  ovrTexture tex[2];
  ovrEyeRenderDesc erd[2];
  Framebuffer* buffer[2];
  
};


SculptVR::SculptVR()
  : running(false)
  , window(nullptr)
  , context(0)
  , shModel("model")
  , shPlane("plane")
  , msGround(20.0f, 20.0f, 40, 40)
{
}

void SculptVR::Init()
{
  HMDInit();

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("Cannot initialise SDL video.");
  }
  
  // GL hints
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  
  // Creat the window
  window = SDL_CreateWindow(
      "SVR", SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1), hmd->Resolution.w, hmd->Resolution.h,
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

  HMDConf();
  GLInit();
}

void SculptVR::HMDInit()
{
  ovr_Initialize();
  hmd = ovrHmd_Create(0);
  if (!hmd)
  {
    std::cerr << "Can't create the HMD handle - is the oculus rift plugged in?" << std::endl;
    std::cerr << "Using debug HMD" << std::endl;
    hmd = ovrHmd_CreateDebug(ovrHmd_DK2);
  }
  
  // Display statistics
  std::cout << "OVR Specs:" << std::endl
            << "\tName: " << hmd->ProductName << std::endl
            << "\tManufacturer: " << hmd->Manufacturer << std::endl
            << "\tResolution: " << hmd->Resolution.w << "x" << hmd->Resolution.h << std::endl;
}

void SculptVR::HMDConf()
{

  ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |
                                ovrTrackingCap_MagYawCorrection |
                                ovrTrackingCap_Position, 0);
  // Set the configuration
  ovrGLConfig cfg;
  memset(&cfg, 0, sizeof(ovrGLConfig));
  cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
  cfg.OGL.Header.BackBufferSize.w = hmd->Resolution.w;
  cfg.OGL.Header.BackBufferSize.h = hmd->Resolution.h;

  ovrHmd_ConfigureRendering(hmd, &cfg.Config,
      ovrDistortionCap_Chromatic | ovrDistortionCap_TimeWarp, hmd->DefaultEyeFov, erd);
  
  offset[0] = erd[0].HmdToEyeViewOffset;
  offset[1] = erd[1].HmdToEyeViewOffset;

  ovrSizei size[2];

  size[0] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left,  hmd->DefaultEyeFov[0], 1);
  size[1] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[1], 1);

  // Initialize the off-screen render buffers. We're using one buffer per-eye
  // instead of concatenating both eyes into a single buffer.
  for (int i = 0; i < 2; i++)
  {
    if ((buffer[i] = new Framebuffer(size[i].w, size[i].h)))
    {
      ovrGLTexture *p = reinterpret_cast<ovrGLTexture*>(tex + i);

      memset(p, 0, sizeof (ovrGLTexture));

      p->OGL.Header.API                 = ovrRenderAPI_OpenGL;
      p->OGL.Header.TextureSize         = size[i];
      p->OGL.Header.RenderViewport.Size = size[i];
      p->OGL.TexId                      = buffer[i]->color;
    }
  }
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


void SculptVR::GLDrawScene()
{
  shPlane.bind();
  shPlane.uniform("u_proj", proj);
  shPlane.uniform("u_view", view);
  msGround.render(shPlane);
}

glm::mat4 convMat(const ovrMatrix4f& m)
{
  return glm::mat4(m.M[0][0], m.M[0][1], m.M[0][2], m.M[0][3],
                m.M[1][0], m.M[1][1], m.M[1][2], m.M[1][3],
                m.M[2][0], m.M[2][1], m.M[2][2], m.M[2][3],
                m.M[3][0], m.M[3][1], m.M[3][2], m.M[3][3]);
}

void SculptVR::GLRender()
{
  ovrPosef pose[2];
 ovrHmd_BeginFrame(hmd, 0);
  {
      ovrHmd_GetEyePoses(hmd, 0, offset, pose, NULL);

      for (int i = 0; i < 2; i++)
      {
          auto eye = hmd->EyeRenderOrder[i];
          buffer[eye]->bind();
          GLDrawScene();
      }
  }
  ovrHmd_EndFrame(hmd, pose, tex);
  glUseProgram(0);
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
        case SDL_QUIT:
          running = false;
          break;

        default:
          break;
     }
    }
    
    GLRender();
  }
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
