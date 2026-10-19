/*
**	Filename : SiPM.cpp
**	2026-10-15
**	username : rsehgal
*/
#include <iostream>
#include "SiPM.h"
#include <algorithm>
#include <set>
#include <utility>
#include <random>
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

/*
double SiPM::GetIntegratedCharge_V1(Photon ph)
{
  double integCharge = GetMicrocell(ph).IntegratedCharge(ph.arrivalTime);
  if (verbose) std::cout << "Integrated charge of photon : " << integCharge << std::endl;
  return integCharge;

}

double SiPM::GetIntegratedCharge_V2(Photon ph)
{
  return GetIntegratedCharge(GetMicrocell(ph),ph.arrivalTime);
}

double SiPM::GetIntegratedCharge(Photon ph)
{
  //double integCharge = GetMicrocell(ph.xposition, ph.yposition).IntegratedCharge(ph.arrivalTime);
  double integCharge = GetMicrocell(ph).IntegratedCharge(ph.arrivalTime);
  if (verbose) std::cout << "Integrated charge of photon : " << integCharge << std::endl;
  return integCharge;

}*/
double SiPM::GetIntegratedCharge(Photon ph)
{
  return GetIntegratedCharge(GetMicrocell(ph), ph.arrivalTime);
}

double SiPM::GetIntegratedCharge(std::vector<Photon> photonVec)
{
  double totalCharge = 0;
  for (auto ph : photonVec) {
    totalCharge += GetIntegratedCharge(ph);
  }
  return totalCharge;
}

SiPM::~SiPM() {}

unsigned short SiPM::GetCellIDFromBins(unsigned short xbin, unsigned short ybin)
{
  return ybin * numberOfMicrocellsX + xbin;
}

std::vector<Microcell> SiPM::GetCrossTalk(Photon ph)
{
  return GetCrossTalk(GetMicrocell(ph));
}
std::vector<CrosstalkHit> SiPM::GetCrossTalk_V2(Photon ph)
{
  return GetCrossTalk(GetMicrocell(ph),ph.arrivalTime);
}

std::vector<CrosstalkHit> SiPM::GetCrossTalk(Microcell primary, double primaryArrivalTime)
{
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  std::vector<CrosstalkHit> queue = {{primary, primaryArrivalTime}};
  std::vector<CrosstalkHit> new_crosstalk_cells;

  std::set<Microcell> global_fired_cells;
  while (!queue.empty()) {
    CrosstalkHit currItem = queue.front();
    // new_crosstalk_cells.push_back(curr);
    queue.erase(queue.begin());

    Microcell curr  = currItem.cell;
    double currTime = currItem.arrivalTime;

    // double q                = 0.0694;
    unsigned short currXbin = curr.GetXBin();
    unsigned short currYbin = curr.GetYBin();

    std::vector<Microcell> neighbors = {GetMicrocell(currXbin - 1, currYbin), GetMicrocell(currXbin + 1, currYbin),
                                        GetMicrocell(currXbin, currYbin - 1), GetMicrocell(currXbin, currYbin + 1)};

    for (const auto &neighbor : neighbors) {
      // 1. Boundary check
      // if (neighbor.x >= 0 && neighbor.x < grid_size && neighbor.y >= 0 && neighbor.y < grid_size)
      {

        // 2. Skip check: if not already fired
        if (global_fired_cells.find(neighbor) == global_fired_cells.end()) {

          // 3. Roll for crosstalk
          if (dist(rng) < q) {
            double crosstalkTime = currTime + 0.01;
            global_fired_cells.insert(neighbor);
            new_crosstalk_cells.push_back({neighbor, crosstalkTime});
            queue.push_back({neighbor, crosstalkTime}); // Propagate further
          }
        }
      }
    }
  }

  // Intentionally putting the primary microcell at the end of crosstalk vector
  new_crosstalk_cells.push_back({primary,primaryArrivalTime});
  return new_crosstalk_cells;
}
std::vector<Microcell> SiPM::GetCrossTalk(Microcell primary)
{
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  std::vector<Microcell> queue = {primary};
  std::vector<Microcell> new_crosstalk_cells;

  std::set<Microcell> global_fired_cells;
  while (!queue.empty()) {
    Microcell curr = queue.front();
    // new_crosstalk_cells.push_back(curr);
    queue.erase(queue.begin());

    // double q                = 0.0694;
    unsigned short currXbin = curr.GetXBin();
    unsigned short currYbin = curr.GetYBin();

    std::vector<Microcell> neighbors = {GetMicrocell(currXbin - 1, currYbin), GetMicrocell(currXbin + 1, currYbin),
                                        GetMicrocell(currXbin, currYbin - 1), GetMicrocell(currXbin, currYbin + 1)};

    for (const auto &neighbor : neighbors) {
      // 1. Boundary check
      // if (neighbor.x >= 0 && neighbor.x < grid_size && neighbor.y >= 0 && neighbor.y < grid_size)
      {

        // 2. Skip check: if not already fired
        if (global_fired_cells.find(neighbor) == global_fired_cells.end()) {

          // 3. Roll for crosstalk
          if (dist(rng) < q) {
            global_fired_cells.insert(neighbor);
            new_crosstalk_cells.push_back(neighbor);
            queue.push_back(neighbor); // Propagate further
          }
        }
      }
    }
  }

  // Intentionally putting the primary microcell at the end of crosstalk vector
  new_crosstalk_cells.push_back(primary);
  return new_crosstalk_cells;
}
