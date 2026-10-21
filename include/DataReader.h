/*
**	Filename : DataReader.h
**	2026-10-20
**	username : rsehgal
*/
#ifndef DataReader_h
#define DataReader_h
#include "globals.h"
#include <TTree.h>
#include <TFile.h>

class DataReader {
  TFile *fp = nullptr;

public:
  // Declaration of leaves types
  Double_t eDep;
  Double_t numOfScintillationPhotons;
  Double_t numOfWLSPhotons;
  Double_t numOfReachedWLSPhotons;
  Double_t numOfAbsorbedWLSPhotons;
  Double_t numOfDetectedWLSPhotons;
  std::vector<double> *posXVec = nullptr;
  std::vector<double> *posYVec = nullptr;
  std::vector<double> *arrivalTimeVec = nullptr;

  TTree *ftree = nullptr;
  DataReader() {}

  DataReader(char *filename, char *treename) {
    // Set branch addresses.

    fp = new TFile(filename, "r");
    ftree = static_cast<TTree*>(fp->Get(treename));
    ftree->SetBranchAddress("eDep", &eDep);
    ftree->SetBranchAddress("numOfScintillationPhotons",
                            &numOfScintillationPhotons);
    ftree->SetBranchAddress("numOfWLSPhotons", &numOfWLSPhotons);
    ftree->SetBranchAddress("numOfReachedWLSPhotons", &numOfReachedWLSPhotons);
    ftree->SetBranchAddress("numOfAbsorbedWLSPhotons",
                            &numOfAbsorbedWLSPhotons);
    ftree->SetBranchAddress("numOfDetectedWLSPhotons",
                            &numOfDetectedWLSPhotons);
    ftree->SetBranchAddress("posXVec", &posXVec);
    ftree->SetBranchAddress("posYVec", &posYVec);
    ftree->SetBranchAddress("arrivalTimeVec", &arrivalTimeVec);
  }

  std::vector<Photon> GetPhotonsVec(Long64_t ent) {
    std::vector<Photon> simPhotonsVec;

    ftree->GetEntry(ent);
    for (unsigned int j = 0; j < posXVec->size(); j++) {
      simPhotonsVec.push_back(
          Photon{posXVec->at(j), posYVec->at(j), arrivalTimeVec->at(j)});
    }
    return simPhotonsVec;
  }
std::vector<Photon> GetPhotonsVecSorted(Long64_t ent) {
    std::vector<Photon> photonsVec = GetPhotonsVec(ent);
    sort(photonsVec.begin(), photonsVec.end(), compareByArrivalTime);
    return photonsVec;
  }

  void Print(){
  std::cout << "EDep : " << eDep << " : numOfDetectedWLSPhotons : " << numOfDetectedWLSPhotons << std::endl;
}
};

#endif
