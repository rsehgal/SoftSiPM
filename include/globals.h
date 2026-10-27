/*
**	Filename : globals.h
**	2026-10-16
**	username : rsehgal
*/
#ifndef globals_h
#define globals_h

#include <vector>
#include <random>
#include <type_traits>
#include <iostream>

// Inside SiPM.h (e.g., right before the class definition)
enum class ChargeMode { NoCrosstalk, CrosstalkOnly, CrosstalkAndAfterpulse, CrosstalkAfterpulseDarkCount };

// A flexible template function for integers and floating-point numbers
struct Photon {
  double xposition;
  double yposition;
  double arrivalTime;

  Photon(double xpos, double ypos, double arrTime)
  {
    xposition   = xpos;
    yposition   = ypos;
    arrivalTime = arrTime;
  }

  void Print() { std::cout << xposition << " : " << yposition << " ::  " << arrivalTime << std::endl; }
};

extern bool verbose;
// Only for testing
extern std::vector<Photon> GetRandomPhotonVec();

extern double tauAfterpulse;
extern double pAfterpulse;
extern double eventWindow;
extern double preStart;
extern double darkCountRate;
extern double sipmXLength;
extern double sipmYLength;

extern unsigned short totalNumberOfMicrocells;
extern unsigned short numberOfMicrocellsX;
extern unsigned short numberOfMicrocellsY;
extern double q;

struct CrosstalkHit {
  unsigned short xbin;
  unsigned short ybin;
  double arrivalTime;

  bool isValid() const { return xbin < numberOfMicrocellsX && ybin < numberOfMicrocellsY; }

  unsigned short GetCellID() const { return ybin * numberOfMicrocellsX + xbin; }
};

extern bool compareByArrivalTime(const Photon &a, const Photon &b);
extern bool compareHitByArrivalTime(const CrosstalkHit &a, const CrosstalkHit &b);

template <typename T>
T getRandomNumber(T min, T max)
{
  // thread_local keeps the engine alive across function calls (safe and fast)
  thread_local static std::random_device rd;
  thread_local static std::mt19937 gen(rd());

  if constexpr (std::is_integral_v<T>) {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(gen);
  } else {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(gen);
  }
}

#endif
