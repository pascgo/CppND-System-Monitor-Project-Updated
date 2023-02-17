/*
  process.h file 
  
  Evolution : 
      15 Feb 2023 : Remove of TODO comment 
      14 Feb 2023 : number max of character for command 
      09-Feb-2023 : Creation 
*/

#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                        
  std::string User();              
  std::string Command();         
  float CpuUtilization();    
  std::string Ram();                
  long int UpTime();          
  bool operator<(Process const& a) const; 
// Declaration of any necessary private members
 private:
  int pid_;
  float cpu_;
};

// Constant declaration 
const int nCmdSize =40;		// number max of character for command 


#endif