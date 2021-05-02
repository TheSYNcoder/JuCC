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

/*
 * jucc begins execution here.
 */
auto main(int argc, char *argv[]) -> int {
  // print a hello world message
  std::cout << jucc::Hello();
  jucc::InputParser input_parser = jucc::InputParser(argc, argv);

  std::string file_grammar = input_parser.GetArgument("-g");
  if (file_grammar.empty()) {
    std::cout << "USAGE \n";
    std::cout << "____________________\n";
    std::cout << "jucc -g <grammar_file> -f <input_file>\n";
    return 0;
  }
  std::string file_input = input_parser.GetArgument("-f");
  if (file_input.empty()) {
    std::cout << "USAGE \n";
    std::cout << "____________________\n";
    std::cout << "jucc -g <grammar_file> -f <input_file>\n";
    return 0;
  }

  jucc::grammar::Parser grammar_parser = jucc::grammar::Parser(file_grammar.c_str());
  if (!grammar_parser.Parse()) {
    std::cout << "Wrong grammar file format\n";
    return 0;
  }

  std::ifstream ifs(file_input);
  if (!ifs.good()) {
    std::cout << "Cannot read input file, bad file !\n";
    return 0;
  }

  jucc::grammar::Productions productions = grammar_parser.GetProductions();

  // TODO(bisakh,abhiskek) : Add code for removing left factors and left recursion

  auto nullables = jucc::utils::CalcNullables(productions);
  auto firsts = jucc::utils::CalcFirsts(productions, nullables);
  auto follows = jucc::utils::CalcFollows(productions, firsts, nullables, grammar_parser.GetStartSymbol());

  auto terminals = grammar_parser.GetTerminals();
  auto non_terminals = grammar_parser.GetNonTerminals();

  jucc::parsing_table::ParsingTable parsing_table = jucc::parsing_table::ParsingTable(terminals, non_terminals);
  parsing_table.SetFirsts(firsts);
  parsing_table.SetFollows(follows);
  parsing_table.SetProductions(productions);
  parsing_table.BuildTable();

  // use lexer to get input tokens
  std::vector<std::string> input_tokens;
  jucc::lexer::Lexer lexer = jucc::lexer::Lexer();
  int token;
  while ((token = lexer.GetToken(ifs)) != jucc::lexer::TOK_EOF) {
    input_tokens.emplace_back(jucc::lexer::Lexer::GetTokenType(token));
  }

  jucc::parsing::Parsing parser = jucc::parsing::Parsing();
  parser.Init();
  parser.SetInputString(input_tokens);
  parser.SetStartSymbol(grammar_parser.GetStartSymbol());
  parser.SetParsingTable(parsing_table);

  while (!parser.IsComplete()) {
    parser.ParseNextStep();
  }

  return 0;
}
