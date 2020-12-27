#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::check(long token){
  string value;
  if(token<10){
    return value = "0"+to_string(token);
  }
  else
    return value = to_string(token);
}


string Format::ElapsedTime(long seconds) {
  string time;
  long minute = seconds/60;
  long hour = minute/60;
  time = check(hour)+":"+check(long(minute%60))+":"+check(long (seconds%60));
  return time;}
  