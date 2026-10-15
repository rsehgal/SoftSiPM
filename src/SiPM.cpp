/*
**	Filename : SiPM.cpp
**	2026-10-15
**	username : rsehgal
*/
#include <iostream>
#include "SiPM.h"
#include <algorithm>
SiPM::SiPM() {}

SiPM::SiPM(unsigned short sid, unsigned short nMicrocells, double xlen, double ylen) : sipmId(sid), numOfMicrocells(nMicrocells), xlength(xlen), ylength(ylen)
{

  for (unsigned short cid = 0; cid < numOfMicrocells; cid++) {
    vecOfMicrocells.push_back(Microcell{cid,numOfMicrocells});
  }
  // std::fill(vecOfMicrocells.begin(), vecOfMicrocells.end(), Microcell{});
}

unsigned short SiPM::size() const
{
  return vecOfMicrocells.size();
}

unsigned short SiPM::GetID() const{
  return sipmId;
}

unsigned short SiPM::MicrocellId(double x, double y){
  short numOfMicrocellInX = std::sqrt(numOfMicrocells);
  short numOfMicrocellInY = std::sqrt(numOfMicrocells);
  double microcellXPitch = xlength / numOfMicrocellInX; 
  double microcellYPitch = ylength / numOfMicrocellInY; 
  short xIndex = (x+xlength/2)/microcellXPitch;
  short yIndex = (y+xlength/2)/microcellYPitch;
  unsigned short microcellId = (yIndex * numOfMicrocellInX) + xIndex;
  return microcellId;
}

Microcell& SiPM::GetMicrocell(double x, double y){
  return vecOfMicrocells[MicrocellId(x,y)];
}

SiPM::~SiPM() {}
