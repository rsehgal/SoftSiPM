/*
**	Filename : SiPM.h
**	2026-10-15
**	username : rsehgal
*/
#ifndef SiPM_h
#define SiPM_h

#include "Microcell.h"
#include <vector>

class SiPM {
  std::vector<Microcell> vecOfMicrocells;
  unsigned short sipmId;
  unsigned short numOfMicrocells;

public:
  SiPM();
  SiPM(unsigned short sid, unsigned short nMicrocells);
  ~SiPM();
  unsigned short size() const;
  unsigned short GetID() const;
};

#endif
