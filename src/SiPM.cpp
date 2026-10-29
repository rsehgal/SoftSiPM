/*
**	Filename : SiPM.cpp
**	2026-10-15
**	username : rsehgal
*/
#include "SiPM.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <utility>
#include <vector>

SiPM::SiPM() {}

SiPM::SiPM(unsigned short sid, unsigned short nMicrocells, double xlen, double ylen)
    : sipmId(sid), numOfMicrocells(nMicrocells), xlength(xlen), ylength(ylen)
{

  for (unsigned short cid = 0; cid < numOfMicrocells; cid++) {
    vecOfMicrocells.push_back(Microcell{cid, numOfMicrocells});
  }
  // std::fill(vecOfMicrocells.begin(), vecOfMicrocells.end(), Microcell{});
}

void SiPM::SetSeed(unsigned int svalue)
{
  rng.seed(svalue);
}

unsigned short SiPM::size() const
{
  return vecOfMicrocells.size();
}

unsigned short SiPM::GetID() const
{
  return sipmId;
}

void SiPM::Reset()
{
  for (auto &micCell : vecOfMicrocells) {
    micCell.Reset();
  }
}

std::vector<Microcell> SiPM::GetVecOfMicrocells() const
{
  return vecOfMicrocells;
}

unsigned short SiPM::MicrocellId(double x, double y)
{
  short numOfMicrocellInX    = std::sqrt(numOfMicrocells);
  short numOfMicrocellInY    = std::sqrt(numOfMicrocells);
  double microcellXPitch     = xlength / numOfMicrocellInX;
  double microcellYPitch     = ylength / numOfMicrocellInY;
  short xIndex               = (x + xlength / 2) / microcellXPitch;
  short yIndex               = (y + xlength / 2) / microcellYPitch;
  unsigned short microcellId = (yIndex * numOfMicrocellInX) + xIndex;
  return microcellId;
}

Microcell &SiPM::GetMicrocell(Photon ph)
{
  return GetMicrocell(ph.xposition, ph.yposition);
}

Microcell &SiPM::GetMicrocell(double x, double y)
{
  return vecOfMicrocells[MicrocellId(x, y)];
}

Microcell &SiPM::GetMicrocell(unsigned short xbin, unsigned short ybin)
{
  return vecOfMicrocells[GetCellIDFromBins(xbin, ybin)];
}

double SiPM::GetIntegratedCharge(Microcell &mc, double arrivalTime)
{
  double integCharge = mc.IntegratedCharge(arrivalTime);
  if (verbose) std::cout << "Integrated charge of photon : " << integCharge << std::endl;
  return integCharge;
}

double SiPM::GetIntegratedCharge(unsigned short xbin, unsigned short ybin, double arrivalTime)
{
  if (xbin >= 0 && ybin >= 0 && xbin < numberOfMicrocellsX && ybin < numberOfMicrocellsY) {
    Microcell &mc = GetMicrocell(xbin, ybin);
    return GetIntegratedCharge(mc, arrivalTime);
  } else
    return 0;
}

double SiPM::GetIntegratedCharge(Photon ph, bool crosstalk)
{
  double integratedCharge = 0.;
  if (!crosstalk)
    integratedCharge = GetIntegratedCharge(GetMicrocell(ph), ph.arrivalTime);
  else {
    std::vector<CrosstalkHit> crosstalkMicrocellVec = GetCrossTalk(ph);
    for (const auto &ctMc : crosstalkMicrocellVec) {
      // Microcell &mc = GetMicrocell(ctMc.xbin, ctMc.ybin);
      // integratedCharge += GetIntegratedCharge(mc, ctMc.arrivalTime);
      integratedCharge += GetIntegratedCharge(ctMc.xbin, ctMc.ybin, ctMc.arrivalTime);
    }
  }
  return integratedCharge;
}

double SiPM::GetIntegratedCharge(std::vector<Photon> photonVec, bool crosstalk)
{
  double totalCharge = 0;
  for (auto ph : photonVec) {
    totalCharge += GetIntegratedCharge(ph, crosstalk);
  }
  return totalCharge;
}

