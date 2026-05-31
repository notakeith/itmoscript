#pragma once
#include "AST.h"
#include "Value.h"
#include "Environment.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

bool interpret(std::istream& input, std::ostream& output);

struct ReturnException {
    Value value;
    explicit ReturnException(Value v) : value(std::move(v)) {}
};

struct BreakException {};
struct ContinueException {};

class Interpreter {
public:
    Interpreter(std::ostream& out);
    void exec(const std::unique_ptr<AST::Stmt>& stmt);
    Value eval(const std::unique_ptr<AST::Expr>& expr);

    std::vector<std::string> call_stack;
private:
    std::ostream& out;
    std::shared_ptr<Environment> env;

    Value eval_expr(AST::Expr* expr);
    Value eval_binary(AST::BinaryExpr* expr);
    Value eval_unary(AST::UnaryExpr* expr);
    Value eval_variable(AST::VariableExpr* expr);
    Value eval_number(AST::NumberExpr* expr);
    Value eval_string(AST::StringExpr* expr);
    Value eval_bool(AST::BoolExpr* expr);
    Value eval_nil(AST::NilExpr* expr);
    Value eval_call(AST::CallExpr* expr);
    Value eval_index(AST::IndexExpr* expr);
    Value eval_slice(AST::SliceExpr* expr);
    Value eval_index_assignment(AST::IndexAssignmentExpr* expr);


    void exec_expr_stmt(AST::ExprStmt* stmt);
    void exec_assign_stmt(AST::AssignStmt* stmt);
    void exec_if_stmt(AST::IfStmt* stmt);
    void exec_while_stmt(AST::WhileStmt* stmt);
    void exec_for_stmt(AST::ForStmt* stmt);
    void exec_block_stmt(AST::BlockStmt* block);
    void exec_block(const std::vector<std::unique_ptr<AST::Stmt>>& stmts);


    Value eval_function(AST::FunctionExpr* expr);
};