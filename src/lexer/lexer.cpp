#include "lexer/lexer.h"

namespace jucc {

int GetToken(FILE *fp) {
  static int lastChar = ' ';

  while (lastChar != EOF && isspace(lastChar)) lastChar = getc(fp);

  // check for identifier and literal tokens

  // RE : [a-zA-Z][0-9a-zA-Z]

  if (isalpha(lastChar)) {
    identifier_string = lastChar;
    while (lastChar != EOF && isalnum(lastChar = getc(fp))) identifier_string += lastChar;
    if (identifier_string == "int")
      return TOK_INT;
    else if (identifier_string == "float")
      return TOK_FLOAT;
    else if (identifier_string == "void")
      return TOK_VOID;

    // conditionals
    else if (identifier_string == "if")
      return TOK_IF;
    else if (identifier_string == "else")
      return TOK_ELSE;

    // cout , cin
    else if (identifier_string == "cout")
      return TOK_COUT;
    else if (identifier_string == "cin")
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
        error_string = "Wrong fractional number format : " + numString + "\n";
        return TOK_ERROR;
      }

      lastChar = getc(fp);
    }
    if (isalpha(lastChar)) {
      error_string = "Wrong fractional number format : " + numString + "\n";
      return TOK_ERROR;
    }

    // fractional number
    if (dot_count) {
      floatval = strtod(numString.c_str(), nullptr);
      return TOK_FRACTIONAL;
    } else {
      intval = (static_cast<int>(strtod(numString.c_str(), nullptr)));
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
      literal_string = "";
      do {
        lastChar = getc(fp);
        literal_string += lastChar;
      } while (lastChar != EOF && lastChar != '"');
      if (lastChar == EOF) {
        error_string = "Wrong literal format\n";
        return TOK_ERROR;
      }
      // eliminating the " at the end
      literal_string = literal_string.substr(0, literal_string.length() - 1);
      if (lastChar != EOF) lastChar = getc(fp);
      return TOK_LITERAL;

    } else if (lastChar == '\'') {
      literal_string = "";
      lastChar = fgetc(fp);
      literal_string += lastChar;
      if (lastChar == EOF) {
        error_string = "Wrong literal format\n";
        return TOK_ERROR;
      }
      lastChar = getc(fp);
      if (lastChar == '\'') {
        if (lastChar != EOF) lastChar = fgetc(fp);
        return TOK_CHARACTER;
      }
      error_string = "Wrong literal format\n";
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

  error_string = "Unexpected Token\n";
  return TOK_ERROR;
}
}  // namespace jucc
