#include "globals.h"

bool compareByArrivalTime(const Photon &a, const Photon &b)
{
  return a.arrivalTime < b.arrivalTime;
}

std::vector<Photon> GetRandomPhotonVec()
{
  std::vector<Photon> photonVec;

  double xpos = getRandomNumber(-3.,3.);
  double ypos = getRandomNumber(-3.,3.);
  for (unsigned int i = 0; i < 10; i++) {
    //photonVec.push_back(Photon{getRandomNumber(-3., 3.), getRandomNumber(-3., 3.), getRandomNumber(0., 250.)});
    photonVec.push_back(Photon{xpos,ypos, getRandomNumber(0., 250.)});
  }

  return photonVec;
}
