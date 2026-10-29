/*
**	Filename : Test_DataReader.cpp
**	2026-10-20
**	username : rsehgal
*/
#include "DataReader.h"
#include <iostream>
#include "globals.h"
#include "SiPM.h"
#include <random>
int main(int argc, char *argv[])
{
  SiPM v(1, 22292, 6, 6);
  DataReader *r                 = new DataReader(argv[1], "ftree");
  std::vector<Photon> photonVec = r->GetPhotonsVec(10);
  sort(photonVec.begin(), photonVec.end(), compareByArrivalTime);
  r->Print();

  //  v.SetSeed(12345);

  {
    double icWithoutCrosstalk           = 0.;
    double icWithCrosstalk              = 0.;
    double icWithCrosstalkAndAfterpulse = 0.;

    unsigned int n = 1000;
    for (unsigned int i = 0; i < n; i++) {
      // std::cout << "--------------------------------------" << std::endl;
      unsigned int seed = std::random_device{}();
      // std::cout << "seed = " << seed << std::endl;
      v.SetSeed(seed);

      v.Reset();
      double ic = v.GetIntegratedCharge(photonVec, false);
      icWithoutCrosstalk += ic;
      // std::cout << "Integrated Charge Without Crosstalk : " << ic << std::endl;

      v.Reset();
      ic = v.GetIntegratedCharge(photonVec, true);
      icWithCrosstalk += ic;
      // std::cout << "Integrated Charge With Crosstalk : " << ic << std::endl;

      v.Reset();
      ic = v.GetIntegratedCharge_CrossTalkAfterpulse(photonVec);
      icWithCrosstalkAndAfterpulse += ic;
      // std::cout << "Integrated Charge With Crosstalk and Afterpulses : "
      //         << ic << std::endl;
    }
    icWithoutCrosstalk /= n;
    icWithCrosstalk /= n;
    icWithCrosstalkAndAfterpulse /= n;

    std::cout << "Average Integrated Charge Without Crosstalk : " << icWithoutCrosstalk << std::endl;
    std::cout << "Average Integrated Charge With Crosstalk : " << icWithCrosstalk << std::endl;
    std::cout << "Average Integrated Charge With Crosstalk and Afterpulse: " << icWithCrosstalkAndAfterpulse
              << std::endl;
  }

  {
    // USING ENUM
    std::cout << "======== USING ENUM ==========" << std::endl;
    double icWithoutCrosstalk                    = 0.;
    double icWithCrosstalk                       = 0.;
    double icWithCrosstalkAndAfterpulse          = 0.;
    double icWithCrosstalkAfterpulseAndDarkCount = 0.;

    unsigned int n = 1000;
    for (unsigned int i = 0; i < n; i++) {
      // std::cout << "--------------------------------------" << std::endl;
      unsigned int seed = std::random_device{}();
      // std::cout << "seed = " << seed << std::endl;
      v.SetSeed(seed);

      v.Reset();
      double ic = v.GetIntegratedCharge(photonVec, ChargeMode::NoCrosstalk);
      icWithoutCrosstalk += ic;
      // std::cout << "Integrated Charge Without Crosstalk : " << ic << std::endl;

      v.Reset();
      ic = v.GetIntegratedCharge(photonVec, ChargeMode::CrosstalkOnly);
      icWithCrosstalk += ic;
      // std::cout << "Integrated Charge With Crosstalk : " << ic << std::endl;

      v.Reset();
      ic = v.GetIntegratedCharge(photonVec, ChargeMode::CrosstalkAndAfterpulse);
      icWithCrosstalkAndAfterpulse += ic;

      v.Reset();
      ic = v.GetIntegratedCharge(photonVec, ChargeMode::CrosstalkAfterpulseDarkCount);
      icWithCrosstalkAfterpulseAndDarkCount += ic;

      // std::cout << "Integrated Charge With Crosstalk and Afterpulses : "
      //         << ic << std::endl;
    }
    icWithoutCrosstalk /= n;
    icWithCrosstalk /= n;
    icWithCrosstalkAndAfterpulse /= n;
    icWithCrosstalkAfterpulseAndDarkCount /= n;

    std::cout << "Average Integrated Charge Without Crosstalk : " << icWithoutCrosstalk << std::endl;
    std::cout << "Average Integrated Charge With Crosstalk : " << icWithCrosstalk << std::endl;
    std::cout << "Average Integrated Charge With Crosstalk and Afterpulse: " << icWithCrosstalkAndAfterpulse
              << std::endl;
    std::cout << "Average Integrated Charge With Crosstalk Afterpulse and DarkCurrent : "
              << icWithCrosstalkAfterpulseAndDarkCount << std::endl;
  }

  {
    std::cout << "========== TEsting it with blank photon vector ===========" << std::endl;
    double ic      = 0;
    unsigned int n = 50000;
    photonVec.clear();
    for (unsigned int i = 0; i < n; i++) {
      v.Reset();
      ic += v.GetIntegratedCharge(photonVec, ChargeMode::CrosstalkAfterpulseDarkCount);
    }
    ic /= n;
    std::cout << "Average Integrated charge without any primary photon : " << ic << std::endl;
  }
  return 0;
}
