#pragma once
#include <memory>
#include <string>
#include <vector>

namespace AST {

struct Node {
    virtual ~Node() = default;
};

struct Expr : Node {};

struct NumberExpr : Expr {
    double value;
    NumberExpr(double val) : value(val) {}
};

struct StringExpr : Expr {
    std::string value;
    StringExpr(const std::string& val) : value(val) {}
};

struct BoolExpr : Expr {
    bool value;
    BoolExpr(bool val) : value(val) {}
};

struct NilExpr : Expr {};

struct VariableExpr : Expr {
    std::string name;
    VariableExpr(const std::string& id) : name(id) {}
};

struct BinaryExpr : Expr {
    std::string op;
    std::unique_ptr<Expr> left, right;
    BinaryExpr(std::string op, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : op(std::move(op)), left(std::move(l)), right(std::move(r)) {}
};

struct UnaryExpr : Expr {
    std::string op;
    std::unique_ptr<Expr> expr;
    UnaryExpr(std::string op, std::unique_ptr<Expr> expr)
        : op(std::move(op)), expr(std::move(expr)) {}
};

struct ListExpr : Expr {
    std::vector<std::unique_ptr<Expr>> elements;
    ListExpr(std::vector<std::unique_ptr<Expr>> elems)
        : elements(std::move(elems)) {}
};

struct IndexExpr : Expr {
    std::unique_ptr<Expr> list;
    std::unique_ptr<Expr> index;
    IndexExpr(std::unique_ptr<Expr> list, std::unique_ptr<Expr> index)
        : list(std::move(list)), index(std::move(index)) {}
};

class IndexAssignmentExpr : public Expr {
public:
    std::unique_ptr<Expr> object;
    std::unique_ptr<Expr> index;
    std::unique_ptr<Expr> value;

    IndexAssignmentExpr(std::unique_ptr<Expr> object, std::unique_ptr<Expr> index, std::unique_ptr<Expr> value)
        : object(std::move(object)), index(std::move(index)), value(std::move(value)) {}
};

struct SliceExpr : Expr {
    std::unique_ptr<Expr> list;
    std::unique_ptr<Expr> start;
    std::unique_ptr<Expr> end;
    SliceExpr(std::unique_ptr<Expr> list, std::unique_ptr<Expr> start, std::unique_ptr<Expr> end)
        : list(std::move(list)), start(std::move(start)), end(std::move(end)) {}
};

struct CallExpr : Expr {
    std::unique_ptr<Expr> callee;
    std::vector<std::unique_ptr<Expr>> args;
    CallExpr(std::unique_ptr<Expr> callee, std::vector<std::unique_ptr<Expr>> args)
        : callee(std::move(callee)), args(std::move(args)) {}
};

struct FunctionExpr : Expr {
    std::vector<std::string> params;
    std::unique_ptr<Node> body;
    std::string name;
    FunctionExpr(std::vector<std::string> params, std::unique_ptr<Node> body)
        : params(std::move(params)), body(std::move(body)) {}

    FunctionExpr(std::string name, std::vector<std::string> params, std::unique_ptr<Node> body)
        : name(name), params(std::move(params)), body(std::move(body)) {}
};

struct Stmt : Node {};

struct ExprStmt : Stmt {
    std::unique_ptr<Expr> expr;
    ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}
};

struct AssignStmt : Stmt {
    std::string name;
    std::string op;
    std::unique_ptr<Expr> value;
    AssignStmt(std::string name, std::string op, std::unique_ptr<Expr> value)
        : name(std::move(name)), op(std::move(op)), value(std::move(value)) {}
};

struct IfStmt : Stmt {
    struct Branch {
        std::unique_ptr<Expr> condition;
        std::vector<std::unique_ptr<Stmt>> body;
    };
    std::vector<Branch> branches;
    std::vector<std::unique_ptr<Stmt>> else_body;
};

struct WhileStmt : Stmt {
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Stmt>> body;
    WhileStmt(std::unique_ptr<Expr> cond, std::vector<std::unique_ptr<Stmt>> body)
        : condition(std::move(cond)), body(std::move(body)) {}
};

struct ForStmt : Stmt {
    std::string var;
    std::unique_ptr<Expr> iterable;
    std::vector<std::unique_ptr<Stmt>> body;
    ForStmt(std::string var, std::unique_ptr<Expr> iterable, std::vector<std::unique_ptr<Stmt>> body)
        : var(std::move(var)), iterable(std::move(iterable)), body(std::move(body)) {}
};

struct ReturnStmt : Stmt {
    std::unique_ptr<Expr> value;
    ReturnStmt(std::unique_ptr<Expr> value) : value(std::move(value)) {}
};

struct BreakStmt : Stmt {};
struct ContinueStmt : Stmt {};

struct BlockStmt : Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;
    BlockStmt() = default;

    BlockStmt(std::vector<std::unique_ptr<Stmt>> stmts) : statements(std::move(stmts)) {}
};

struct EmptyStmt : Stmt {};

}