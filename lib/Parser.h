#pragma once
#include "Lexer.h"
#include "AST.h"

class Parser {
    Lexer& lexer;
    Token cur;

    void advance();
    void expect(TokenType type);
    void error(const std::string& msg);

    std::unique_ptr<AST::Expr> parseExpr();
    std::unique_ptr<AST::Expr> parseOr();
    std::unique_ptr<AST::Expr> parseAnd();
    std::unique_ptr<AST::Expr> parseEquality();
    std::unique_ptr<AST::Expr> parseComparison();
    std::unique_ptr<AST::Expr> parseAdditive();
    std::unique_ptr<AST::Expr> parseAdd();
    std::unique_ptr<AST::Expr> parseTerm();
    std::unique_ptr<AST::Expr> parsePower();
    std::unique_ptr<AST::Expr> parseUnary();
    std::unique_ptr<AST::Expr> parseFactor();

    std::unique_ptr<AST::Stmt> parseStatement();
    std::unique_ptr<AST::Stmt> parseAssignment();
    std::unique_ptr<AST::Stmt> parseIndexAssignment();
    std::unique_ptr<AST::Stmt> parseIf();
    std::unique_ptr<AST::Stmt> parseWhile();
    std::unique_ptr<AST::Stmt> parseFor();
    std::unique_ptr<AST::Stmt> parseReturn();

    bool isAssignOp(TokenType t);

    std::vector<std::unique_ptr<AST::Stmt>> parseBlock(TokenType closing, std::optional<TokenType> matchKeyword = std::nullopt);
public:
    Parser(Lexer& lexer);
    std::unique_ptr<AST::BlockStmt> parse();
};

