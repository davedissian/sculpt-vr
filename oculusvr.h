#ifndef OCULUSVR_H
#define OCULUSVR_H

#include <OVR.h>

struct EyeState
{
  glm::vec3 position;
  glm::quat orientation;
  float fov;
};

class OculusVR
{
public:
  OculusVR();
  ~OculusVR();

  // Call this before rendering
  void update();

  int getEyeCount() const;
  EyeState getEyeState(int eyeIndex) const;

private:
  ovrHmd hmd;
  ovrTrackingState state;

};

#endif /* OCULUSVR_H */
