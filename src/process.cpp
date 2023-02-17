/*
  Process cpp file 
  
  Evolution : 
      16-Feb 2023 : Subtractration from UpTime of the time the process started after system boot.
      14 Feb 2023 : command list not exceeding nCmdSize characters 
      14 Feb 2023 : Using namespace std
      14 Feb 2023 : Remove of TODO comment 
      09-Feb-2023 : Creation 
*/

#include "process.h"
#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"


// using directive 
using namespace std;

Process::Process(int pid) : pid_(pid) {
  cpu_ = LinuxParser::ActiveJiffies(pid);
}

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return LinuxParser::CpuUtilization(Pid()); }

// Return the command that generated this process
std::string Process::Command() {
   string cmdPidLtd = "";
   return (cmdPidLtd.append(LinuxParser::Command(Pid()),0,nCmdSize));
}

// Return this process's memory utilization
std::string Process::Ram() { return LinuxParser::Ram(Pid()); }

// Return the user (name) that generated this process
std::string Process::User() { return LinuxParser::User(Pid()); }

// Return the age of this process (in seconds)
long int Process::UpTime() {
  return (LinuxParser::UpTime()-LinuxParser::UpTime(Pid())); 
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a ) const { return this->cpu_ > a.cpu_; }
