#include "common.h"
#include "oculusvr.h"

OculusVR::OculusVR()
  : hmd(0)
{
  ovr_Initialize();
  hmd = ovrHmd_Create(0);
  if (!hmd)
  {
    std::cerr << "There was an error creating the HMD handle - is the oculus rift plugged in?" << std::endl;
  }
}

OculusVR::~OculusVR()
{
}
