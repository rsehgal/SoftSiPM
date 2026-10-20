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

SiPM::SiPM(unsigned short sid, unsigned short nMicrocells, double xlen,
           double ylen)
    : sipmId(sid), numOfMicrocells(nMicrocells), xlength(xlen), ylength(ylen) {

  for (unsigned short cid = 0; cid < numOfMicrocells; cid++) {
    vecOfMicrocells.push_back(Microcell{cid, numOfMicrocells});
  }
  // std::fill(vecOfMicrocells.begin(), vecOfMicrocells.end(), Microcell{});
}

unsigned short SiPM::size() const { return vecOfMicrocells.size(); }

unsigned short SiPM::GetID() const { return sipmId; }

void SiPM::Reset() {
  for (auto &micCell : vecOfMicrocells) {
    micCell.Reset();
  }
}

std::vector<Microcell> SiPM::GetVecOfMicrocells() const {
  return vecOfMicrocells;
}

unsigned short SiPM::MicrocellId(double x, double y) {
  short numOfMicrocellInX = std::sqrt(numOfMicrocells);
  short numOfMicrocellInY = std::sqrt(numOfMicrocells);
  double microcellXPitch = xlength / numOfMicrocellInX;
  double microcellYPitch = ylength / numOfMicrocellInY;
  short xIndex = (x + xlength / 2) / microcellXPitch;
  short yIndex = (y + xlength / 2) / microcellYPitch;
  unsigned short microcellId = (yIndex * numOfMicrocellInX) + xIndex;
  return microcellId;
}

Microcell &SiPM::GetMicrocell(Photon ph) {
  return GetMicrocell(ph.xposition, ph.yposition);
}

Microcell &SiPM::GetMicrocell(double x, double y) {
  return vecOfMicrocells[MicrocellId(x, y)];
}

Microcell &SiPM::GetMicrocell(unsigned short xbin, unsigned short ybin) {
  return vecOfMicrocells[GetCellIDFromBins(xbin, ybin)];
}

double SiPM::GetIntegratedCharge(Microcell &mc, double arrivalTime) {
  double integCharge = mc.IntegratedCharge(arrivalTime);
  if (verbose)
    std::cout << "Integrated charge of photon : " << integCharge << std::endl;
  return integCharge;
}

/*
double SiPM::GetIntegratedCharge_V1(Photon ph)
{
  double integCharge = GetMicrocell(ph).IntegratedCharge(ph.arrivalTime);
  if (verbose) std::cout << "Integrated charge of photon : " << integCharge <<
std::endl; return integCharge;

}

double SiPM::GetIntegratedCharge_V2(Photon ph)
{
  return GetIntegratedCharge(GetMicrocell(ph),ph.arrivalTime);
}

double SiPM::GetIntegratedCharge(Photon ph)
{
  //double integCharge = GetMicrocell(ph.xposition,
ph.yposition).IntegratedCharge(ph.arrivalTime); double integCharge =
GetMicrocell(ph).IntegratedCharge(ph.arrivalTime); if (verbose) std::cout <<
"Integrated charge of photon : " << integCharge << std::endl; return
integCharge;

}*/
double SiPM::GetIntegratedCharge(Photon ph) {
  return GetIntegratedCharge(GetMicrocell(ph), ph.arrivalTime);
}

double SiPM::GetIntegratedCharge(std::vector<Photon> photonVec) {
  double totalCharge = 0;
  for (auto ph : photonVec) {
    totalCharge += GetIntegratedCharge(ph);
  }
  return totalCharge;
}

SiPM::~SiPM() {}

unsigned short SiPM::GetCellIDFromBins(unsigned short xbin,
                                       unsigned short ybin) {
  return ybin * numberOfMicrocellsX + xbin;
}

std::vector<CrosstalkHit> SiPM::GetCrossTalk(Photon ph) {
  return GetCrossTalk(GetMicrocell(ph), ph.arrivalTime);
}
std::vector<CrosstalkHit> SiPM::GetCrossTalk(Microcell primary,
                                             double primaryArrivalTime) {
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  std::vector<CrosstalkHit> queue = {
      {primary.GetXBin(), primary.GetYBin(), primaryArrivalTime}};
  std::vector<CrosstalkHit> new_crosstalk_cells;

  std::set<unsigned short> global_fired_cells;

  while (!queue.empty()) {
    CrosstalkHit currItem = queue.front();
    queue.erase(queue.begin());

    double currTime = currItem.arrivalTime;
    unsigned short currXbin = currItem.xbin;
    unsigned short currYbin = currItem.ybin;

    std::vector<CrosstalkHit> neighbors = {
        {static_cast<unsigned short>(currXbin - 1),
         static_cast<unsigned short>(currYbin), currTime},
        {static_cast<unsigned short>(currXbin + 1),
         static_cast<unsigned short>(currYbin), currTime},
        {static_cast<unsigned short>(currXbin),
         static_cast<unsigned short>(currYbin - 1), currTime},
        {static_cast<unsigned short>(currXbin),
         static_cast<unsigned short>(currYbin + 1), currTime}};

    for (auto &neighbor : neighbors) {
      // 1. Boundary check
      // if (neighbor.x >= 0 && neighbor.x < grid_size && neighbor.y >= 0 &&
      // neighbor.y < grid_size)
      {

        // 2. Skip check: if not already fired
        unsigned short neighborCellID = neighbor.GetCellID();
        if (global_fired_cells.find(neighborCellID) ==
            global_fired_cells.end()) {

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
  new_crosstalk_cells.push_back(
      {primary.GetXBin(), primary.GetYBin(), primaryArrivalTime});
  return new_crosstalk_cells;
}