double SiPM::GetIntegratedCharge(std::vector<Photon> photonVec, ChargeMode mode)
{
  switch (mode) {
  case ChargeMode::NoCrosstalk:
    return GetIntegratedCharge(photonVec, false); // Calls the bool version for single photon/no crosstalk

  case ChargeMode::CrosstalkOnly:
    return GetIntegratedCharge(photonVec, true); // Calls the bool version with crosstalk = true

  case ChargeMode::CrosstalkAndAfterpulse: {
    std::vector<CrosstalkHit> all = GetCrossTalkAndAfterpulse(photonVec);
    return GetIntegratedCharge(all);
  }

  case ChargeMode::CrosstalkAfterpulseDarkCount: {
    std::vector<CrosstalkHit> all = GetCrossTalkAfterpulseDarkCount(photonVec);
    return GetIntegratedCharge(all);
  }
  }
  return 0.;
}

/*double SiPM::GetIntegratedCharge(std::vector<Photon> photonVec, unsigned short option)
{
  if (option == 0) return GetIntegratedCharge(photonVec, false);
  if (option == 1) return GetIntegratedCharge(photonVec, true);
  if (option == 2) {
    std::vector<CrosstalkHit> all = GetCrossTalkAndAfterpulse(photonVec);
    return GetIntegratedCharge(all);
  }
  if (option == 3) {
    std::vector<CrosstalkHit> all = GetCrossTalkAfterpulseDarkCount(photonVec);
    return GetIntegratedCharge(all);
  }
  return 0.;
}*/
double SiPM::GetIntegratedCharge(const CrosstalkHit &hit)
{
  double integratedCharge = 0.;
  if (hit.isValid()) integratedCharge = GetIntegratedCharge(hit.xbin, hit.ybin, hit.arrivalTime);
  return integratedCharge;
}

double SiPM::GetIntegratedCharge(std::vector<CrosstalkHit> &hits)
{
  sort(hits.begin(), hits.end(), compareHitByArrivalTime);
  double integratedCharge = 0.;
  for (auto &h : hits) {
    // integratedCharge += GetIntegratedCharge(h.xbin, h.ybin, h.arrivalTime);
    integratedCharge += GetIntegratedCharge(h);
  }
  return integratedCharge;
}
SiPM::~SiPM() {}

unsigned short SiPM::GetCellIDFromBins(unsigned short xbin, unsigned short ybin)
{
  return ybin * numberOfMicrocellsX + xbin;
}

std::vector<CrosstalkHit> SiPM::GetCrossTalk(Photon ph)
{
  return GetCrossTalk(GetMicrocell(ph), ph.arrivalTime);
}

void SiPM::AddAfterpulses(std::vector<CrosstalkHit> &hits)
{
  std::uniform_real_distribution<double> uni(0, 1);
  std::exponential_distribution<double> expo(1.0 / tauAfterpulse);
  size_t n = hits.size(); // only the avalanches that exist now
  for (size_t i = 0; i < n; i++)
    if (uni(rng) < pAfterpulse) {
      double t   = hits[i].arrivalTime + expo(rng);
      auto chain = GetCrossTalk(GetMicrocell(hits[i].xbin, hits[i].ybin), t);
      hits.insert(hits.end(), chain.begin(), chain.end());
    }
}
std::vector<CrosstalkHit> SiPM::GetCrossTalk(std::vector<Photon> photonsVec)
{
  std::vector<CrosstalkHit> all;
  for (auto &ph : photonsVec) {
    auto ctHits = GetCrossTalk(ph);
    all.insert(all.end(), ctHits.begin(), ctHits.end());
  }
  return all;
}

std::vector<CrosstalkHit> SiPM::GetCrossTalkAndAfterpulse(std::vector<Photon> photonsVec)
{
  std::vector<CrosstalkHit> all;
  for (auto &ph : photonsVec) {
    auto h = GetCrossTalk(ph);
    AddAfterpulses(h);
    all.insert(all.end(), h.begin(), h.end());
  }
  return all;
}
std::vector<CrosstalkHit> SiPM::GetPrimaryHits(std::vector<Photon> photonsVec)
{
  std::vector<CrosstalkHit> all;
  for (auto &ph : photonsVec) {
    auto mc = GetMicrocell(ph);
    all.push_back({mc.GetXBin(), mc.GetYBin(), ph.arrivalTime});
  }
  return all;
}

std::vector<CrosstalkHit> SiPM::GetHitsVec(std::vector<Photon> photonsVec, ChargeMode mode)
{
  switch (mode) {
  case ChargeMode::NoCrosstalk:
    return GetPrimaryHits(photonsVec); // Calls the bool version for single photon/no crosstalk

  case ChargeMode::CrosstalkOnly:
    return GetCrossTalk(photonsVec); // Calls the bool version with crosstalk = true

  case ChargeMode::CrosstalkAndAfterpulse: {
    return GetCrossTalkAndAfterpulse(photonsVec);
  }

  case ChargeMode::CrosstalkAfterpulseDarkCount: {
    return GetCrossTalkAfterpulseDarkCount(photonsVec);
  }
  }
}

