#ifndef LEAPMOTION_H
#define LEAPMOTION_H

#include <Leap.h>

class LeapMotion
{
public:
  LeapMotion();
  ~LeapMotion();

private:
  Leap::Controller controller;

};

#endif /* LEAPMOTION_H */
