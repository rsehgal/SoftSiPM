/*
**	Filename : Test_FullSimData.cpp
**	2026-10-26
**	username : rsehgal
*/
#include <iostream>
#include "DataReader.h"
#include "globals.h"
#include "SiPM.h"
#include <random>
#include <TH1D.h>
#include <TH2D.h>
#include <TApplication.h>
#include <TCanvas.h>

int main(int argc, char *argv[])
{

  TApplication *fApp = new TApplication("fApp",NULL,NULL);

  DataReader r("../SimData/out_1K.root", "ftree");
  SiPM v(1, 22292, 6, 6);
  v.SetSeed(std::random_device{}()); // seed ONCE, outside the loop

  TH1D hQ("hQ", "Charge;Q (fC);events", 200, 0, 400000);
  TH2D hQE("hQE", "Charge vs EDep;EDep (MeV);Q (fC)", 100, 0, 12, 200, 0, 400000);

  Long64_t nEnt = r.ftree->GetEntries();
  for (Long64_t e = 0; e < nEnt; e++) {
    //auto photons = r.GetPhotonsVecSorted(e); // sorted by time; also loads eDep
    v.Reset();                               // fully charged cells for every event
    std::vector<Photon> photonVec = r.GetPhotonsVec(e);
    double Q = v.GetIntegratedCharge(photonVec, ChargeMode::CrosstalkAfterpulseDarkCount);
    hQ.Fill(Q);
    hQE.Fill(r.eDep, Q);
  }

  new TCanvas();
  hQ.Draw();

  new TCanvas();
  hQE.Draw();

 fApp->Run();

 return 0;
}
