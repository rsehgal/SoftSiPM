/*
**	Filename : SiPM.h
**	2026-10-15
**	username : rsehgal
*/
#ifndef SiPM_h
#define SiPM_h

#include "Microcell.h"
#include <vector>

struct Photon {
  double xposition;
  double yposition;
  double arrivalTime;

  Photon(double xpos, double ypos, double arrTime)
  {
    xposition   = xpos;
    yposition   = ypos;
    arrivalTime = arrTime;
  }
};

class SiPM {
  std::vector<Microcell> vecOfMicrocells;
  unsigned short sipmId;
  unsigned short numOfMicrocells;
  double xlength;
  double ylength;

public:
  SiPM();
  SiPM(unsigned short sid, unsigned short nMicrocells, double xlen = 6, double ylen = 6);
  ~SiPM();
  unsigned short size() const;
  unsigned short GetID() const;

  void Reset();
  unsigned short MicrocellId(double x, double y);
  Microcell &GetMicrocell(double x, double y);

  double GetIntegratedCharge(Photon ph);
  double GetIntegratedCharge(std::vector<Photon> photonVec);
};

#endif
