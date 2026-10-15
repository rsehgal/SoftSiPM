/*
**	Filename : Microcell.h
**	2026-10-15
**	username : rsehgal
*/
#ifndef Microcell_h
#define Microcell_h

#include <iostream>
#include <cmath>
constexpr double kTauRecharge = 50.0;   // ns
constexpr double kQfullFC     = 1009.0; // fC

class Microcell {
  unsigned short cellId = 0;
  bool fired = false;
  double tLast = 0.;
  double gainFactor = 1.;
  unsigned short xbin =10000;
  unsigned short ybin=10000;

public:
  Microcell(unsigned short cid=0) : cellId(cid), fired(false), tLast(0.), gainFactor(1.) {}
  Microcell(unsigned short cid , unsigned short nMicrocells) : Microcell(cid)
  {
    unsigned short nCellInEachDim = std::sqrt(nMicrocells);
    ybin                          = nMicrocells / nCellInEachDim;
    xbin                          = nMicrocells % nCellInEachDim;
  }

  void CalculateXbin() {}
  void CalculateYbin() {}

  ~Microcell() {}

  void Reset() { fired = false; }

  double GainFactor() const { return gainFactor; }

  void CalculateGainFactor(double t)
  {
    if (!fired) {
      gainFactor = 1.0;
      fired      = true;
      //std::cout << "NOt fired ..." << std::endl;
    } else {
      //std::cout << "Fired......" << std::endl;
      gainFactor = 1.0 - std::exp(-(t - tLast) / kTauRecharge);
      if (gainFactor < 0.0) gainFactor = 0.0; // photons fed out of time order
    }
    tLast = t;
  }

  double IntegratedCharge(double t)
  { // charge of THIS avalanche, in fC
    CalculateGainFactor(t);
    return gainFactor * kQfullFC;
  }

  void Print(){
    std::cout << cellId << " : " << xbin <<" : " << ybin <<  " :: " << gainFactor <<std::endl;
  }
  unsigned short GetID() const { return cellId; }
};
#endif
