#include "benchmark/benchmark.h"
#include "main/jucc.h"

// NOLINTNEXTLINE
static void BenchmarkRadd(benchmark::State &state) {
  for (const auto &_ : state) {
    jucc::Radd(0, 1000000);
  }
}

BENCHMARK(BenchmarkRadd);

// NOLINTNEXTLINE
static void BenchmarkJuCC(benchmark::State &state) {
  std::string file_grammar = "../benchmark/input/benchmark_grammar.g";
  std::string file_input = "../benchmark/input/in1.cc";
  for (const auto &_ : state) {
    jucc::grammar::Parser grammar_parser = jucc::grammar::Parser(file_grammar.c_str());
    grammar_parser.Parse();
    jucc::grammar::Productions raw_productions = grammar_parser.GetProductions();
    jucc::grammar::Productions productions = jucc::utils::RemoveAllPossibleAmbiguity(raw_productions);
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

    std::ifstream ifs(file_input);
    std::vector<std::string> input_tokens;
    jucc::lexer::Lexer lexer = jucc::lexer::Lexer();
    int token;
    while ((token = lexer.GetToken(ifs)) != jucc::lexer::TOK_EOF) {
      input_tokens.emplace_back(jucc::lexer::Lexer::GetTokenType(token));
    }

    jucc::parser::Parser parser = jucc::parser::Parser();
    parser.SetInputString(input_tokens);
    parser.SetStartSymbol(grammar_parser.GetStartSymbol());
    parser.SetParsingTable(parsing_table);
    while (!parser.IsComplete()) {
      parser.ParseNextStep();
    }
    parser.BuildParseTree();
  }
}

BENCHMARK(BenchmarkJuCC);
