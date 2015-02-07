#include "common.h"
#include "leapmotion.h"

class LeapListener : public Leap::Listener
{
public:
  virtual void onConnect(const Leap::Controller& c)
  {
    std::cout << "Connected to leap motion controller" << std::endl;
  }

  virtual void onFrame(const Leap::Controller& c)
  {
    std::cout << c.frame(0).hands().isEmpty() << std::endl;
  }
};

LeapListener listener;

LeapMotion::LeapMotion()
{
  controller.addListener(listener);
}

LeapMotion::~LeapMotion()
{
  controller.removeListener(listener);
}
