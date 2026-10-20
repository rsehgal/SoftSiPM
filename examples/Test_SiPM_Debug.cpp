/*
**	Filename : Test_SiPM.cpp
**	2026-10-15
**	username : rsehgal
*/
#include "Microcell.h"
#include "SiPM.h"
#include "globals.h"
#include <algorithm>
#include <iostream>
int main(int argc, char *argv[]) {
  {
    SiPM u(1, 22292, 6, 6);
    std::vector<Photon> photonVec = GetRandomPhotonVec();
    sort(photonVec.begin(), photonVec.end(), compareByArrivalTime);
    std::cout << "--------------------------------------" << std::endl;
    unsigned short phInd = 0;
    for (auto ph : photonVec) {
      // u.GetMicrocell(ph).IntegratedCharge(ph.arrivalTime);

      std::cout << "========== Photon Index : " << phInd
                << " ==========" << std::endl;
      std::vector<CrosstalkHit> crosstalkMicrocellVec = u.GetCrossTalk(ph);

      if (crosstalkMicrocellVec.size() > 1) {
        std::cout << "CrossTalk Found...." << std::endl;
      }
      for (auto &ctMc : crosstalkMicrocellVec) {
        // u.GetIntegratedCharge(ctMc.cell, ctMc.arrivalTime);
        Microcell &mc = u.GetMicrocell(ctMc.xbin, ctMc.ybin);
        u.GetIntegratedCharge(mc, ctMc.arrivalTime);
        // ctMc.cell.Print();
        mc.Print();
      }

      phInd++;
    }

    unsigned short numOfFiredCells = 0;
    for (auto mc : u.GetVecOfMicrocells()) {
      numOfFiredCells += 1 ? mc.Fired() : 0;
    }

    std::cout << "Number of Fired cells : " << numOfFiredCells << std::endl;
  }

  {

    std::cout << "============= TESTING OVERALL INTEGRATED CHARGE WITH and "
                 "WITHOUT CROSSTALK ==============="
              << std::endl;
    SiPM v(1, 22292, 6, 6);
   
    std::vector<Photon> photonVec = GetRandomPhotonVec();

    v.Reset();
    sort(photonVec.begin(), photonVec.end(), compareByArrivalTime);
    std::cout << "Integrated Charge Without Crosstalk : "
              << v.GetIntegratedCharge(photonVec, false) << std::endl;

    v.Reset();
    std::cout << "Integrated Charge With Crosstalk : "
              << v.GetIntegratedCharge(photonVec, true) << std::endl;
  }
  return 0;
}
