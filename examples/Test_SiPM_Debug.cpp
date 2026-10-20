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
  SiPM u(1, 22292, 6, 6);
  u.Reset();

  {
    std::vector<Photon> photonVec = GetRandomPhotonVec();
    sort(photonVec.begin(), photonVec.end(), compareByArrivalTime);

    /*    std::cout << "--------------- Photon Vec -----------------------"
                  << std::endl;
        unsigned short phInd = 0;
        for (auto ph : photonVec) {
          std::cout << "Photon Index : " << phInd << " : ";
          ph.Print();
          std::cout << "Integrated Charge : "
                    << u.GetMicrocell(ph).IntegratedCharge(ph.arrivalTime)
                    << std::endl;
          phInd++;
        }
    */
  }

  {
    std::vector<Photon> photonVec = GetRandomPhotonVec();
    sort(photonVec.begin(), photonVec.end(), compareByArrivalTime);
    std::cout << "--------------------------------------" << std::endl;
    unsigned short phInd = 0;
    for (auto ph : photonVec) {
      // u.GetMicrocell(ph).IntegratedCharge(ph.arrivalTime);

      std::cout << "========== Photon Index : " << phInd
                << " ==========" << std::endl;
      std::vector<CrosstalkHit_V2> crosstalkMicrocellVec = u.GetCrossTalk_V2(ph);

      if (crosstalkMicrocellVec.size() > 1) {
        std::cout << "CrossTalk Found...." << std::endl;
      }
      for (auto& ctMc : crosstalkMicrocellVec) {
        //u.GetIntegratedCharge(ctMc.cell, ctMc.arrivalTime);
        Microcell& mc=u.GetMicrocell(ctMc.xbin,ctMc.ybin);
        u.GetIntegratedCharge(mc, ctMc.arrivalTime);
        //ctMc.cell.Print();
        mc.Print();
      }

      phInd++;
    }
  }

  return 0;
}
