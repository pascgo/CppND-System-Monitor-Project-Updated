/*
  ncurses_display.h file 
  
  Evolution : 
    14-Feb-2023 : Number of process to display  
   	14-Feb-2023 : Add refresh time as constant REFRESH_WINDOW_C 
    09-Feb-2023 : Creation 
*/

#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"

#define NB_PROCESS_C 10						// Number of process to display
#define REFRESH_WINDOW_C 1					// Refresh Time for Windows in seconds


namespace NCursesDisplay {
void Display(System& system, int nbProcess = NB_PROCESS_C);		
void DisplaySystem(System& system, WINDOW* window);
void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n);
std::string ProgressBar(float percent);
};  // namespace NCursesDisplay

#endif