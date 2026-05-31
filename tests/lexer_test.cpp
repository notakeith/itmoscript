#include "gtest/gtest.h"
#include "Lexer.h"

auto test_lexer(const std::string& src) {
    Lexer lexer(src);
    return lexer.get_next_token();
}

TEST(LexerTest, Token_PLUS) {
    auto token = test_lexer("+");
    EXPECT_EQ(token.type, TokenType::PLUS);
    EXPECT_EQ(token.lexeme, "+");
}

TEST(LexerTest, Token_MINUS) {
    auto token = test_lexer("-");
    EXPECT_EQ(token.type, TokenType::MINUS);
    EXPECT_EQ(token.lexeme, "-");
}

TEST(LexerTest, Token_MUL) {
    auto token = test_lexer("*");
    EXPECT_EQ(token.type, TokenType::MUL);
    EXPECT_EQ(token.lexeme, "*");
}

TEST(LexerTest, Token_DIV) {
    auto token = test_lexer("/");
    EXPECT_EQ(token.type, TokenType::DIV);
    EXPECT_EQ(token.lexeme, "/");
}

TEST(LexerTest, Token_MOD) {
    auto token = test_lexer("%");
    EXPECT_EQ(token.type, TokenType::MOD);
    EXPECT_EQ(token.lexeme, "%");
}

TEST(LexerTest, Token_POW) {
    auto token = test_lexer("^");
    EXPECT_EQ(token.type, TokenType::POW);
    EXPECT_EQ(token.lexeme, "^");
}

TEST(LexerTest, Token_ASSIGN) {
    auto token = test_lexer("=");
    EXPECT_EQ(token.type, TokenType::ASSIGN);
    EXPECT_EQ(token.lexeme, "=");
}

TEST(LexerTest, Token_LT) {
    auto token = test_lexer("<");
    EXPECT_EQ(token.type, TokenType::LT);
    EXPECT_EQ(token.lexeme, "<");
}

TEST(LexerTest, Token_GT) {
    auto token = test_lexer(">");
    EXPECT_EQ(token.type, TokenType::GT);
    EXPECT_EQ(token.lexeme, ">");
}

TEST(LexerTest, Token_SEMI) {
    auto token = test_lexer(";");
    EXPECT_EQ(token.type, TokenType::SEMI);
    EXPECT_EQ(token.lexeme, ";");
}

TEST(LexerTest, Token_COMMA) {
    auto token = test_lexer(",");
    EXPECT_EQ(token.type, TokenType::COMMA);
    EXPECT_EQ(token.lexeme, ",");
}

TEST(LexerTest, Token_COLON) {
    auto token = test_lexer(":");
    EXPECT_EQ(token.type, TokenType::COLON);
    EXPECT_EQ(token.lexeme, ":");
}

TEST(LexerTest, Token_LPAREN) {
    auto token = test_lexer("(");
    EXPECT_EQ(token.type, TokenType::LPAREN);
    EXPECT_EQ(token.lexeme, "(");
}

TEST(LexerTest, Token_RPAREN) {
    auto token = test_lexer(")");
    EXPECT_EQ(token.type, TokenType::RPAREN);
    EXPECT_EQ(token.lexeme, ")");
}

TEST(LexerTest, Token_LBRACK) {
    auto token = test_lexer("[");
    EXPECT_EQ(token.type, TokenType::LBRACK);
    EXPECT_EQ(token.lexeme, "[");
}

TEST(LexerTest, Token_RBRACK) {
    auto token = test_lexer("]");
    EXPECT_EQ(token.type, TokenType::RBRACK);
    EXPECT_EQ(token.lexeme, "]");
}

TEST(LexerTest, Token_EQ) {
    auto token = test_lexer("==");
    EXPECT_EQ(token.type, TokenType::EQ);
    EXPECT_EQ(token.lexeme, "==");
}

TEST(LexerTest, Token_NEQ) {
    auto token = test_lexer("!=");
    EXPECT_EQ(token.type, TokenType::NEQ);
    EXPECT_EQ(token.lexeme, "!=");
}

TEST(LexerTest, Token_LE) {
    auto token = test_lexer("<=");
    EXPECT_EQ(token.type, TokenType::LE);
    EXPECT_EQ(token.lexeme, "<=");
}

