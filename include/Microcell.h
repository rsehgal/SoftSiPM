/*
**	Filename : Microcell.h
**	2026-10-15
**	username : rsehgal
*/
#ifndef Microcell_h
#define Microcell_h

#include <iostream>
#include <cmath>
#include "globals.h"
constexpr double kTauRecharge = 50.0;   // ns
constexpr double kQfullFC     = 1009.0; // fC

class Microcell {
  unsigned short cellId = 0;
  bool fired            = false;
  double tLast          = 0.;
  double gainFactor     = 1.;
  unsigned short xbin   = 10000;
  unsigned short ybin   = 10000;
  unsigned short count  = 0;

public:
  Microcell(unsigned short cid = 0) : cellId(cid), fired(false), tLast(0.), gainFactor(1.), count(0)
  {
    xbin = GetXBinFromCellID();
    ybin = GetYBinFromCellID();
  }

  Microcell(unsigned short cid, unsigned short nMicrocells) : Microcell(cid)
  {
    /*unsigned short nCellInEachDim = std::sqrt(nMicrocells);
    // ybin                          = nMicrocells / nCellInEachDim;
    // xbin                          = nMicrocells % nCellInEachDim;
    ybin = cellId / nCellInEachDim;
    xbin = cellId % nCellInEachDim;*/
  }

  bool operator<(const Microcell &other) const { return cellId < other.cellId; }

  void CalculateXbin() {}
  void CalculateYbin() {}

  unsigned short GetXBinFromCellID() { return cellId % numberOfMicrocellsY; }
  unsigned short GetYBinFromCellID() { return cellId / numberOfMicrocellsX; }

  unsigned short GetCellIDFromBins() { return ybin * numberOfMicrocellsX + xbin; }

  unsigned short GetXBin() const { return xbin; }
  unsigned short GetYBin() const { return ybin; }

  ~Microcell() {}

  void Reset()
  {
    fired = false;
    count = 0;
  }

  double GainFactor() const { return gainFactor; }

  void CalculateGainFactor(double t)
  {
    count++;
    //std::cout << "Fired : " << fired << std::endl;
    if (!fired) {
      gainFactor = 1.0;
      fired      = true;
      // std::cout << "NOt fired ..." << std::endl;
    } else {
      // std::cout << "Fired......" << std::endl;
      gainFactor = 1.0 - std::exp(-(t - tLast) / kTauRecharge);
      if (gainFactor < 0.0) gainFactor = 0.0; // photons fed out of time order
    }
    tLast = t;
  }

  double IntegratedCharge(double t)
  { // charge of THIS avalanche, in fC
    //std::cout <<"Arrival time from Integrated charge of Microcell : " << t << std::endl;
    CalculateGainFactor(t);
    //std::cout << "Calculated Gain factor : " << gainFactor << std::endl;
    return gainFactor * kQfullFC;
  }

  bool Fired() {
    return fired;
  }
  void Print() { std::cout << cellId << " : " << xbin << " : " << ybin << " :: " << gainFactor << std::endl; }
  unsigned short GetID() const { return cellId; }
};
#endif
