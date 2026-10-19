/*
**	Filename : SiPM.h
**	2026-10-15
**	username : rsehgal
*/
#ifndef SiPM_h
#define SiPM_h

#include "Microcell.h"
#include <vector>
#include "globals.h"
#include <random>

struct CrosstalkHit {
  Microcell cell;
  double arrivalTime;
};


class SiPM {
  std::vector<Microcell> vecOfMicrocells;
  unsigned short sipmId;
  unsigned short numOfMicrocells;
  double xlength;
  double ylength;

  //SipM related Parameters
  double ctProb;
  std::mt19937 rng; 
  //mutable std::mt19937 rng{std::random_device{}()}; 

public:
  SiPM();
  SiPM(unsigned short sid, unsigned short nMicrocells, double xlen = 6, double ylen = 6);
  ~SiPM();
  unsigned short size() const;
  unsigned short GetID() const;

  void Reset();
  unsigned short MicrocellId(double x, double y);
  Microcell &GetMicrocell(Photon ph);
  Microcell &GetMicrocell(double x, double y);
  Microcell &GetMicrocell(unsigned short xbin, unsigned short  ybin);

  double GetIntegratedCharge(Microcell& mc, double arrivalTime);
  //double GetIntegratedCharge_V1(Photon ph);
  //double GetIntegratedCharge_V2(Photon ph);
  double GetIntegratedCharge(Photon ph);
  double GetIntegratedCharge(std::vector<Photon> photonVec);

  unsigned short GetCellIDFromBins(unsigned short xbin, unsigned short ybin);

  std::vector<CrosstalkHit> GetCrossTalk(Microcell primary, double arrivalTime);//{}
  std::vector<Microcell> GetCrossTalk(Microcell primary);//{}
  std::vector<Microcell> GetCrossTalk(Photon ph);//{}
  std::vector<CrosstalkHit> GetCrossTalk_V2(Photon ph);//{}
};

#endif
