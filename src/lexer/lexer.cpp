#include "lexer/lexer.h"

namespace jucc::lexer {

int Lexer::GetToken(std::ifstream &is) {
  static char last_char = ' ';

  while (!is.eof() && (isspace(last_char) != 0)) {
    is.get(last_char);
  }

  // return TOK_EOF if end of file is reached.
  if (is.eof()) {
    return TOK_EOF;
  }

  // check for identifier and literal tokens
  // RE : [a-zA-Z][0-9a-zA-Z]
  if (isalpha(last_char) != 0) {
    identifier_string_ = last_char;
    int ret_token = TOK_IDENTIFIER;
    while (is.get(last_char) && (isalnum(last_char) != 0)) {
      identifier_string_ += last_char;
    }
    if (identifier_string_ == "int") {
      current_datatype_ = identifier_string_;
      ret_token = TOK_INT;
    } else if (identifier_string_ == "float") {
      current_datatype_ = identifier_string_;
      ret_token = TOK_FLOAT;
    } else if (identifier_string_ == "void") {
      current_datatype_ = identifier_string_;
      ret_token = TOK_VOID;
    } else if (identifier_string_ == "if") {
      current_datatype_ = "";
      ret_token = TOK_IF;
    } else if (identifier_string_ == "else") {
      current_datatype_ = "";
      ret_token = TOK_ELSE;
    } else if (identifier_string_ == "cout") {
      current_datatype_ = "";
      ret_token = TOK_COUT;
    } else if (identifier_string_ == "cin") {
      current_datatype_ = "";
      ret_token = TOK_CIN;
    } else if (identifier_string_ == "main") {
      current_datatype_ = "";
      ret_token = TOK_MAIN;
    } else {
      ret_token = TOK_IDENTIFIER;

      /**
       *  if identifier insert into symbol_table
       */
      auto *node = new symbol_table::Node(identifier_string_, current_datatype_, current_nesting_level_);
      symbol_table_.CheckAndAddEntry(node);
      delete node;
      if (!symbol_table_.GetDuplicateSymbols().empty()) {
        duplicate_symbol_errors_ = symbol_table_.GetDuplicateSymbols();
      }
      if (!symbol_table_.GetUndeclaredSymbols().empty()) {
        undeclared_symbol_errors_ = symbol_table_.GetUndeclaredSymbols();
      }
    }
    if (is.eof()) {
      last_char = ' ';
    }
    return ret_token;
  }

  // RE : [0-9]+.?[0-9]*
  if (isdigit(last_char) != 0) {
    std::string num_string;
    num_string += last_char;
    // to check if fractional
    int dot_count = 0;
    while (is.get(last_char) && ((isdigit(last_char) != 0) || last_char == '.')) {
      if (last_char == '.') {
        dot_count++;
      }
      num_string += last_char;
    }
    if (isalpha(last_char) != 0) {
      // read till the end of the string
      // panic mode
      while (!is.eof() && (isalnum(last_char) != 0)) {
        num_string += last_char;
        is.get(last_char);
      }
      current_datatype_ = "";
      error_string_ = "Wrong fractional number format : " + num_string + "\n";
      return TOK_ERROR;
    }

    // fractional number
    int ret_token;
    if (dot_count == 1) {
      floatval_ = strtod(num_string.c_str(), nullptr);
      ret_token = TOK_FRACTIONAL;
    } else if (dot_count == 0) {
      intval_ = (static_cast<int>(strtod(num_string.c_str(), nullptr)));
      ret_token = TOK_DECIMAL;
    } else {
      error_string_ = "Wrong fractional number format : " + num_string + "\n";
      ret_token = TOK_ERROR;
    }
    if (is.eof()) {
      current_datatype_ = "";
      last_char = ' ';
    }
    return ret_token;
  }

  // handle punctuations
  if (ispunct(last_char) != 0) {
    int ret_token = TOK_ERROR;
    if (last_char == ';') {
      current_datatype_ = "";
      is.get(last_char);
      ret_token = TOK_SEMICOLON;
    } else if (last_char == ',') {
      is.get(last_char);
      ret_token = TOK_COMMA;
    } else if (last_char == '.') {
      is.get(last_char);
      ret_token = TOK_DOT;
    } else if (last_char == '+') {
      is.get(last_char);
      ret_token = TOK_PLUS;
    } else if (last_char == '-') {
      is.get(last_char);
      ret_token = TOK_MINUS;
    } else if (last_char == '*') {
      is.get(last_char);
      ret_token = TOK_MULTIPLY;
    } else if (last_char == '%') {
      is.get(last_char);
      ret_token = TOK_MODULUS;
    } else if (last_char == '{') {
      current_datatype_ = "";
      current_nesting_level_++;
      is.get(last_char);
      ret_token = TOK_CURLY_OPEN;
    } else if (last_char == '}') {
      current_datatype_ = "";
      symbol_table_.RemoveNodesOnScopeEnd(current_nesting_level_);
      current_nesting_level_--;
      is.get(last_char);
      ret_token = TOK_CURLY_CLOSE;
    } else if (last_char == '(') {
      is.get(last_char);
      ret_token = TOK_PAREN_OPEN;
    } else if (last_char == ')') {
      is.get(last_char);
      ret_token = TOK_PAREN_CLOSE;
    } else if (last_char == '=') {
      is.get(last_char);
      if (last_char == '=') {
        is.get(last_char);
        ret_token = TOK_EQUAL_TO;
      } else {
        ret_token = TOK_ASSIGNMENT;
      }
    } else if (last_char == '!') {
      is.get(last_char);
      if (last_char == '=') {
        is.get(last_char);
        ret_token = TOK_NOT_EQUAL_TO;
      } else {
        ret_token = TOK_NOT;
      }
    } else if (last_char == '<') {
      is.get(last_char);
      if (last_char == '<') {
        current_datatype_ = "";
        is.get(last_char);
        ret_token = TOK_LEFT_SHIFT;
      } else if (last_char == '=') {
        is.get(last_char);
        ret_token = TOK_LESS_THAN_OR_EQUALS;
      } else {
        ret_token = TOK_LESS_THAN;
      }
    } else if (last_char == '>') {
      is.get(last_char);
      if (last_char == '>') {
        current_datatype_ = "";
        is.get(last_char);
        ret_token = TOK_RIGHT_SHIFT;
      } else if (last_char == '=') {
        is.get(last_char);
        ret_token = TOK_GREATER_THAN_OR_EQUALS;
      } else {
        ret_token = TOK_GREATER_THAN;
      }
    } else if (last_char == '"') {
      literal_string_ = "";
      do {
        is.get(last_char);
        literal_string_ += last_char;
      } while (!is.eof() && last_char != '"');
      if (is.eof()) {
        error_string_ = "Wrong literal format\n";
        return TOK_ERROR;
      }
      // eliminating the " at the end
      literal_string_ = literal_string_.substr(0, literal_string_.length() - 1);
      is.get(last_char);
      ret_token = TOK_LITERAL;

    } else if (last_char == '\'') {
      literal_string_ = "";
      is.get(last_char);
      literal_string_ += last_char;
      if (is.eof()) {
        error_string_ = "Wrong literal format\n";
        return TOK_ERROR;
      }
      is.get(last_char);
      if (last_char == '\'') {
        is.get(last_char);
        ret_token = TOK_CHARACTER;
      } else {
        // read till the end of the string
        // panic mode
        while (!is.eof() && (isalnum(last_char) != 0)) {
          is.get(last_char);
        }
        error_string_ = "Wrong literal format\n";
        return TOK_ERROR;
      }

    } else if (last_char == '/') {
      is.get(last_char);
      if (last_char == '/') {
        // commented out line
        do {
          is.get(last_char);
        } while (!is.eof() && last_char != '\n' && last_char != '\r');
        is.get(last_char);
        ret_token = TOK_COMMENT;
      } else {
        ret_token = TOK_DIVIDE;
      }
    } else {
      error_string_ = "Unexpected Token\n";
      ret_token = TOK_ERROR;
    }
    if (is.eof()) {
      last_char = ' ';
    }
    return ret_token;
  }

  last_char = ' ';
  error_string_ = "Unexpected Token\n";
  return TOK_ERROR;
}

std::string Lexer::GetTokenType(int token) {
  std::string ret_string;
  if (token == TOK_INT) {
    ret_string = "int";
  } else if (token == TOK_FLOAT) {
    ret_string = "float";
  } else if (token == TOK_VOID) {
    ret_string = "void";
  } else if (token == TOK_IF) {
    ret_string = "if";
  } else if (token == TOK_ELSE) {
    ret_string = "else";
  } else if (token == TOK_PAREN_OPEN) {
    ret_string = "(";
  } else if (token == TOK_PAREN_CLOSE) {
    ret_string = ")";
  } else if (token == TOK_CURLY_OPEN) {
    ret_string = "{";
  } else if (token == TOK_CURLY_CLOSE) {
    ret_string = "}";
  } else if (token == TOK_COMMA) {
    ret_string = ",";
  } else if (token == TOK_DOT) {
    ret_string = ".";
  } else if (token == TOK_PLUS) {
    ret_string = "+";
  } else if (token == TOK_MINUS) {
    ret_string = "-";
  } else if (token == TOK_DIVIDE) {
    ret_string = "/";
  } else if (token == TOK_MULTIPLY) {
    ret_string = "*";
  } else if (token == TOK_MODULUS) {
    ret_string = "%";
  } else if (token == TOK_SEMICOLON) {
    ret_string = ";";
  } else if (token == TOK_LEFT_SHIFT) {
    ret_string = "<<";
  } else if (token == TOK_RIGHT_SHIFT) {
    ret_string = ">>";
  } else if (token == TOK_EQUAL_TO) {
    ret_string = "==";
  } else if (token == TOK_ASSIGNMENT) {
    ret_string = "=";
  } else if (token == TOK_NOT) {
    ret_string = "!";
  } else if (token == TOK_NOT_EQUAL_TO) {
    ret_string = "!=";
  } else if (token == TOK_GREATER_THAN) {
    ret_string = ">";
  } else if (token == TOK_LESS_THAN) {
    ret_string = "<";
  } else if (token == TOK_GREATER_THAN_OR_EQUALS) {
    ret_string = ">=";
  } else if (token == TOK_LESS_THAN_OR_EQUALS) {
    ret_string = "<=";
  } else if (token == TOK_IDENTIFIER) {
    ret_string = "identifier";
  } else if (token == TOK_DECIMAL) {
    ret_string = "integer_constant";
  } else if (token == TOK_FRACTIONAL) {
    ret_string = "float_constant";
  } else if (token == TOK_COUT) {
    ret_string = "cout";
  } else if (token == TOK_CIN) {
    ret_string = "cin";
  } else if (token == TOK_MAIN) {
    ret_string = "main";
  } else if (token == TOK_COMMENT) {
    ret_string = "ignore";
  } else {
    ret_string = "error";
  }

  return ret_string;
}

std::string Lexer::GetCurrentDatatype() { return current_datatype_; }

std::vector<std::string> Lexer::GetDuplicateSymbolErrors() { return duplicate_symbol_errors_; }

std::vector<std::string> Lexer::GetUndeclaredSymbolErrors() { return undeclared_symbol_errors_; }

[[nodiscard]] int Lexer::GetCurrentNestingLevel() const { return current_nesting_level_; }

}  // namespace jucc::lexer
