/*
**	Filename : SiPM.cpp
**	2026-10-15
**	username : rsehgal
*/
#include <iostream>
#include "SiPM.h"
#include <algorithm>
SiPM::SiPM() {}

SiPM::SiPM(unsigned short sid, unsigned short nMicrocells) : sipmId(sid), numOfMicrocells(nMicrocells)
{

  for (unsigned short cid = 0; cid < numOfMicrocells; cid++) {
    vecOfMicrocells.push_back(Microcell{cid});
  }
  // std::fill(vecOfMicrocells.begin(), vecOfMicrocells.end(), Microcell{});
}

unsigned short SiPM::size() const
{
  return vecOfMicrocells.size();
}
SiPM::~SiPM() {}
