/*
  processor.h file 
  
  Evolution : 
      15 Feb 2023 : Remove of TODO comment 
      09-Feb-2023 : Creation 
*/

#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization(); 

  // Declaration of any necessary private members
 private:
   float prevIdle_ = 0.0;
   float prevNonIdle_ = 0.0;
};

#endif