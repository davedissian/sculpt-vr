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
    auto frame = c.frame();
    std::cout << "Hands: " << frame.hands().count()
              << ", Fingers: " << frame.fingers().count() << std::endl;
    if (frame.hands().count() > 0)
      printHand(frame.hands()[0]);
    else
      std::cout << std::endl;
    std::cout << "\r\033[2A";
  }

  void printHand(const Leap::Hand& h)
  {
    std::cout << "Confidence: " << h.confidence()
              << ", Pinch: " << h.pinchStrength()
              << ", Position : ";
    printVec(h.stabilizedPalmPosition());
    std::cout << std::endl;
  }

  void printVec(const Leap::Vector& v)
  {
    std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")";
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
