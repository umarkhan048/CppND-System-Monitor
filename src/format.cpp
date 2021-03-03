#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  long hour, min;
  hour = seconds/3600;
  seconds = seconds%3600;
  min = seconds/60;
  string sec = std::to_string(seconds%60);
  sec.insert(0, 2 - sec.length(), '0');
  return std::to_string(hour) + ":" + std::to_string(min) + ":" + sec; 
}