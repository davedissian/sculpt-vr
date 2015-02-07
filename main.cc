#include <iostream>
using namespace std;

#include <OVR.h>

int main()
{
  ovr_Initialize();
  ovrHmd hmd = ovrHmd_Create(0);
  if (!hmd)
  {
    cerr << "There was an error creating the HMD handle - is the rift plugged in?" << endl;
    return 100;
  }

  // Get more details
  cout << "Res X: " << hmd->Resolution.w << " - Res Y: " << hmd->Resolution.h << endl;
  
  return 0;
}
