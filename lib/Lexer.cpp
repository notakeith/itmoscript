#include "Lexer.h"
#include <cctype>
#include <stdexcept>

const std::unordered_map<std::string,TokenType> Lexer::double_tok = {
    {"==", TokenType::EQ},          {"!=", TokenType::NEQ},
    {"<=", TokenType::LE},          {">=", TokenType::GE},
    {"+=", TokenType::PLUS_ASSIGN}, {"-=", TokenType::MINUS_ASSIGN},
    {"*=", TokenType::MUL_ASSIGN},  {"/=", TokenType::DIV_ASSIGN},
    {"%=", TokenType::MOD_ASSIGN},  {"^=", TokenType::POW_ASSIGN}
};

const std::unordered_map<char,TokenType> Lexer::single_tok = {
    {'+', TokenType::PLUS},   {'-', TokenType::MINUS},
    {'*', TokenType::MUL},    {'/', TokenType::DIV},
    {'%', TokenType::MOD},    {'^', TokenType::POW},
    {'=', TokenType::ASSIGN}, {'<', TokenType::LT},
    {'>', TokenType::GT},     {';', TokenType::SEMI},
    {',', TokenType::COMMA},  {':', TokenType::COLON},
    {'(', TokenType::LPAREN}, {')', TokenType::RPAREN},
    {'[', TokenType::LBRACK}, {']', TokenType::RBRACK}
};

const std::unordered_map<std::string,TokenType> Lexer::keywords = {
    {"true",     TokenType::TRUE},     {"false",    TokenType::FALSE},
    {"nil",      TokenType::NIL},      {"and",      TokenType::AND},
    {"or",       TokenType::OR},       {"not",      TokenType::NOT},
    {"if",       TokenType::IF},       {"then",     TokenType::THEN},
    {"else",     TokenType::ELSE},     {"elseif",   TokenType::ELSEIF},
    {"end",      TokenType::END},      {"while",    TokenType::WHILE},
    {"for",      TokenType::FOR},      {"in",       TokenType::IN},
    {"function", TokenType::FUNCTION}, {"return",   TokenType::RETURN},
    {"break",    TokenType::BREAK},    {"continue", TokenType::CONTINUE}
};

Lexer::Lexer(const std::string& src) : input(src) {
    advance();
}

Token Lexer::peek(size_t offset) {
    while (lookahead_buffer.size() < offset) {
        Token next = get_next_token();
        lookahead_buffer.push_back(next);
    }
    return lookahead_buffer[offset - 1];
}


inline void Lexer::advance() {
    if (pos < input.size()) {
        current = input[pos++];
        if (current == '\n') {
            lineno++;
            column = 0;
        } else {
            column++;
        }
    } else {
        current = '\0';
    }
}

inline char Lexer::char_peek() const {
    if (pos < input.size()) {
        return input[pos];
    } else {
        return '\0';
    }
}

void Lexer::skip_whitespace() {
    while (current && std::isspace(current)) {
        advance();
    }
}

void Lexer::skip_comment() {
    advance();
    advance();
    while (current && current != '\n') {
        advance();
    }
}

void Lexer::error(const std::string& msg) const {
    throw std::runtime_error("Lexer error at " +
        std::to_string(lineno) + ":" + std::to_string(column) +
        ": " + msg);
}

Token Lexer::number() {
    size_t start_col = column;
    std::string num;
    while (current && (std::isdigit(current) || current == '.' ||
           current=='e' || current=='E' ||
          (num.size() > 0 && (current == '+' || current == '-') &&
          (num.back() == 'e' || num.back() == 'E')))) {
        num += current;
        advance();
    }
    return { TokenType::NUMBER, num, lineno, start_col };
}

Token Lexer::ident_or_keyword() {
    size_t start_col = column;
    std::string id;
    while (current && (std::isalnum(current) || current == '_')) {
        id += current;
        advance();
    }
    auto it = keywords.find(id);
    TokenType tt = it!=keywords.end() ? it->second : TokenType::IDENT;
    return { tt, id, lineno, start_col };
}

Token Lexer::string_literal() {
    size_t start_col = column;
    std::string s;
    advance();
    while (current && current != '"') {
        if (current == '\\') {
            advance();
            switch (current) {
                case 'n': s += '\n'; break;
                case 't': s += '\t'; break;
                case 'b': s+= '\b'; break;
                case 'r': s+= '\r'; break;
                case 'a': s+= '\a'; break;
                case '\'': s += '\''; break;
                case '"': s += '"'; break;
                case '?': s+= '?'; break;
                case '\\': s+= '\\'; break;
                case 'f': s+= '\f'; break;
                case 'v': s+= '\v'; break;
                case '0': s+= '\0'; break;
                default: error("invalid escape sequence \\" + std::string(1,current));
            }
        } else {
            s += current;
        }
        advance();
    }
    if (!current) error("unclosed string literal");
    advance();
    return { TokenType::STRING, s, lineno, start_col };
}

Token Lexer::get_next_token() {
   if (!lookahead_buffer.empty()) {
        Token token = lookahead_buffer.front();
        lookahead_buffer.erase(lookahead_buffer.begin());
        return token;
    }

    while (current) {
        if (std::isspace(current)) { skip_whitespace(); continue; }
        if (current=='/' && char_peek()== '/') { skip_comment(); continue; }

        size_t start_col = column;

        std::string two { current, char_peek() };
        auto it2 = double_tok.find(two);
        if (it2 != double_tok.end()) {
            advance(); advance();
            return { it2->second, two, lineno, start_col };
        }

        auto it1 = single_tok.find(current);
        if (it1 != single_tok.end()) {
            std::string lex(1, current);
            TokenType tt = it1->second;
            advance();
            return { tt, lex, lineno, start_col };
        }

        if (std::isdigit(current) ||
           (current=='.' && std::isdigit(char_peek()))) {
            return number();
        }

        if (std::isalpha(current) || current=='_') {
            return ident_or_keyword();
        }

        if (current=='"') {
            return string_literal();
        }

        if (current) {
            std::string bad(1, current);
            error("unexpected character '"+bad+"'");
        }
    }

    return { TokenType::END_OF_FILE, "", lineno, column };
}
