#include <iostream>
using namespace std;

#include <OVR.h>
#include <Leap.h>

class LeapListener : public Leap::Listener
{
public:
  virtual void onConnect(const Leap::Controller& c)
  {
    cout << "Connected to leap motion controller" << endl;
  }

  virtual void onFrame(const Leap::Controller& c)
  {
    cout << c.frame(0).hands().isEmpty() << endl;
  }
};

int main()
{
  ovr_Initialize();
  ovrHmd hmd = ovrHmd_Create(0);
  if (!hmd)
  {
    cerr << "There was an error creating the HMD handle - is the rift plugged in?" << endl;
  }

  // Get more details
#if 0
  cout << "Res X: " << hmd->Resolution.w << " - Res Y: " << hmd->Resolution.h << endl;
#endif
  // Set up leap motion
  LeapListener listener;
  Leap::Controller controller;
  controller.addListener(listener);

  // Do stuff
  cin.get();
  return 0;
}
