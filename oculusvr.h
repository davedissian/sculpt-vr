#ifndef OCULUSVR_H
#define OCULUSVR_H

#include <OVR.h>

class OculusVR
{
public:
  OculusVR();
  ~OculusVR();

  void printState();

private:
  ovrHmd hmd;

};

#endif /* OCULUSVR_H */