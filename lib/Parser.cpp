#include "Parser.h"
#include <iostream>
#include <sstream>

Parser::Parser(Lexer& lexer) : lexer(lexer) {
    advance();
}

void Parser::advance() {
    cur = lexer.get_next_token();
}

void Parser::expect(TokenType type) {
    if (cur.type != type)
        error("Expected token: " + tokenNames.at(type));
    advance();
}

void Parser::error(const std::string& msg) {
    std::ostringstream oss;
    oss << "Parser error at line " << cur.line << ", column " << cur.column << ": " << msg
        << "\nCurrent token: '" << cur.lexeme << "' (" << tokenNames.at(cur.type) << ")";
    throw std::runtime_error(oss.str());
}

bool Parser::isAssignOp(TokenType t) {
    switch (t) {
        case TokenType::ASSIGN:
        case TokenType::PLUS_ASSIGN:
        case TokenType::MINUS_ASSIGN:
        case TokenType::MUL_ASSIGN:
        case TokenType::DIV_ASSIGN:
        case TokenType::MOD_ASSIGN:
        case TokenType::POW_ASSIGN:
            return true;
        default:
            return false;
    }
}

std::vector<std::unique_ptr<AST::Stmt>> Parser::parseBlock(TokenType closing, std::optional<TokenType> matchKeyword) {
    std::vector<std::unique_ptr<AST::Stmt>> body;
    while (cur.type != closing && cur.type != TokenType::END_OF_FILE) {
        body.push_back(parseStatement());
    }
    expect(closing);
    if (matchKeyword) {
        expect(*matchKeyword);
    }
    return body;
}



std::unique_ptr<AST::Expr> Parser::parseExpr() {
    return parseOr();
}

