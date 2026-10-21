#include "globals.h"
#include <cmath>

bool verbose = false;

unsigned short totalNumberOfMicrocells = 22292;
unsigned short numberOfMicrocellsX     = std::sqrt(totalNumberOfMicrocells);
unsigned short numberOfMicrocellsY     = std::sqrt(totalNumberOfMicrocells);

double q = 0.0694;
double tauAfterpulse = 20.;
double pAfterpulse = 0.05;

bool compareByArrivalTime(const Photon &a, const Photon &b)
{
  return a.arrivalTime < b.arrivalTime;
}
bool compareHitByArrivalTime(const CrosstalkHit& a, CrosstalkHit &b){
  return a.arrivalTime < b.arrivalTime;
}
std::vector<Photon> GetRandomPhotonVec()
{
  std::vector<Photon> photonVec;

  double xpos = getRandomNumber(-3. + 0.001, 3. - 0.001);
  double ypos = getRandomNumber(-3. + 0.001, 3. - 0.001);
  for (unsigned int i = 0; i < 10; i++) {
    // photonVec.push_back(Photon{getRandomNumber(-3., 3.), getRandomNumber(-3., 3.), getRandomNumber(0., 250.)});
    photonVec.push_back(Photon{xpos, ypos, getRandomNumber(0., 250.)});
  }

  return photonVec;
}
