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

    ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |
                                  ovrTrackingCap_MagYawCorrection |
                                  ovrTrackingCap_Position, 0);
  }
}

OculusVR::~OculusVR()
{
  if (hmd)
    ovrHmd_Destroy(hmd);
  ovr_Shutdown();
}


void printVec(const ovrVector3f& v)
{
  std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

void printQuat(const ovrQuatf& q)
{
  std::cout << "(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
}

void OculusVR::printState()
{
  auto state = ovrHmd_GetTrackingState(hmd, ovr_GetTimeInSeconds());
  std::cout << "Position: ";
  printVec(state.HeadPose.ThePose.Position);
  std::cout << ", Orientation: ";
  printQuat(state.HeadPose.ThePose.Orientation);
  std::cout << "\r";
}
