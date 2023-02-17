/*
  format.cpp file 
  
    01-Feb-2023 : Creation 
*/

#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
// string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); }
string Format::ElapsedTime(long seconds) {
  std::string hr = "";
  std::string min = "";
  std::string sec = "";

  hr = std::to_string(seconds / 3600);
  min = std::to_string((seconds % 3600) / 60);
  sec = std::to_string(seconds % 60);

  if(hr.length() == 1) { hr = "0" + hr; }
  if(min.length() == 1) { min = "0" + min; }
  if(sec.length() == 1) { sec = "0" + sec; }

  return (hr + ":" + min + ":" + sec);
}