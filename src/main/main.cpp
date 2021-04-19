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

#include "lexer/lexer.h"
#include "main/jucc.h"
using jucc::Hello;

/*
 * jucc begins execution here.
 */
auto main() -> int {
  // print a hello world message
  Hello();
  std::string filename("/Users/shuvayan/Desktop/local/test/post/JuCC/test/lexer/input.txt");
  FILE *fp = fopen(filename.c_str(), "r");
  int token;
  token = jucc::GetToken(fp);
  std::cout << "\n" << token << "\n" << jucc::TOK_INT << "\n";
  token = jucc::GetToken(fp);
  std::cout << "\n" << token << "\n" << jucc::TOK_INT << "\n";
  token = jucc::GetToken(fp);
  std::cout << "\n" << token << "\n" << jucc::TOK_INT << "\n";
  token = jucc::GetToken(fp);
  std::cout << "\n" << token << "\n" << jucc::TOK_INT << "\n";
  token = jucc::GetToken(fp);
  std::cout << "\n" << token << "\n" << jucc::TOK_INT << "\n";
  token = jucc::GetToken(fp);
  std::cout << "\n" << token << "\n" << jucc::TOK_INT << "\n";
  return 0;
}