TEST(LexerTest, Token_GE) {
    auto token = test_lexer(">=");
    EXPECT_EQ(token.type, TokenType::GE);
    EXPECT_EQ(token.lexeme, ">=");
}

TEST(LexerTest, Token_PLUS_ASSIGN) {
    auto token = test_lexer("+=");
    EXPECT_EQ(token.type, TokenType::PLUS_ASSIGN);
    EXPECT_EQ(token.lexeme, "+=");
}

TEST(LexerTest, Token_MINUS_ASSIGN) {
    auto token = test_lexer("-=");
    EXPECT_EQ(token.type, TokenType::MINUS_ASSIGN);
    EXPECT_EQ(token.lexeme, "-=");
}

TEST(LexerTest, Token_MUL_ASSIGN) {
    auto token = test_lexer("*=");
    EXPECT_EQ(token.type, TokenType::MUL_ASSIGN);
    EXPECT_EQ(token.lexeme, "*=");
}

TEST(LexerTest, Token_DIV_ASSIGN) {
    auto token = test_lexer("/=");
    EXPECT_EQ(token.type, TokenType::DIV_ASSIGN);
    EXPECT_EQ(token.lexeme, "/=");
}

TEST(LexerTest, Token_MOD_ASSIGN) {
    auto token = test_lexer("%=");
    EXPECT_EQ(token.type, TokenType::MOD_ASSIGN);
    EXPECT_EQ(token.lexeme, "%=");
}

TEST(LexerTest, Token_POW_ASSIGN) {
    auto token = test_lexer("^=");
    EXPECT_EQ(token.type, TokenType::POW_ASSIGN);
    EXPECT_EQ(token.lexeme, "^=");
}
typedef std::pair<const char*, TokenType> Kw;

#define TEST_KEYWORD(literal, kwType)                          \
TEST(LexerTest, Token_##kwType) {                             \
    auto token = test_lexer(literal);                               \
    EXPECT_EQ(token.type, TokenType::kwType);                   \
    EXPECT_EQ(token.lexeme, std::string(literal));              \
}

TEST_KEYWORD("true", TRUE);
TEST_KEYWORD("false", FALSE);
TEST_KEYWORD("nil", NIL);
TEST_KEYWORD("and", AND);
TEST_KEYWORD("or", OR);
TEST_KEYWORD("not", NOT);
TEST_KEYWORD("if", IF);
TEST_KEYWORD("then", THEN);
TEST_KEYWORD("else", ELSE);
TEST_KEYWORD("elseif", ELSEIF);
TEST_KEYWORD("end", END);
TEST_KEYWORD("while", WHILE);
TEST_KEYWORD("for", FOR);
TEST_KEYWORD("in", IN);
TEST_KEYWORD("function", FUNCTION);
TEST_KEYWORD("return", RETURN);
TEST_KEYWORD("break", BREAK);
TEST_KEYWORD("continue", CONTINUE);

TEST(LexerTest, Token_IDENT) {
    auto token = test_lexer("abc_123");
    EXPECT_EQ(token.type, TokenType::IDENT);
    EXPECT_EQ(token.lexeme, "abc_123");
}

TEST(LexerTest, Token_NUMBER_Integer) {
    auto token = test_lexer("12345");
    EXPECT_EQ(token.type, TokenType::NUMBER);
    EXPECT_EQ(token.lexeme, "12345");
}

TEST(LexerTest, Token_NUMBER_Float) {
    auto token = test_lexer("3.14");
    EXPECT_EQ(token.type, TokenType::NUMBER);
    EXPECT_EQ(token.lexeme, "3.14");
}

TEST(LexerTest, Token_NUMBER_Exponent) {
    auto token = test_lexer("1e-10");
    EXPECT_EQ(token.type, TokenType::NUMBER);
    EXPECT_EQ(token.lexeme, "1e-10");
}

TEST(LexerTest, Token_STRING) {
    auto token = test_lexer("\"hello\\nworld\"");
    EXPECT_EQ(token.type, TokenType::STRING);
    EXPECT_EQ(token.lexeme, "hello\nworld");
}

TEST(LexerTest, Token_EOF) {
    Lexer lexer("");
    auto token = lexer.get_next_token();
    EXPECT_EQ(token.type, TokenType::END_OF_FILE);
    EXPECT_EQ(token.lexeme, "");
}
