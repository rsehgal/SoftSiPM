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

// A flexible template function for integers and floating-point numbers
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
};

extern bool compareByArrivalTime(const Photon& a, const Photon& b);
// Only for testing
extern std::vector<Photon> GetRandomPhotonVec();

#endif
