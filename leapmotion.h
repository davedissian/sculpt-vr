#ifndef LEAP_H
#define LEAP_H

#include <Leap.h>

class LeapMotion
{
public:
  LeapMotion();
  ~LeapMotion();

private:
  Leap::Controller controller;

};

#endif /* LEAP_H */
