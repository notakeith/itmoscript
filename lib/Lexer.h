#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "TokenType.h"

class Lexer {
public:
    Lexer(const std::string& src);

    Token get_next_token();

    Token peek(size_t offset=1);

private:
    const std::string input;
    size_t pos = 0;
    size_t lineno = 1;
    size_t column = 0;
    char  current = '\0';

    std::vector<Token> lookahead_buffer;

    static const std::unordered_map<std::string,TokenType> double_tok;
    static const std::unordered_map<char, TokenType> single_tok;
    static const std::unordered_map<std::string,TokenType> keywords;

private:
    inline void advance();
    inline char char_peek() const;
    void error(const std::string& msg) const;

    void skip_whitespace();
    void skip_comment();

    Token number();
    Token ident_or_keyword();
    Token string_literal();
};
