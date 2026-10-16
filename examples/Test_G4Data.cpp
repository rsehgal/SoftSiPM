#include <TTree.h>
#include <TH2F.h>
#include <TFile.h>
#include <TApplication.h>
#include <iostream>
#include <vector>
#include "globals.h"
#include "SiPM.h"
int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp", NULL, NULL);
  TFile *fp          = new TFile(argv[1], "r");
  TTree *ftree       = (TTree *)fp->Get("ftree");

  // Declaration of leaves types
  Double_t eDep;
  Double_t numOfScintillationPhotons;
  Double_t numOfWLSPhotons;
  Double_t numOfReachedWLSPhotons;
  Double_t numOfAbsorbedWLSPhotons;
  Double_t numOfDetectedWLSPhotons;
  std::vector<double> *posXVec        = nullptr;
  std::vector<double> *posYVec        = nullptr;
  std::vector<double> *arrivalTimeVec = nullptr;

  // Set branch addresses.
  ftree->SetBranchAddress("eDep", &eDep);
  ftree->SetBranchAddress("numOfScintillationPhotons", &numOfScintillationPhotons);
  ftree->SetBranchAddress("numOfWLSPhotons", &numOfWLSPhotons);
  ftree->SetBranchAddress("numOfReachedWLSPhotons", &numOfReachedWLSPhotons);
  ftree->SetBranchAddress("numOfAbsorbedWLSPhotons", &numOfAbsorbedWLSPhotons);
  ftree->SetBranchAddress("numOfDetectedWLSPhotons", &numOfDetectedWLSPhotons);
  ftree->SetBranchAddress("posXVec", &posXVec);
  ftree->SetBranchAddress("posYVec", &posYVec);
  ftree->SetBranchAddress("arrivalTimeVec", &arrivalTimeVec);

  Long64_t nentries = ftree->GetEntries();

  Long64_t nbytes = 0;

  TH2F *posHist = new TH2F("posHist", "posHist", 1000, -3., 3., 1000, -3., 3.);

  std::vector<Photon> simPhotonsVec;

  for (Long64_t i = 0; i < nentries; i++) {
    nbytes += ftree->GetEntry(i);
    for (unsigned int j = 0; j < posXVec->size(); j++) {
      posHist->Fill(posXVec->at(j), posYVec->at(j));
      // std::cout << posXVec->at(j) << " : " << posYVec->at(j) << " :: ArrivalTime :  " << arrivalTimeVec->at(j)
      //         << std::endl;
      simPhotonsVec.push_back(Photon{posXVec->at(j), posYVec->at(j), arrivalTimeVec->at(j)});
    }
  }

  SiPM u(1, 22292, 6, 6);
  sort(simPhotonsVec.begin(), simPhotonsVec.end(), compareByArrivalTime);

  double totalCharge = u.GetIntegratedCharge(simPhotonsVec);
  std::cout << "--------------------------------------" << std::endl;
  std::cout << "Total Charge in an event : " << totalCharge << std::endl;
  std::cout << "--------------------------------------" << std::endl;

  posHist->Draw();

  fApp->Run();

  return 0;
}
