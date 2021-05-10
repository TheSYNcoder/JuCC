/**-------------------------------------------------------------------------
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

/**
 * jucc begins execution here.
 */
auto main(int argc, char *argv[]) -> int {
  /* print a hello world message */
  std::cout << jucc::Hello();

  /**
   * InputParser parses the cmd line arguments and returns
   * input file path and grammar file path
   */
  jucc::InputParser input_parser = jucc::InputParser(argc, argv);
  std::string file_grammar = input_parser.GetArgument("-g");
  std::string file_input = input_parser.GetArgument("-f");
  std::string output_file = input_parser.GetArgument("-o");
  if (output_file.empty() || file_input.empty() || file_grammar.empty()) {
    std::cout << "jucc: usage: jucc -g <grammar_file> -f <input_file> -o <output_json_file>\n";
    return 0;
  }

  /* Parse the grammar file and check for formatting errors */
  jucc::grammar::Parser grammar_parser = jucc::grammar::Parser(file_grammar.c_str());
  if (!grammar_parser.Parse()) {
    std::cout << "jucc: " << grammar_parser.GetError() << '\n';
    return 0;
  }

  /* Check if the input file path is good */
  std::ifstream ifs(file_input);
  if (!ifs.good()) {
    std::cout << "jucc: cannot read input file, bad file!\n";
    return 0;
  }

  /**
   * Get the parsed grammar production and process it
   * Steps include:
   * 1. Left recursion removal
   * 2. Left factoring
   */
  jucc::grammar::Productions raw_productions = grammar_parser.GetProductions();
  jucc::grammar::Productions productions = jucc::utils::RemoveAllPossibleAmbiguity(raw_productions);

  /* Calculate first and follows to build the LL(1) parsing table */
  auto nullables = jucc::utils::CalcNullables(productions);
  auto firsts = jucc::utils::CalcFirsts(productions, nullables);
  auto follows = jucc::utils::CalcFollows(productions, firsts, nullables, grammar_parser.GetStartSymbol());
  auto terminals = grammar_parser.GetTerminals();
  auto non_terminals = jucc::utils::GetAllNonTerminals(productions);

  jucc::parser::ParsingTable parsing_table = jucc::parser::ParsingTable(terminals, non_terminals);
  parsing_table.SetFirsts(firsts);
  parsing_table.SetFollows(follows);
  parsing_table.SetProductions(productions);
  parsing_table.BuildTable();

  /* Check for errors in grammar and exit if errors exist */
  auto err = parsing_table.GetErrors();
  if (!err.empty()) {
    std::cout << "jucc: ";
    for (auto &e : err) {
      std::cout << e << '\n';
    }
    return 0;
  }

  /* Use Lexer to get input tokens */
  std::vector<std::string> input_tokens;
  jucc::lexer::Lexer lexer = jucc::lexer::Lexer();
  int token;
  while ((token = lexer.GetToken(ifs)) != jucc::lexer::TOK_EOF) {
    input_tokens.emplace_back(jucc::lexer::Lexer::GetTokenType(token));
  }

  /* Check for symbol table errors and exit if errors exist */
  std::vector<std::string> errors = lexer.GetUndeclaredSymbolErrors();
  errors.insert(errors.end(), lexer.GetDuplicateSymbolErrors().begin(), lexer.GetDuplicateSymbolErrors().end());
  if (!errors.empty()) {
    std::cout << "jucc: ";
    for (auto &e : errors) {
      std::cout << e << '\n';
    }
    return 0;
  }

  /* Parse the input file using the parsing table and report errors */
  jucc::parser::Parser parser = jucc::parser::Parser();
  parser.SetInputString(input_tokens);
  parser.SetStartSymbol(grammar_parser.GetStartSymbol());
  parser.SetParsingTable(parsing_table);

  while (!parser.IsComplete()) {
    parser.ParseNextStep();
  }

  err = parser.GetParserErrors();
  if (!err.empty()) {
    std::cout << "jucc: ";
    for (auto &e : err) {
      std::cout << e << '\n';
    }
  }

  /* If there are no parser errors then proceed to generate the parse tree */
  if (err.empty()) {
    parser.BuildParseTree();
    if (!parser.WriteParseTree(output_file)) {
      std::cout << "jucc: Couldn't write parse tree to " + output_file + '\n';
      return 0;
    }
    std::string command = "cd ../server && npm start " + output_file;
    system(command.c_str());
  }

  return 0;
}
