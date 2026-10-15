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
  unsigned short cellId;
  bool fired;
  double tLast;
  double gainFactor;

public:
  Microcell(unsigned short cid = 0) : cellId(cid)
  {
    bool fired        = false;
    double tLast      = 0.0;
    double gainFactor = 1.0;
  }

  ~Microcell() {}

  void Reset() { fired = false; }

  double GainFactor() const { return gainFactor; }

  void CalculateGainFactor(double t)
  {
    if (!fired) {
      gainFactor = 1.0;
      fired      = true;
    } else {
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
};
#endif
