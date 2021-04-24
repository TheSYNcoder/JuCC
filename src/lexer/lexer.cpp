#include "lexer/lexer.h"

namespace jucc {

int Lexer::GetToken(std::ifstream &is) {
  static char last_char = ' ';

  while (!is.eof() && (isspace(last_char) != 0)) {
    is.get(last_char);
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
      ret_token = TOK_INT;
    } else if (identifier_string_ == "float") {
      ret_token = TOK_FLOAT;
    } else if (identifier_string_ == "void") {
      ret_token = TOK_VOID;
    } else if (identifier_string_ == "if") {
      ret_token = TOK_IF;
    } else if (identifier_string_ == "else") {
      ret_token = TOK_ELSE;
    } else if (identifier_string_ == "cout") {
      ret_token = TOK_COUT;
    } else if (identifier_string_ == "cin") {
      ret_token = TOK_CIN;
    } else {
      ret_token = TOK_IDENTIFIER;
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
      last_char = ' ';
    }
    return ret_token;
  }

  // handle punctuations
  if (ispunct(last_char) != 0) {
    int ret_token = TOK_ERROR;
    if (last_char == ';') {
      is.get(last_char);
      ret_token = TOK_SEMICOLON;
    } else if (last_char == ',') {
      is.get(last_char);
      ret_token = TOK_COMMA;
    } else if (last_char == '.') {
      is.get(last_char);
      ret_token = TOK_DOT;
    } else if (last_char == '{') {
      is.get(last_char);
      ret_token = TOK_CURLY_OPEN;
    } else if (last_char == '}') {
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
    } else if (last_char == '<') {
      is.get(last_char);
      if (last_char == '<') {
        is.get(last_char);
        ret_token = TOK_LEFT_SHIFT;
      } else {
        ret_token = TOK_LESS_THAN;
      }
    } else if (last_char == '>') {
      is.get(last_char);
      if (last_char == '>') {
        is.get(last_char);
        ret_token = TOK_RIGHT_SHIFT;
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
        while (!is.eof() && (isalnum(last_char) == 0)) {
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
}  // namespace jucc