std::unique_ptr<AST::Expr> Parser::parseOr() {
    auto left = parseAnd();
    while (cur.type == TokenType::OR) {
        std::string op = cur.lexeme;
        advance();
        auto right = parseAnd();
        left = std::make_unique<AST::BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<AST::Expr> Parser::parseAnd() {
    auto left = parseComparison();
    while (cur.type == TokenType::AND) {
        std::string op = cur.lexeme;
        advance();
        auto right = parseComparison();
        left = std::make_unique<AST::BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<AST::Expr> Parser::parseComparison() {
    auto left = parseAdditive();
    while (cur.type == TokenType::EQ || cur.type == TokenType::NEQ ||
           cur.type == TokenType::LT || cur.type == TokenType::LE ||
           cur.type == TokenType::GT || cur.type == TokenType::GE) {
        std::string op = cur.lexeme;
        advance();
        auto right = parseAdditive();
        left = std::make_unique<AST::BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<AST::Expr> Parser::parseAdditive() {
    auto left = parseTerm();
    while (cur.type == TokenType::PLUS || cur.type == TokenType::MINUS) {
        std::string op = cur.lexeme;
        advance();
        auto right = parseTerm();
        left = std::make_unique<AST::BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<AST::Expr> Parser::parseTerm() {
    auto left = parsePower();
    while (cur.type == TokenType::MUL || cur.type == TokenType::DIV ||
           cur.type == TokenType::MOD) {
        std::string op = cur.lexeme;
        advance();
        auto right = parsePower();
        left = std::make_unique<AST::BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<AST::Expr> Parser::parsePower() {
    auto left = parseUnary();

    while (true) {
        if (cur.type == TokenType::POW) {
            std::string op = cur.lexeme;
            advance();
            auto right = parsePower();
            left = std::make_unique<AST::BinaryExpr>(op, std::move(left), std::move(right));
        } else if (cur.type == TokenType::LBRACK) {
            advance();
            std::unique_ptr<AST::Expr> start, end;
            bool is_slice = false;

            if (cur.type != TokenType::COLON && cur.type != TokenType::RBRACK) {
                start = parseExpr();
            }

            if (cur.type == TokenType::COLON) {
                is_slice = true;
                advance();
                if (cur.type != TokenType::RBRACK)
                    end = parseExpr();
            }

            expect(TokenType::RBRACK);

            if (is_slice) {
                left = std::make_unique<AST::SliceExpr>(
                    std::move(left),
                    std::move(start),
                    std::move(end)
                );
            } else if (start) {
                left = std::make_unique<AST::IndexExpr>(
                    std::move(left),
                    std::move(start)
                );
            } else {
                error("Expected index or slice expression");
            }

        } else if (cur.type == TokenType::LPAREN) {
            advance();
            std::vector<std::unique_ptr<AST::Expr>> args;
            if (cur.type != TokenType::RPAREN) {
                args.push_back(parseExpr());
                while (cur.type == TokenType::COMMA) {
                    advance();
                    args.push_back(parseExpr());
                }
            }
            expect(TokenType::RPAREN);
            left = std::make_unique<AST::CallExpr>(std::move(left), std::move(args));
        } else {
            break;
        }
    }

    return left;
}


std::unique_ptr<AST::Expr> Parser::parseUnary() {
    if (cur.type == TokenType::PLUS || cur.type == TokenType::MINUS || cur.type == TokenType::NOT) {
        std::string op = cur.lexeme;
        advance();
        return std::make_unique<AST::UnaryExpr>(op, parseUnary());
    }
    return parseFactor();
}


std::unique_ptr<AST::Expr> Parser::parseFactor() {
    if (cur.type == TokenType::PLUS || cur.type == TokenType::MINUS || cur.type == TokenType::NOT) {
        std::string op = cur.lexeme;
        advance();
        auto expr = parseFactor();
        return std::make_unique<AST::UnaryExpr>(op, std::move(expr));
    }

    if (cur.type == TokenType::NUMBER) {
        size_t parsed_len = 0;
        double val;
        try {
            val = std::stod(cur.lexeme, &parsed_len);
        } catch (const std::exception& e) {
            error("Invalid numeric literal: " + cur.lexeme);
        }

        if (parsed_len != cur.lexeme.size()) {
            error("Invalid numeric literal: " + cur.lexeme);
        }

        advance();
        return std::make_unique<AST::NumberExpr>(val);
    }


    if (cur.type == TokenType::STRING) {
        std::string val = cur.lexeme;
        advance();
        return std::make_unique<AST::StringExpr>(val);
    }

    if (cur.type == TokenType::TRUE || cur.type == TokenType::FALSE) {
        bool val = (cur.type == TokenType::TRUE);
        advance();
        return std::make_unique<AST::BoolExpr>(val);
    }

    if (cur.type == TokenType::NIL) {
        advance();
        return std::make_unique<AST::NilExpr>();
    }

    if (cur.type == TokenType::IDENT) {
        std::string id = cur.lexeme;
        advance();
        return std::make_unique<AST::VariableExpr>(id);
    }

    if (cur.type == TokenType::FUNCTION) {
        advance();
        expect(TokenType::LPAREN);

        std::vector<std::string> params;
        if (cur.type != TokenType::RPAREN) {
            if (cur.type != TokenType::IDENT)
                error("Expected parameter name in function definition");
            params.push_back(cur.lexeme);
            advance();
            while (cur.type == TokenType::COMMA) {
                advance();
                if (cur.type != TokenType::IDENT)
                    error("Expected parameter name after ','");
                params.push_back(cur.lexeme);
                advance();
            }
        }

        expect(TokenType::RPAREN);

        auto body = parseBlock(TokenType::END, TokenType::FUNCTION);

        return std::make_unique<AST::FunctionExpr>(
            std::move(params),
            std::make_unique<AST::BlockStmt>(std::move(body))
        );
    }

    if (cur.type == TokenType::LBRACK) {
        advance();
        std::vector<std::unique_ptr<AST::Expr>> elems;

        if (cur.type != TokenType::RBRACK) {
        while (true) {
            elems.push_back(parseExpr());

            if (cur.type == TokenType::COMMA) {
                advance();
                if (cur.type == TokenType::RBRACK) break;
            } else {
                break;
            }
        }
    }
    expect(TokenType::RBRACK);
    return std::make_unique<AST::ListExpr>(std::move(elems));
    }



    if (cur.type == TokenType::LPAREN) {
        advance();
        auto expr = parseExpr();
        expect(TokenType::RPAREN);
        return expr;
    }

    error("Unexpected token in factor");
    return nullptr;
}


std::unique_ptr<AST::Stmt> Parser::parseStatement() {
    switch (cur.type) {
        case TokenType::IF:        return parseIf();
        case TokenType::WHILE:     return parseWhile();
        case TokenType::FOR:       return parseFor();
        case TokenType::RETURN:    return parseReturn();

        case TokenType::IDENT: {
            Token next = lexer.peek();
            if (isAssignOp(next.type)) {
                return parseAssignment();
            } else if (next.type == TokenType::LBRACK) {
                return parseIndexAssignment();
            }
            return std::make_unique<AST::ExprStmt>(parseExpr());
        }

        case TokenType::BREAK:
            advance();
            return std::make_unique<AST::BreakStmt>();

        case TokenType::CONTINUE:
            advance();
            return std::make_unique<AST::ContinueStmt>();

        case TokenType::NUMBER:
        case TokenType::STRING:
        case TokenType::TRUE:
        case TokenType::FALSE:
        case TokenType::NIL:
        case TokenType::LBRACK:
        case TokenType::LPAREN:
            return std::make_unique<AST::ExprStmt>(parseExpr());

        default:
            error("Unexpected token at top-level: " + tokenNames.at(cur.type));
            return std::make_unique<AST::EmptyStmt>();
    }
}

std::unique_ptr<AST::Stmt> Parser::parseAssignment() {
    std::string var = cur.lexeme;
    advance();

    if (!isAssignOp(cur.type))
        error("Expected assignment operator after identifier");
    std::string op = cur.lexeme;
    advance();

    if (cur.type == TokenType::FUNCTION) {
        advance();

        expect(TokenType::LPAREN);
        std::vector<std::string> params;
        if (cur.type != TokenType::RPAREN) {
            params.push_back(cur.lexeme);
            advance();
            while (cur.type == TokenType::COMMA) {
                advance();
                params.push_back(cur.lexeme);
                advance();
            }
        }
        expect(TokenType::RPAREN);

        auto body = parseBlock(TokenType::END, TokenType::FUNCTION);

        auto func_expr = std::make_unique<AST::FunctionExpr>(
            var,
            std::move(params),
            std::make_unique<AST::BlockStmt>(std::move(body))
        );

        return std::make_unique<AST::AssignStmt>(
            var,
            "=",
            std::move(func_expr)
        );
    }

    auto value = parseExpr();
    return std::make_unique<AST::AssignStmt>(var, op, std::move(value));
}

std::unique_ptr<AST::Stmt> Parser::parseIndexAssignment() {
    auto expr = parseExpr();

    if (auto index_expr = dynamic_cast<AST::IndexExpr*>(expr.get())) {
        if (isAssignOp(cur.type)) {
            std::string op = cur.lexeme;
            advance();
            auto value = parseExpr();

            return std::make_unique<AST::ExprStmt>(
                std::make_unique<AST::IndexAssignmentExpr>(
                    std::move(index_expr->list),
                    std::move(index_expr->index),
                    std::move(value)
                )
            );
        }
    }

    return std::make_unique<AST::ExprStmt>(std::move(expr));
}


std::unique_ptr<AST::Stmt> Parser::parseIf() {
    advance();
    auto cond = parseExpr();
    expect(TokenType::THEN);

    std::vector<std::unique_ptr<AST::Stmt>> then_branch;
    while (cur.type != TokenType::ELSE && cur.type != TokenType::END) {
        then_branch.push_back(parseStatement());
    }

    std::vector<AST::IfStmt::Branch> branches;
    branches.push_back({ std::move(cond), std::move(then_branch) });

    std::vector<std::unique_ptr<AST::Stmt>> else_branch;

    while (cur.type == TokenType::ELSE) {
        advance();
        if (cur.type == TokenType::IF) {
            advance();
            auto elif_cond = parseExpr();
            expect(TokenType::THEN);

            std::vector<std::unique_ptr<AST::Stmt>> elif_branch;
            while (cur.type != TokenType::ELSE && cur.type != TokenType::END) {
                elif_branch.push_back(parseStatement());
            }

            branches.push_back({ std::move(elif_cond), std::move(elif_branch) });
        } else {
            while (cur.type != TokenType::END) {
                else_branch.push_back(parseStatement());
            }
            break;
        }
    }

    expect(TokenType::END);
    expect(TokenType::IF);

    auto stmt = std::make_unique<AST::IfStmt>();
    stmt->branches = std::move(branches);
    stmt->else_body = std::move(else_branch);
    return stmt;
}


std::unique_ptr<AST::Stmt> Parser::parseWhile() {
    advance();
    auto cond = parseExpr();
    auto body = parseBlock(TokenType::END, TokenType::WHILE);
    return std::make_unique<AST::WhileStmt>(std::move(cond), std::move(body));
}


std::unique_ptr<AST::Stmt> Parser::parseFor() {
    advance();
    if (cur.type != TokenType::IDENT)
        error("Expected identifier after 'for'");
    std::string var = cur.lexeme;
    advance();
    expect(TokenType::IN);
    auto iterable = parseExpr();
    auto body = parseBlock(TokenType::END, TokenType::FOR);
    return std::make_unique<AST::ForStmt>(
        std::move(var), std::move(iterable), std::move(body)
    );
}


std::unique_ptr<AST::Stmt> Parser::parseReturn() {
    advance();
    auto value = parseExpr();
    return std::make_unique<AST::ReturnStmt>(std::move(value));
}


std::unique_ptr<AST::BlockStmt> Parser::parse() {
    auto block = std::make_unique<AST::BlockStmt>();
    while (cur.type != TokenType::END_OF_FILE) {
        block->statements.push_back(parseStatement());
    }
    return block;
}