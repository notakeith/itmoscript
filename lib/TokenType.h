#pragma once
#include <string>

enum class TokenType {
    // — Литералы —
    IDENT,        // идентификатор
    NUMBER,       // числовой литерал
    STRING,       // строковый литерал
    TRUE,FALSE,   // булевы литералы
    NIL,          // nil

    // — Арифметические операторы —
    PLUS,         // +
    MINUS,        // -
    MUL,          // *
    DIV,          // /
    MOD,          // %
    POW,          // ^

    // — Операторы присваивания —
    ASSIGN,       // =
    PLUS_ASSIGN,  // +=
    MINUS_ASSIGN, // -=
    MUL_ASSIGN,   // *=
    DIV_ASSIGN,   // /=
    MOD_ASSIGN,   // %=
    POW_ASSIGN,   // ^=

    // — Операторы сравнения —
    EQ, NEQ,      // ==, !=
    LT, GT, LE, GE, // <, >, <=, >=

    // — Логические операторы —
    AND, OR, NOT, // and, or, not

    // — Разделители и пунктуация —
    LPAREN, RPAREN,   // ( )
    LBRACK, RBRACK,   // [ ]
    COMMA, COLON, SEMI, // , : ;

    // — Ключевые слова управления потоком —
    IF, THEN, ELSE, ELSEIF,
    WHILE, FOR, IN, END,
    FUNCTION, RETURN,
    BREAK, CONTINUE,

    // — Прочие —
    END_OF_FILE,  // конец файла
    UNKNOWN       // неизвестная лексема
};

struct Token {
    TokenType type;
    std::string lexeme;
    size_t      line;
    size_t      column;
};


static const std::unordered_map<TokenType, std::string> tokenNames = {
    {TokenType::IDENT,        "TokenType::IDENT"},
    {TokenType::NUMBER,       "TokenType::NUMBER"},
    {TokenType::STRING,       "TokenType::STRING"},
    {TokenType::TRUE,         "TokenType::TRUE"},
    {TokenType::FALSE,        "TokenType::FALSE"},
    {TokenType::NIL,          "TokenType::NIL"},
    {TokenType::PLUS,         "TokenType::PLUS"},
    {TokenType::MINUS,        "TokenType::MINUS"},
    {TokenType::MUL,          "TokenType::MUL"},
    {TokenType::DIV,          "TokenType::DIV"},
    {TokenType::MOD,          "TokenType::MOD"},
    {TokenType::POW,          "TokenType::POW"},
    {TokenType::ASSIGN,       "TokenType::ASSIGN"},
    {TokenType::PLUS_ASSIGN,  "TokenType::PLUS_ASSIGN"},
    {TokenType::MINUS_ASSIGN, "TokenType::MINUS_ASSIGN"},
    {TokenType::MUL_ASSIGN,   "TokenType::MUL_ASSIGN"},
    {TokenType::DIV_ASSIGN,   "TokenType::DIV_ASSIGN"},
    {TokenType::MOD_ASSIGN,   "TokenType::MOD_ASSIGN"},
    {TokenType::POW_ASSIGN,   "TokenType::POW_ASSIGN"},
    {TokenType::EQ,           "TokenType::EQ"},
    {TokenType::NEQ,          "TokenType::NEQ"},
    {TokenType::LT,           "TokenType::LT"},
    {TokenType::GT,           "TokenType::GT"},
    {TokenType::LE,           "TokenType::LE"},
    {TokenType::GE,           "TokenType::GE"},
    {TokenType::AND,          "TokenType::AND"},
    {TokenType::OR,           "TokenType::OR"},
    {TokenType::NOT,          "TokenType::NOT"},
    {TokenType::LPAREN,       "TokenType::LPAREN"},
    {TokenType::RPAREN,       "TokenType::RPAREN"},
    {TokenType::LBRACK,       "TokenType::LBRACK"},
    {TokenType::RBRACK,       "TokenType::RBRACK"},
    {TokenType::COMMA,        "TokenType::COMMA"},
    {TokenType::COLON,        "TokenType::COLON"},
    {TokenType::SEMI,         "TokenType::SEMI"},
    {TokenType::IF,           "TokenType::IF"},
    {TokenType::THEN,         "TokenType::THEN"},
    {TokenType::ELSE,         "TokenType::ELSE"},
    {TokenType::ELSEIF,       "TokenType::ELSEIF"},
    {TokenType::WHILE,        "TokenType::WHILE"},
    {TokenType::FOR,          "TokenType::FOR"},
    {TokenType::IN,           "TokenType::IN"},
    {TokenType::FUNCTION,     "TokenType::FUNCTION"},
    {TokenType::RETURN,       "TokenType::RETURN"},
    {TokenType::BREAK,        "TokenType::BREAK"},
    {TokenType::CONTINUE,     "TokenType::CONTINUE"},
    {TokenType::END,          "TokenType::END"},
    {TokenType::END_OF_FILE,  "TokenType::END_OF_FILE"},
    {TokenType::UNKNOWN,      "TokenType::UNKNOWN"}
};

// std::string tokenTypeToString(TokenType token) {
//     return tokenNames.at(token);
// }