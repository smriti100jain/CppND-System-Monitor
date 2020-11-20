#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include <processor.h>
#include <process.h>
#include <vector>
int main() {
  System system;
  int num_processes = 30;
  NCursesDisplay::Display(system,num_processes);
}