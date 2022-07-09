#include "scanner.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

Scanner scanner;

void initScanner(char const *source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

static char advance() {
  scanner.current++;
  return scanner.current[-1];
}

static char peek() { return *scanner.current; }
static char peekNext() { return *(scanner.current + 1); }

static bool isAtEnd() { return *scanner.current == '\0'; }
static int currentTokenLength() { return scanner.current - scanner.start; }

static Token makeToken(TokenType type) {
  return (Token){.type = type,
                 .line = scanner.line,
                 .length = currentTokenLength(),
                 .start = scanner.start};
}

static Token errorToken(const char *errmsg) {
  return (Token){.type = TOKEN_ERROR,
                 .line = scanner.line,
                 .start = errmsg,
                 .length = (int)strlen(errmsg)};
}

static bool matchNext(const char c) {
  if (isAtEnd())
    return false;

  if (*scanner.current == c) {
    scanner.current++;
    return true;
  } else {
    return false;
  }
}

static bool isDigit(char c) { return c >= '0' && c <= '9'; }
static bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static Token consumeStringLiteral() {
  while (peek() != '"' && !isAtEnd()) {
    advance();
    if (peek() == '\n')
      scanner.line++;
  }

  if (isAtEnd())
    return errorToken("Unterminated string");
  // comsume closing "
  advance();
  return makeToken(TOKEN_STRING);
}

static Token consumeNumberLiteral() {
  while (isDigit(peek()))
    advance();

  if (peek() == '.' && isDigit(peekNext())) {
    advance();

    while (isDigit(peek()))
      advance();
  }

  return makeToken(TOKEN_NUMBER);
}

static TokenType matchKeyword(int start, int length, char const *rest,
                              TokenType type) {
  if (currentTokenLength() == start + length &&
      memcmp(scanner.start + start, rest, length)) {
    return type;
  } else {
    return TOKEN_IDENTIFIER;
  }
}

static TokenType resolveIdentifier() {
  switch (*scanner.start) {
  case 'a':
    return matchKeyword(1, 2, "nd", TOKEN_AND);
  case 'c':
    return matchKeyword(1, 4, "lass", TOKEN_CLASS);
  case 'e':
    return matchKeyword(1, 3, "lse", TOKEN_ELSE);
  case 'f':
    if (currentTokenLength() <= 1) {
      return TOKEN_IDENTIFIER;
    }
    switch (*(scanner.start + 1)) {
    case 'a':
      return matchKeyword(2, 3, "lse", TOKEN_FALSE);
    case 'o':
      return matchKeyword(2, 1, "r", TOKEN_FOR);
    case 'u':
      return matchKeyword(2, 3, "nct", TOKEN_FUNCT);
    default:
      return TOKEN_IDENTIFIER;
    }
  case 'i':
    return matchKeyword(1, 1, "f", TOKEN_IF);
  case 'n':
    return matchKeyword(1, 2, "il", TOKEN_NIL);
  case 'o':
    return matchKeyword(1, 1, "r", TOKEN_OR);
  case 'p':
    return matchKeyword(1, 4, "rint", TOKEN_PRINT);
  case 'r':
    return matchKeyword(1, 5, "eturn", TOKEN_RETURN);
  case 's':
    return matchKeyword(1, 4, "uper", TOKEN_SUPER);
  case 't':
    if (currentTokenLength() <= 1) {
      return TOKEN_IDENTIFIER;
    }
    switch (*(scanner.start + 1)) {
    case 'h':
      return matchKeyword(2, 2, "is", TOKEN_THIS);
    case 'r':
      return matchKeyword(2, 2, "ue", TOKEN_TRUE);
    default:
      return TOKEN_IDENTIFIER;
    }
  case 'v':
    return matchKeyword(1, 2, "ar", TOKEN_VAR);
  case 'w':
    return matchKeyword(1, 4, "hile", TOKEN_WHILE);
  default:
    return TOKEN_IDENTIFIER;
  }
}

static Token consumeIdentifier() {
  while ((isAlpha(peek()) || isDigit(peek())) && !isAtEnd())
    advance();
  return makeToken(resolveIdentifier());
}

static void skipWhitespace() {
  while (true) {
    switch (peek()) {
    case '\r':
    case '\t':
    case ' ':
      advance();
      break;
    case '\n':
      advance();
      scanner.line++;
      break;
    default:
      return;
    }
  }
}

static void skipComment() {
  while (peek() != '\n' && !isAtEnd()) {
    advance();
  }
  advance();
  scanner.line++;
}

Token scanToken() {
  skipWhitespace();
  scanner.start = scanner.current;

  if (isAtEnd())
    return makeToken(TOKEN_EOF);

  char c = advance();

  switch (c) {
  case '(':
    return makeToken(TOKEN_LEFT_PAREN);
  case ')':
    return makeToken(TOKEN_RIGHT_PAREN);
  case '{':
    return makeToken(TOKEN_LEFT_BRACE);
  case '}':
    return makeToken(TOKEN_RIGHT_BRACE);
  case ';':
    return makeToken(TOKEN_SEMICOLON);
  case ',':
    return makeToken(TOKEN_COMMA);
  case '.':
    return makeToken(TOKEN_DOT);
  case '-':
    return makeToken(TOKEN_MINUS);
  case '+':
    return makeToken(TOKEN_PLUS);
  case '/':
    if (matchNext('/')) {
      skipComment();
      break;
    } else
      return makeToken(TOKEN_SLASH);
  case '*':
    return makeToken(TOKEN_STAR);
  case '!':
    return matchNext('=') ? makeToken(TOKEN_BANG_EQUAL) : makeToken(TOKEN_BANG);
  case '=':
    return matchNext('=') ? makeToken(TOKEN_EQUAL_EQUAL)
                          : makeToken(TOKEN_EQUAL);
  case '>':
    return matchNext('=') ? makeToken(TOKEN_GREATER_EQUAL)
                          : makeToken(TOKEN_GREATER);
  case '<':
    return matchNext('=') ? makeToken(TOKEN_LESS_EQUAL) : makeToken(TOKEN_LESS);
  case '"':
    return consumeStringLiteral();
  default:
    if (isDigit(c))
      return consumeNumberLiteral();
    else if (isAlpha(c))
      return consumeIdentifier();
  }

  return errorToken("Unexpected character.");
}

