/*
  processor.cpp file 
  
  Evolution : 
      15 Feb 2023 : usage of jiffies   
      15 Feb 2023 : usage of namespace std   
      14 Feb 2023 : usage of filterCpu instead of "cpu"  
      15 Feb 2023 : Remove of TODO comment 
      09-Feb-2023 : Creation 
*/

#include <string>
#include "processor.h"
#include "linux_parser.h"
#include <iostream>
#include <unistd.h>			// for sysconf

// using directive 
using namespace std;


#define printVariableNameAndValue(x) cout << "The name of variable ** " << (#x) << " ** and the value of variable is => " <<x<<"\n"


// Return the aggregate CPU utilization
float Processor::Utilization() {
  float cpuUtil = 0.0;
  float Idle = 0.0;					
  float NonIdle = 0.0;
  
  // Get time cpu is active
  NonIdle = float(LinuxParser::ActiveJiffies(LinuxParser::filterCpu));
  // Get time cpu is suspended
  Idle =  float(LinuxParser::Jiffies(LinuxParser::filterCpu)) - NonIdle ; 

  // Compute variation 
  float diffIdle = Idle - prevIdle_ ;
  float diffNonIdle = NonIdle - prevNonIdle_;

  cpuUtil = (diffNonIdle) / (diffIdle + diffNonIdle);

  // Save previous value
  prevIdle_ = Idle;
  prevNonIdle_ = NonIdle;
  
  return cpuUtil;
}