/*
**	Filename : SiPM.h
**	2026-10-15
**	username : rsehgal
*/
#ifndef SiPM_h
#define SiPM_h

#include "Microcell.h"
#include "globals.h"
#include <random>
#include <vector>

/*struct CrosstalkHit {
  Microcell cell;
  double arrivalTime;
};*/

/*struct CrosstalkHit {
  unsigned short xbin;
  unsigned short ybin;
  double arrivalTime;

  unsigned short GetCellID() { return ybin * numberOfMicrocellsX + xbin; }
};*/

class SiPM {
  std::vector<Microcell> vecOfMicrocells;
  unsigned short sipmId;
  unsigned short numOfMicrocells;
  double xlength;
  double ylength;

  // SipM related Parameters
  double ctProb;
  std::mt19937 rng;
  // mutable std::mt19937 rng{std::random_device{}()};

public:
  SiPM();
  SiPM(unsigned short sid, unsigned short nMicrocells, double xlen = 6,
       double ylen = 6);
  ~SiPM();
  unsigned short size() const;
  unsigned short GetID() const;
  void Reset();
  unsigned short MicrocellId(double x, double y);
  Microcell &GetMicrocell(Photon ph);
  Microcell &GetMicrocell(double x, double y);
  Microcell &GetMicrocell(unsigned short xbin, unsigned short ybin);

  double GetIntegratedCharge(Microcell &mc, double arrivalTime);
  double GetIntegratedCharge(unsigned short xbin, unsigned short ybin, double arrivalTime);
  // double GetIntegratedCharge_V1(Photon ph);
  // double GetIntegratedCharge_V2(Photon ph);
  double GetIntegratedCharge(Photon ph, bool crosstalk=true);
  double GetIntegratedCharge(std::vector<Photon> photonVec, bool crosstalk=true);
  //double GetIntegratedCharge(std::vector<Photon> photonVec, unsigned short option);
  double GetIntegratedCharge(std::vector<Photon> photonVec, ChargeMode mode);
  double GetIntegratedCharge(std::vector<CrosstalkHit> &hits);
  double GetIntegratedCharge_CrossTalkAfterpulse(std::vector<Photon> photonsVec);
  double GetIntegratedCharge(CrosstalkHit &hit) ;
  unsigned short GetCellIDFromBins(unsigned short xbin, unsigned short ybin);

  std::vector<CrosstalkHit> GetCrossTalk(Photon ph); //{}
  std::vector<CrosstalkHit> GetCrossTalk(Microcell primary,
                                         double arrivalTime); //{}
  std::vector<CrosstalkHit> GetDarkCounts(double tStart, double tEnd);
  std::vector<CrosstalkHit> GetCrossTalkAndAfterpulse(std::vector<Photon> photonsVec);
  std::vector<CrosstalkHit> GetCrossTalkAfterpulseDarkCount(std::vector<Photon> photonsVec);

  std::vector<Microcell> GetVecOfMicrocells() const;

  void AddAfterpulses(std::vector<CrosstalkHit> &hits);

  void SetSeed(unsigned int svalue);
};

#endif
