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

void OculusVR::update()
{
  state = ovrHmd_GetTrackingState(hmd, ovr_GetTimeInSeconds());
}

glm::vec3 convVec(const ovrVector3f& v)
{
  return glm::vec3(v.x, v.y, v.z);
}

glm::quat convQuat(const ovrQuatf& q)
{
  return glm::quat(q.w, q.x, q.y, q.z);
}

int OculusVR::getEyeCount() const
{
  return ovrEye_Count;
}

EyeState OculusVR::getEyeState(int eyeIndex) const
{
  EyeState es;
  ovrEyeType eye = hmd->EyeRenderOrder[eyeIndex];
  ovrPosef pose = ovrHmd_GetHmdPosePerEye(hmd, eye);
  es.position = convVec(pose.Position);
  es.orientation = convQuat(pose.Orientation);
  return es;
}
