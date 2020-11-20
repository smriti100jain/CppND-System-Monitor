#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long time_in_secs) { 
    long hh = time_in_secs/3600;
    long remaining_seconds = time_in_secs-(hh*3600);
    long mm = remaining_seconds/60;
    long ss = remaining_seconds - (mm*60);
    return (std::to_string(hh)+":"+std::to_string(mm)+":"+std::to_string(ss));
}