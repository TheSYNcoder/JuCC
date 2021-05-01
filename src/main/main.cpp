/*-------------------------------------------------------------------------
 *
 * main.cpp
 *	  Stub main() routine for the jucc executable.
 *
 * This does some essential startup tasks for any incarnation of jucc
 *
 * Contributors
 *    Shuvayan Ghosh Dastidar
 *    Abhishek Pal
 *    Aritra Samanta
 *    Bisakh Mondal
 *
 * Department of Computer Science and Engineering, Jadavpur University
 *
 *
 * IDENTIFICATION
 *	  src/main/main.cpp
 *
 *-------------------------------------------------------------------------
 */

#include "main/jucc.h"
using jucc::Hello;
/*
 * jucc begins execution here.
 */
auto main() -> int {
  // print a hello world message
  std::cout << Hello();
  return 0;
}