std::vector<Avalanche> SiPM::GetAvalancheVec(std::vector<Photon> photonsVec, ChargeMode mode)
{
  std::vector<CrosstalkHit> hitsVec = GetHitsVec(photonsVec, mode);
  std::vector<Avalanche> avalancheVec;
  for (const auto &hit : hitsVec) {
    avalancheVec.push_back({GetIntegratedCharge(hit), hit.arrivalTime});
  }
  return avalancheVec;
}

std::vector<CrosstalkHit> SiPM::GetDarkCounts(double tStart, double tEnd)
{
  double area = sipmXLength * sipmYLength;
  double mu   = darkCountRate * area * (tEnd - tStart) * 1e-9;
  int n       = std::poisson_distribution<int>(mu)(rng);
  std::uniform_real_distribution<double> ux(-sipmXLength / 2, sipmXLength / 2), uy(-sipmYLength / 2, sipmYLength / 2),
      ut(tStart, tEnd);
  std::vector<CrosstalkHit> all;

  for (unsigned int i = 0; i < n; i++) {
    Photon dark(ux(rng), uy(rng), ut(rng));
    auto chain = GetCrossTalk(dark);
    AddAfterpulses(chain);
    all.insert(all.end(), chain.begin(), chain.end());
  }

  return all;
}

std::vector<CrosstalkHit> SiPM::GetCrossTalkAfterpulseDarkCount(std::vector<Photon> photonsVec)
{
  std::vector<CrosstalkHit> all       = GetCrossTalkAndAfterpulse(photonsVec);
  std::vector<CrosstalkHit> darkCount = GetDarkCounts(0, 250);
  all.insert(all.end(), darkCount.begin(), darkCount.end());
  return all;
}

double SiPM::GetIntegratedCharge_CrossTalkAfterpulse(std::vector<Photon> photonsVec)
{
  /*  std::vector<CrosstalkHit> all;
    for (auto &ph : photonsVec) {
      auto h = GetCrossTalk(ph);
      AddAfterpulses(h);
      all.insert(all.end(), h.begin(), h.end());
    }*/
  std::vector<CrosstalkHit> all = GetCrossTalkAndAfterpulse(photonsVec);
  sort(all.begin(), all.end(), compareHitByArrivalTime);
  return GetIntegratedCharge(all);
}
std::vector<CrosstalkHit> SiPM::GetCrossTalk(Microcell primary, double primaryArrivalTime)
{
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  std::vector<CrosstalkHit> queue = {{primary.GetXBin(), primary.GetYBin(), primaryArrivalTime}};
  std::vector<CrosstalkHit> new_crosstalk_cells;

  std::set<unsigned short> global_fired_cells;

  while (!queue.empty()) {
    CrosstalkHit currItem = queue.front();
    queue.erase(queue.begin());

    double currTime         = currItem.arrivalTime;
    unsigned short currXbin = currItem.xbin;
    unsigned short currYbin = currItem.ybin;

    std::vector<CrosstalkHit> neighbors = {
        {static_cast<unsigned short>(currXbin - 1), static_cast<unsigned short>(currYbin), currTime},
        {static_cast<unsigned short>(currXbin + 1), static_cast<unsigned short>(currYbin), currTime},
        {static_cast<unsigned short>(currXbin), static_cast<unsigned short>(currYbin - 1), currTime},
        {static_cast<unsigned short>(currXbin), static_cast<unsigned short>(currYbin + 1), currTime}};

    for (auto &neighbor : neighbors) {
      // 1. Boundary check
      // if (neighbor.x >= 0 && neighbor.x < grid_size && neighbor.y >= 0 &&
      // neighbor.y < grid_size)
      {

        // 2. Skip check: if not already fired
        unsigned short neighborCellID = neighbor.GetCellID();
        if (global_fired_cells.find(neighborCellID) == global_fired_cells.end()) {

          // 3. Roll for crosstalk
          if (dist(rng) < q) {
            neighbor.arrivalTime += 0.01;
            global_fired_cells.insert(neighborCellID);
            new_crosstalk_cells.push_back(neighbor);
            queue.push_back(neighbor); // Propagate further
          }
        }
      }
    }
  }

  // Intentionally putting the primary microcell at the end of crosstalk vector
  new_crosstalk_cells.push_back({primary.GetXBin(), primary.GetYBin(), primaryArrivalTime});
  return new_crosstalk_cells;
}
