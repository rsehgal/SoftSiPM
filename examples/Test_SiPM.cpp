/*
**	Filename : Test_SiPM.cpp
**	2026-10-15
**	username : rsehgal
*/
#include <iostream>
#include "SiPM.h"
int main(int argc, char *argv[])
{
  SiPM s(0, 100);
  std::cout << "Numbe of Microcells : " << s.size() << std::endl;
  return 0;
}
