/*
**	Filename : Test_Microcell.cpp
**	2026-10-15
**	username : rsehgal
*/
#include "Microcell.h"
#include<iostream>
int main() {
  Microcell m;
  const double times[3] = {2, 10, 20};
  double tot = 0;
  for (double t : times) {
    double q = m.IntegratedCharge(t);
    tot += q;
    std::cout << "t = " << t << " ns   a = " << m.GainFactor() << "   Q = " << q << " fC\n";
  }
  std::cout << "total = " << tot << " fC\n";
  m.Reset();                                    // new event
  std::cout << "after Reset(), first hit: Q = " << m.IntegratedCharge(5) << " fC\n";
}
