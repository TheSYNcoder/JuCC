#include "lexer/lexer.h"

namespace jucc {

int Lexer::GetToken(FILE *fp) {
  static int lastChar = ' ';

  while (lastChar != EOF && isspace(lastChar)) lastChar = getc(fp);

  // check for identifier and literal tokens
  // RE : [a-zA-Z][0-9a-zA-Z]
  if (isalpha(lastChar)) {
    identifier_string_ = lastChar;
    while (lastChar != EOF && isalnum(lastChar = getc(fp))) identifier_string_ += lastChar;
    if (identifier_string_ == "int")
      return TOK_INT;
    else if (identifier_string_ == "float")
      return TOK_FLOAT;
    else if (identifier_string_ == "void")
      return TOK_VOID;

    // conditionals
    else if (identifier_string_ == "if")
      return TOK_IF;
    else if (identifier_string_ == "else")
      return TOK_ELSE;

    // cout , cin
    else if (identifier_string_ == "cout")
      return TOK_COUT;
    else if (identifier_string_ == "cin")
      return TOK_CIN;

    else
      return TOK_IDENTIFIER;
  }

  // RE : [0-9]+.?[0-9]*
  if (isdigit(lastChar)) {
    std::string numString;
    numString += lastChar;
    lastChar = getc(fp);
    // to check if fractional
    int dot_count = 0;
    while (!feof(fp) && (isdigit(lastChar) || lastChar == '.')) {
      if (lastChar == '.') dot_count++;
      numString += lastChar;
      if (dot_count == 2) {
        error_string_ = "Wrong fractional number format : " + numString + "\n";
        return TOK_ERROR;
      }

      lastChar = getc(fp);
    }
    if (isalpha(lastChar)) {
      error_string_ = "Wrong fractional number format : " + numString + "\n";
      return TOK_ERROR;
    }

    // fractional number
    if (dot_count) {
      floatval_ = strtod(numString.c_str(), nullptr);
      return TOK_FRACTIONAL;
    } else {
      intval_ = (static_cast<int>(strtod(numString.c_str(), nullptr)));
      return TOK_DECIMAL;
    }
  }

  // handle punctuations
  if (ispunct(lastChar)) {
    if (lastChar == ';') {
      lastChar = getc(fp);
      return TOK_SEMICOLON;
    } else if (lastChar == ',') {
      lastChar = getc(fp);
      return TOK_COMMA;
    } else if (lastChar == '.') {
      lastChar = getc(fp);
      return TOK_DOT;
    } else if (lastChar == '{') {
      lastChar = getc(fp);
      return TOK_CURLY_OPEN;
    } else if (lastChar == '}') {
      lastChar = getc(fp);
      return TOK_CURLY_CLOSE;
    } else if (lastChar == '(') {
      lastChar = getc(fp);
      return TOK_PAREN_OPEN;
    } else if (lastChar == ')') {
      lastChar = getc(fp);
      return TOK_PAREN_CLOSE;
    } else if (lastChar == '=') {
      lastChar = getc(fp);
      if (lastChar == '=') {
        lastChar = fgetc(fp);
        return TOK_EQUAL_TO;
      } else {
        return TOK_ASSIGNMENT;
      }
    } else if (lastChar == '<') {
      lastChar = getc(fp);
      if (lastChar == '<') {
        lastChar = fgetc(fp);
        return TOK_LEFT_SHIFT;
      } else {
        return TOK_LESS_THAN;
      }
    } else if (lastChar == '>') {
      lastChar = getc(fp);
      if (lastChar == '>') {
        lastChar = getc(fp);
        return TOK_RIGHT_SHIFT;
      } else {
        return TOK_GREATER_THAN;
      }
    } else if (lastChar == '"') {
      literal_string_ = "";
      do {
        lastChar = getc(fp);
        literal_string_ += lastChar;
      } while (lastChar != EOF && lastChar != '"');
      if (lastChar == EOF) {
        error_string_ = "Wrong literal format\n";
        return TOK_ERROR;
      }
      // eliminating the " at the end
      literal_string_ = literal_string_.substr(0, literal_string_.length() - 1);
      if (lastChar != EOF) lastChar = getc(fp);
      return TOK_LITERAL;

    } else if (lastChar == '\'') {
      literal_string_ = "";
      lastChar = fgetc(fp);
      literal_string_ += lastChar;
      if (lastChar == EOF) {
        error_string_ = "Wrong literal format\n";
        return TOK_ERROR;
      }
      lastChar = getc(fp);
      if (lastChar == '\'') {
        if (lastChar != EOF) lastChar = fgetc(fp);
        return TOK_CHARACTER;
      }
      error_string_ = "Wrong literal format\n";
      return TOK_ERROR;

    } else if (lastChar == '/') {
      lastChar = fgetc(fp);

      if (lastChar == '/') {
        // commented out line
        do {
          lastChar = getc(fp);
        } while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');

        if (lastChar != EOF) lastChar = fgetc(fp);
        return TOK_COMMENT;
      }
    }
  }

  error_string_ = "Unexpected Token\n";
  return TOK_ERROR;
}
}  // namespace jucc
