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
  else
  {
    // Display statistics
    std::cout << "OVR Specs:" << std::endl
              << "\tName: " << hmd->ProductName << std::endl
              << "\tManufacturer: " << hmd->Manufacturer << std::endl
              << "\tResolution: " << hmd->Resolution.w << "x" << hmd->Resolution.h << std::endl;
  }
}

OculusVR::~OculusVR()
{
  if (hmd)
    ovrHmd_Destroy(hmd);
  ovr_Shutdown();
}
