/*
**	Filename : Test_DataReader.cpp
**	2026-10-20
**	username : rsehgal
*/
#include "DataReader.h"
#include <iostream>
#include "globals.h"
#include "SiPM.h"

int main(int argc, char *argv[]) {
  SiPM v(1, 22292, 6, 6); 
  DataReader *r = new DataReader("../SimData/out_1K.root", "ftree");
  std::vector<Photon> photonVec = r->GetPhotonsVec(10);
  sort(photonVec.begin(), photonVec.end(), compareByArrivalTime); 
  r->Print();

  v.Reset();
  std::cout << "Integrated Charge Without Crosstalk : "<< v.GetIntegratedCharge(photonVec, false) << std::endl; 

  v.Reset();
  std::cout << "Integrated Charge With Crosstalk : "<< v.GetIntegratedCharge(photonVec, true) << std::endl; 
 
  v.Reset();
  std::cout << "Integrated Charge With Crosstalk and Afterpulses : "<< v.GetIntegratedCharge_CrossTalkAfterpulse(photonVec) << std::endl; 
 
  return 0;
}
