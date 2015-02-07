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
  void SDLCreateWindow(int width, int height, SDL_Window** window, SDL_GLContext* context, int monitor);
  void HMDConf();
  void GLInit();
  void GLDrawScene(const glm::mat4& view, const glm::mat4& proj);
  void GLRender();
  void GLCleanup();
  void RotateModel();
  glm::vec3 GetArcballVector(const glm::ivec2& pos);
  
  void DismissWarning();

private:
  volatile bool running;
  SDL_Window *window[2];
  SDL_GLContext context[2];

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
  
  /// Really big VBO.
  GLuint vbo;
  /// VAO for the Really big VBO.
  GLuint vao;

  // VR Stuff
  ovrHmd hmd;
  ovrVector3f offset[2];
  ovrTexture tex[2];
  ovrEyeRenderDesc erd[2];
  Framebuffer* buffer[2];
};


SculptVR::SculptVR()
  : running(false)
  , shModel("model")
  , shPlane("plane")
  , msGround(20.0f, 20.0f)
  , mouseDown(false)
  , viewQuat(0, 0, 0, 1)
  , cameraMat(glm::lookAt(
      glm::vec3(3.0f, 3.0f, 3.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f)))
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
  SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
  SDL_GL_SetSwapInterval(1);
  
  // Creat the window
  SDLCreateWindow(vpWidth, vpHeight, &window[0], &context[0], 0);
  SDLCreateWindow(hmd->Resolution.w, hmd->Resolution.h, &window[1], &context[1], 1);

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
      ovrGLTexture* p = reinterpret_cast<ovrGLTexture*>(tex + i);

      memset(p, 0, sizeof(ovrGLTexture));

      p->OGL.Header.API                 = ovrRenderAPI_OpenGL;
      p->OGL.Header.TextureSize         = size[i];
      p->OGL.Header.RenderViewport.Size = size[i];
      p->OGL.TexId                      = buffer[i]->color;
    }
  }
}

void SculptVR::SDLCreateWindow(int width, int height, SDL_Window** window, SDL_GLContext* context, int monitor)
{
  auto pos = SDL_WINDOWPOS_CENTERED_DISPLAY(monitor);
  *window = SDL_CreateWindow("SVR", pos, pos, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  *context = SDL_GL_CreateContext(*window);
}

static const Vertex temp[] =
{
  { -1.0f, 2.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF}, 
  {  1.0f, 2.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF},
  {  1.0f, 2.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF},
  { -1.0f, 2.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF},
  { -1.0f, 2.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF},
  {  1.0f, 2.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0, 0, 0xFF, 0xFF}
};


void SculptVR::GLInit()
{

  shPlane.compile("shader/plane.fs", Shader::Type::FRAG);
  shPlane.compile("shader/plane.vs", Shader::Type::VERT);
  shPlane.link();

  shModel.compile("shader/model.fs", Shader::Type::FRAG);
  shModel.compile("shader/model.vs", Shader::Type::VERT);
  shModel.link();

  msGround.create();

  // Set up the big VBO.
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(temp), temp, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 32, (void*)12);
  glVertexAttribPointer(2, 4, GL_UNSIGNED_SHORT, GL_FALSE, 32, (void*)24);
  
  glBindVertexArray(0);
}


void SculptVR::GLDrawScene(const glm::mat4& view, const glm::mat4& proj)
{
  // Render the ground plane.
  shPlane.bind();
  shPlane.uniform("u_proj", proj);
  shPlane.uniform("u_view", view);
  msGround.render(shPlane);

  // Render the model.
  shModel.bind();
  shModel.uniform("u_proj", proj);
  shModel.uniform("u_view", view);
  shModel.uniform("u_model", glm::translate(glm::vec3(0.0f, -3.0f, 0.0f)));
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void printMat(const float* f)
{
  for (int i = 0; i < 16; i++)
    std::cout << f[i] << ", ";
  std::cout << std::endl;
}

glm::mat4 convMat(const ovrMatrix4f& m)
{
  return glm::transpose(glm::mat4(m.M[0][0], m.M[0][1], m.M[0][2], m.M[0][3],
    m.M[1][0], m.M[1][1], m.M[1][2], m.M[1][3],
    m.M[2][0], m.M[2][1], m.M[2][2], m.M[2][3],
    m.M[3][0], m.M[3][1], m.M[3][2], m.M[3][3]));
}

void SculptVR::GLRender()
{
  glEnable(GL_DEPTH_TEST);
  
  // Render non-VR stuff
  SDL_GL_MakeCurrent(window[0], context[0]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GLDrawScene(cameraMat * glm::mat4_cast(viewQuat), glm::perspective(2.0f, 640.0f / 480.0f, 0.1f, 100.0f));
  SDL_GL_SwapWindow(window[0]);

  // Render VR stuff
  SDL_GL_MakeCurrent(window[1], context[1]);
  ovrPosef pose[2];
  ovrHmd_BeginFrame(hmd, 0);
  ovrHmd_GetEyePoses(hmd, 0, offset, pose, NULL);
  for (int i = 0; i < 2; i++)
  {
    auto eye = hmd->EyeRenderOrder[i];
    buffer[eye]->bind();
    auto proj = convMat(ovrMatrix4f_Projection(erd[eye].Fov, 0.1f, 100.0f, 1));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLDrawScene(cameraMat * glm::mat4_cast(viewQuat), proj);
  }
  ovrHmd_EndFrame(hmd, pose, tex);
  glUseProgram(0);

  GLuint err = glGetError();
  if (err != 0) {
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
        case SDL_KEYDOWN: {
          DismissWarning();
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
  SDL_DestroyWindow(window[0]);
  SDL_DestroyWindow(window[1]);
}

void SculptVR::DismissWarning()
{
    ovrHSWDisplayState state;

    ovrHmd_GetHSWDisplayState(hmd, &state);

    if (state.Displayed)
        ovrHmd_DismissHSWDisplay(hmd);
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
