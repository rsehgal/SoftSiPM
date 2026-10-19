/*
**	Filename : Test_SiPM.cpp
**	2026-10-15
**	username : rsehgal
*/
#include <iostream>
#include "SiPM.h"
#include "Microcell.h"
#include "globals.h"
#include <algorithm>
int main(int argc, char *argv[])
{
  SiPM s(0, 100);
  std::cout << "Numbe of Microcells : " << s.size() << std::endl;

  SiPM t(1, 25, 10, 10);
  std::cout << "Microcell Id of coordinate (0,0) : " << t.MicrocellId(3., 2.) << std::endl;

  SiPM u(1, 22292, 6, 6);
  double xc = 1.3;
  double yc = -1.3;
  std::cout << "Microcell Id of coordinate (1.3,-1.3) : " << u.MicrocellId(xc, yc) << std::endl;
  xc = 2.95;
  yc = 2.99;
  std::cout << "Microcell Id of coordinate (2.95,2.99) : " << u.MicrocellId(xc, yc) << std::endl;
  u.GetMicrocell(xc, yc).Print();

  xc = 2.999;
  yc = 2.999;
  std::cout << "Microcell Id of coordinate (2.999,2.999) : " << u.MicrocellId(xc, yc) << std::endl;

  u.GetMicrocell(xc, yc).Print();

  std::cout << "Integrated charge : " << u.GetMicrocell(xc, yc).IntegratedCharge(0) << std::endl;
  // u.GetMicrocell(xc,yc).Print();
  std::cout << "Integrated charge : " << u.GetMicrocell(xc, yc).IntegratedCharge(10) << std::endl;
  std::cout << "Integrated charge : " << u.GetMicrocell(xc, yc).IntegratedCharge(20) << std::endl;
  std::cout << "Integrated charge : " << u.GetMicrocell(xc, yc).IntegratedCharge(30) << std::endl;
  std::cout << "Integrated charge : " << u.GetMicrocell(xc, yc).IntegratedCharge(50) << std::endl;

  std::cout << "----------------- Testing with Photon variable ---------------------" << std::endl;

  std::cout << "--------------- Resetting full SiPM ------------------" << std::endl;
  u.Reset();
  {
    Photon ph(xc, yc, 60);
    std::cout << "Integrated charge of a Photon : " << u.GetIntegratedCharge(ph) << std::endl;
  }
  {
    Photon ph(1.3, 2.5, 60);
    std::cout << "Integrated charge of a Photon : " << u.GetIntegratedCharge(ph) << std::endl;
  }
  std::cout << "--------------- Resetting full SiPM ------------------" << std::endl;
  u.Reset();
  {
    std::vector<Photon> photonVec = GetRandomPhotonVec();
    sort(photonVec.begin(), photonVec.end(), compareByArrivalTime);
    u.GetIntegratedCharge(photonVec);

    // Checking for Crosstalk
    unsigned short phCount = 0;
    for (auto const &ph : photonVec) {
      // std::vector<Microcell> crosstalkMicrocellVec = u.GetCrossTalk(u.GetMicrocell(ph));
      std::vector<Microcell> crosstalkMicrocellVec = u.GetCrossTalk(ph);
      if (crosstalkMicrocellVec.size() > 1) {
        std::cout << "Photon Index : " << phCount << " :: Crosstalk found....." << std::endl;

        for (auto& ctMC : crosstalkMicrocellVec) {
          ctMC.Print();
        }
      }
      phCount++;
    }
  }
std::cout << "--------------- Resetting full SiPM ------------------" << std::endl;
  u.Reset();
  {
    std::vector<Photon> photonVec = GetRandomPhotonVec();
    sort(photonVec.begin(), photonVec.end(), compareByArrivalTime);
    u.GetIntegratedCharge(photonVec);

    // Checking for Crosstalk
    unsigned short phCount = 0;
    for (auto const &ph : photonVec) {
      // std::vector<Microcell> crosstalkMicrocellVec = u.GetCrossTalk(u.GetMicrocell(ph));
      std::vector<CrosstalkHit> crosstalkMicrocellVec = u.GetCrossTalk_V2(ph);
      if (crosstalkMicrocellVec.size() > 1) {
        std::cout << "Photon Index : " << phCount << " :: Crosstalk found....." << std::endl;

        for (auto& ctMC : crosstalkMicrocellVec) {
          ctMC.cell.Print();
        }
      }
      phCount++;
    }
  }

  return 0;
}
