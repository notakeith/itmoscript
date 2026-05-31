#include "Lexer.h"
#include "Parser.h"
#include "Builtins.h"
#include <stdexcept>
#include <iostream>
#include "Interpreter.h"

Interpreter::Interpreter(std::ostream& out) : env(std::make_shared<Environment>()), out(out) {
    Builtins::register_all(*env, out, *this);
}

Value Interpreter::eval_index(AST::IndexExpr* expr) {
    Value collection = eval(expr->list);
    int idx = static_cast<int>(eval(expr->index).as_num());

    if (collection.is_str()) {
        const std::string& s = collection.as_str();
        if (idx < 0) idx += s.size();
        if (idx < 0 || idx >= (int)s.size())
            throw std::runtime_error("String index out of bounds");
        return Value(std::string(1, s[idx]));
    }

    if (collection.is_list()) {
        const auto& list = collection.as_list();
        if (idx < 0) idx += list.size();
        if (idx < 0 || idx >= (int)list.size())
            throw std::runtime_error("List index out of bounds");
        return list[idx];
    }

    throw std::runtime_error("Indexing only allowed for strings or lists");
}


Value Interpreter::eval_index_assignment(AST::IndexAssignmentExpr* expr) {
    if (auto var_expr = dynamic_cast<AST::VariableExpr*>(expr->object.get())) {
        Value& collection = env->get_ref(var_expr->name);
        int idx = static_cast<int>(eval(expr->index).as_num());
        Value new_value = eval(expr->value);

        if (collection.is_str()) {
            std::string& s = collection.as_str();
            if (idx < 0) idx += s.size();
            if (idx < 0 || idx >= (int)s.size())
                throw std::runtime_error("String index out of bounds");
            if (!new_value.is_str() || new_value.as_str().size() != 1)
                throw std::runtime_error("String index assignment requires single character");
            s[idx] = new_value.as_str()[0];
            return new_value;
        }

        if (collection.is_list()) {
            auto& list = collection.as_list();
            if (idx < 0) idx += list.size();
            if (idx < 0 || idx >= (int)list.size())
                throw std::runtime_error("List index out of bounds");
            list[idx] = new_value;
            return new_value;
        }

        throw std::runtime_error("Index assignment only allowed for strings or lists");
    }
    throw std::runtime_error("Can only assign to variable index");
}

Value Interpreter::eval_slice(AST::SliceExpr* expr) {
    Value collection = eval(expr->list);
    int sz;

    if (collection.is_str()) {
        const std::string& s = collection.as_str();
        sz = s.size();

        int start = expr->start ? static_cast<int>(eval(expr->start).as_num()) : 0;
        int end = expr->end ? static_cast<int>(eval(expr->end).as_num()) : sz;

        if (start < 0) start += sz;
        if (end < 0) end += sz;

        start = std::clamp(start, 0, sz);
        end = std::clamp(end, 0, sz);

        return Value(s.substr(start, end - start));
    }

    if (collection.is_list()) {
        const auto& list = collection.as_list();
        sz = list.size();

        int start = expr->start ? static_cast<int>(eval(expr->start).as_num()) : 0;
        int end = expr->end ? static_cast<int>(eval(expr->end).as_num()) : sz;

        if (start < 0) start += sz;
        if (end < 0) end += sz;

        start = std::clamp(start, 0, sz);
        end = std::clamp(end, 0, sz);

        return Value(List(list.begin() + start, list.begin() + end));
    }

    throw std::runtime_error("Slicing only allowed for strings or lists");
}



void Interpreter::exec(const std::unique_ptr<AST::Stmt>& stmt) {
    if (auto x = dynamic_cast<AST::ExprStmt*>(stmt.get())) exec_expr_stmt(x);
    else if (auto x = dynamic_cast<AST::AssignStmt*>(stmt.get())) exec_assign_stmt(x);
    else if (auto x = dynamic_cast<AST::IfStmt*>(stmt.get())) exec_if_stmt(x);
    else if (auto x = dynamic_cast<AST::WhileStmt*>(stmt.get())) exec_while_stmt(x);
    else if (auto x = dynamic_cast<AST::ForStmt*>(stmt.get())) exec_for_stmt(x);
    else if (auto x = dynamic_cast<AST::BlockStmt*>(stmt.get())) exec_block_stmt(x);
    else if (auto x = dynamic_cast<AST::ReturnStmt*>(stmt.get())) throw ReturnException(eval(x->value));
    else if (auto x = dynamic_cast<AST::BreakStmt*>(stmt.get())) throw BreakException();
    else if (auto x = dynamic_cast<AST::ContinueStmt*>(stmt.get())) throw ContinueException();
    else throw std::runtime_error("Unsupported statement");
}

Value Interpreter::eval(const std::unique_ptr<AST::Expr>& expr) {
    return eval_expr(expr.get());
}

Value Interpreter::eval_expr(AST::Expr* expr) {
    if (auto l = dynamic_cast<AST::ListExpr*>(expr)) {
        std::vector<Value> vals;
        for (const auto& e : l->elements) {
            vals.push_back(eval(e));
        }
        return Value(vals);
    }
    if (auto i = dynamic_cast<AST::IndexExpr*>(expr)) return eval_index(i);
    if (auto s = dynamic_cast<AST::SliceExpr*>(expr)) return eval_slice(s);
    if (auto ia = dynamic_cast<AST::IndexAssignmentExpr*>(expr)) return eval_index_assignment(ia);
    if (auto f = dynamic_cast<AST::FunctionExpr*>(expr)) return eval_function(f);
    if (auto n = dynamic_cast<AST::NumberExpr*>(expr)) return eval_number(n);
    if (auto s = dynamic_cast<AST::StringExpr*>(expr)) return eval_string(s);
    if (auto b = dynamic_cast<AST::BoolExpr*>(expr)) return eval_bool(b);
    if (auto n = dynamic_cast<AST::NilExpr*>(expr)) return eval_nil(n);
    if (auto v = dynamic_cast<AST::VariableExpr*>(expr)) return eval_variable(v);
    if (auto u = dynamic_cast<AST::UnaryExpr*>(expr)) return eval_unary(u);
    if (auto b = dynamic_cast<AST::BinaryExpr*>(expr)) return eval_binary(b);
    if (auto c = dynamic_cast<AST::CallExpr*>(expr)) return eval_call(c);
    throw std::runtime_error("Unknown expression");
}

Value Interpreter::eval_number(AST::NumberExpr* expr) {
    return Value(expr->value);
}

Value Interpreter::eval_string(AST::StringExpr* expr) {
    return Value(expr->value);
}

Value Interpreter::eval_bool(AST::BoolExpr* expr) {
    return Value(expr->value);
}

Value Interpreter::eval_nil(AST::NilExpr*) {
    return Value();
}

Value Interpreter::eval_variable(AST::VariableExpr* expr) {
    return env->get(expr->name);
}

Value Interpreter::eval_unary(AST::UnaryExpr* expr) {
    Value right = eval(expr->expr);
    if (expr->op == "-") {
        if (!right.is_num()) throw std::runtime_error("Unary - expects number");
        return Value(-right.as_num());
    } else if (expr->op == "+") {
        if (!right.is_num()) throw std::runtime_error("Unary + expects number");
        return Value(+right.as_num());
    } else if (expr->op == "not") {
        return Value(!right.as_bool());
    }
    throw std::runtime_error("Unknown unary operator: " + expr->op);
}

Value Interpreter::eval_binary(AST::BinaryExpr* expr) {
    Value left = eval(expr->left);
    Value right = eval(expr->right);
    const std::string& op = expr->op;

    if (op == "+") {
        if ((left.is_num() || left.is_bool()) && (right.is_num() || right.is_bool()))
            return Value(left.as_num() + right.as_num());
        if (left.is_str() && right.is_str())
            return Value(left.as_str() + right.as_str());
        if (left.is_list() && right.is_list()) {
            const auto& a = left.as_list();
            const auto& b = right.as_list();
            List result = a;
            result.insert(result.end(), b.begin(), b.end());
            return Value(result);
        }
        if (left.is_list()) {
            List result = left.as_list();
            result.push_back(right);
            return Value(result);
        }

        if (right.is_list()) {
            List result = { left };
            const auto& b = right.as_list();
            result.insert(result.end(), b.begin(), b.end());
            return Value(result);
        }

        throw std::runtime_error("Invalid operands for '+'");
    }

    if (op == "-") {
        if (left.is_num() && right.is_num()) return Value(left.as_num() - right.as_num());

        if (left.is_str() && right.is_str()) {
            const std::string& base = left.as_str();
            const std::string& suffix = right.as_str();
            if (base.size() >= suffix.size() &&
                base.compare(base.size() - suffix.size(), suffix.size(), suffix) == 0) {
                return Value(base.substr(0, base.size() - suffix.size()));
            }
            return Value(base);
        }

        if (left.is_list() && right.is_list()) {
            List result = left.as_list();
            const auto& remove = right.as_list();
            for (const auto& item : remove) {
                auto it = std::find(result.begin(), result.end(), item);
                if (it != result.end()) result.erase(it);
            }
            return Value(result);
        }


        throw std::runtime_error("Invalid operands for '-'");
    }
if (op == "*") {
    if (left.is_str()) {
        double times = right.as_num();
        if (times < 0) throw std::runtime_error("Can't repeat string negative times");
        const std::string& str = left.as_str();
        double full_repeats = std::floor(times);
        double fraction = times - full_repeats;
        std::string result;
        for (int i = 0; i < static_cast<int>(full_repeats); ++i) {
            result += str;
        }
        if (fraction > 0 && !str.empty()) {
            size_t partial_chars = static_cast<size_t>(std::round(str.size() * fraction));
            if (partial_chars > 0) {
                result += str.substr(0, partial_chars);
            }
        }
        return Value(result);
    }

    if (right.is_str()) {
        double times = left.as_num();
        if (times < 0) throw std::runtime_error("Can't repeat string negative times");
        const std::string& str = right.as_str();
        double full_repeats = std::floor(times);
        double fraction = times - full_repeats;
        std::string result;
        for (int i = 0; i < static_cast<int>(full_repeats); ++i) {
            result += str;
        }
        if (fraction > 0 && !str.empty()) {
            size_t partial_chars = static_cast<size_t>(std::round(str.size() * fraction));
            if (partial_chars > 0) {
                result += str.substr(0, partial_chars);
            }
        }
        return Value(result);
    }

        if (left.is_num() && right.is_num()) {
            return Value(left.as_num() * right.as_num());
        }

        if (left.is_list() && right.is_num()) {
            int repeat = static_cast<int>(std::floor(right.as_num()));
            if (repeat < 0) throw std::runtime_error("Can't repeat list negative times");
            const auto& list = left.as_list();
            List result;
            for (int i = 0; i < repeat; ++i)
                result.insert(result.end(), list.begin(), list.end());
            return Value(result);
        }

        if (left.is_num() && right.is_list()) {
            int repeat = static_cast<int>(std::floor(left.as_num()));
            if (repeat < 0) throw std::runtime_error("Can't repeat list negative times");
            const auto& list = right.as_list();
            List result;
            for (int i = 0; i < repeat; ++i)
                result.insert(result.end(), list.begin(), list.end());
            return Value(result);
        }

        throw std::runtime_error("Operator '*' not supported for given types");
    }


    if (op == "/") {
        if (left.is_num() && right.is_num()) {
            if (right.as_num() == 0) throw std::runtime_error("Division by zero");
            return Value(left.as_num() / right.as_num());
        }
        throw std::runtime_error("Invalid operands for '/'");
    }

    if (op == "%") {
        if (!left.is_num() || !right.is_num())
            throw std::runtime_error("Operator '%' requires two numbers");
        return Value(std::fmod(left.as_num(), right.as_num()));
    }

    if (op == "==") return Value(left == right);
    if (op == "!=") return Value(!(left == right));

    if (op == "<" || op == "<=" || op == ">" || op == ">=") {
        if (left.is_num() && right.is_num()) {
            double l = left.as_num(), r = right.as_num();
            if (op == "<") return Value(l < r);
            if (op == "<=") return Value(l <= r);
            if (op == ">") return Value(l > r);
            if (op == ">=") return Value(l >= r);
        }

        if (left.is_str() && right.is_str()) {
            const auto& l = left.as_str();
            const auto& r = right.as_str();
            if (op == "<") return Value(l < r);
            if (op == "<=") return Value(l <= r);
            if (op == ">") return Value(l > r);
            if (op == ">=") return Value(l >= r);
        }

        if (left.is_list() && right.is_list()) {
            const auto& a = left.as_list();
            const auto& b = right.as_list();
            if (op == "<") return Value(a < b);
            if (op == "<=") return Value(a <= b);
            if (op == ">") return Value(a > b);
            if (op == ">=") return Value(a >= b);
        }

        throw std::runtime_error("Invalid operands for comparison operator: " + op);
    }

    if (op == "^") {
        if (!left.is_num() || !right.is_num())
            throw std::runtime_error("Operator '^' requires numbers");
        return Value(std::pow(left.as_num(), right.as_num()));
    }

    if (op == "and") {
        if (!eval(expr->left).as_bool()) return Value(false);
        return Value(eval(expr->right).as_bool());
    }
    if (op == "or") {
        if (eval(expr->left).as_bool()) return Value(true);
        return Value(eval(expr->right).as_bool());
    }


    throw std::runtime_error("Unknown binary operator: " + op);
}

void Interpreter::exec_expr_stmt(AST::ExprStmt* stmt) {
    eval(stmt->expr);
}

void Interpreter::exec_assign_stmt(AST::AssignStmt* stmt) {
    Value val = eval(stmt->value);
    if (stmt->op == "=") {
        try {
            env->assign(stmt->name, val);
        } catch (...) {
            env->define(stmt->name, val);
        }
    } else if (stmt->op == "+=" || stmt->op == "-=" || stmt->op == "*=" || stmt->op == "/=") {
        Value old = env->get(stmt->name);
        Value delta = eval(stmt->value);

        if (stmt->op == "+=") {
            if (old.is_num() && delta.is_num()) {
                env->assign(stmt->name, Value(old.as_num() + delta.as_num()));
                return;
            }
            if (old.is_str() && delta.is_str()) {
                env->assign(stmt->name, Value(old.as_str() + delta.as_str()));
                return;
            }
            if (old.is_list()) {
                auto& list = old.as_list();
                if (delta.is_list()) {
                    const auto& other = delta.as_list();
                    list.insert(list.end(), other.begin(), other.end());
                } else {
                    list.push_back(delta);
                }
                return;
            }
        }

        if (stmt->op == "-=" && old.is_num() && delta.is_num()) {
            env->assign(stmt->name, Value(old.as_num() - delta.as_num()));
            return;
        }

        if (stmt->op == "*=") {
            if (old.is_num() && delta.is_num()) {
                env->assign(stmt->name, Value(old.as_num() * delta.as_num()));
                return;
            }
            if (old.is_str()) {
                int repeat = static_cast<int>(std::floor(delta.as_num()));
                if (repeat < 0) throw std::runtime_error("Can't repeat string negative times");
                std::string result;
                for (int i = 0; i < repeat; ++i) result += old.as_str();
                env->assign(stmt->name, Value(result));
                return;
            }
            if (old.is_list()) {
                int repeat = static_cast<int>(std::floor(delta.as_num()));
                if (repeat < 0) throw std::runtime_error("Can't repeat list negative times");
                auto& list = old.as_list();
                List result;
                for (int i = 0; i < repeat; ++i)
                    result.insert(result.end(), list.begin(), list.end());
                env->assign(stmt->name, Value(result));
                return;
            }
        }

        if (stmt->op == "/=" && old.is_num() && delta.is_num()) {
            if (delta.as_num() == 0)
                throw std::runtime_error("Division by zero in '/='");
            env->assign(stmt->name, Value(old.as_num() / delta.as_num()));
            return;
        }

        if (stmt->op == "%=" && old.is_num() && delta.is_num()) {
            env->assign(stmt->name, Value(std::fmod(old.as_num(), delta.as_num())));
            return;
        }

        throw std::runtime_error("Unsupported '" + stmt->op + "' operation for given types");
    } else {
        throw std::runtime_error("Unsupported assignment operator: " + stmt->op);
    }
}

Value Interpreter::eval_call(AST::CallExpr* expr) {
    Value callee = eval(expr->callee);

    if (!callee.is_func()) throw std::runtime_error("Attempt to call non-function");

    std::vector<Value> args;
    for (const auto& arg : expr->args) {
        args.push_back(eval(arg));
    }

    return callee.as_func()(args);
}

void Interpreter::exec_if_stmt(AST::IfStmt* stmt) {
    for (const auto& branch : stmt->branches) {
        if (eval(branch.condition).as_bool()) {
            exec_block(branch.body);
            return;
        }
    }
    exec_block(stmt->else_body);
}


void Interpreter::exec_while_stmt(AST::WhileStmt* stmt) {
    while (eval(stmt->condition).as_bool()) {
        try {
            exec_block(stmt->body);
        } catch (const ContinueException&) {
            continue;
        } catch (const BreakException&) {
            break;
        }
    }
}

void Interpreter::exec_for_stmt(AST::ForStmt* stmt) {
    Value iterable = eval(stmt->iterable);
    if (!iterable.is_list()) throw std::runtime_error("for loop expects list");

    for (const auto& val : iterable.as_list()) {
        auto previous = env;
        auto loop_env = std::make_shared<Environment>(previous);
        loop_env->define(stmt->var, val);
        env = loop_env;

        try {
            exec_block(stmt->body);
        } catch (const ContinueException&) {
            env = previous;
            continue;
        } catch (const BreakException&) {
            env = previous;
            break;
        }

        env = previous;
    }
}


void Interpreter::exec_block_stmt(AST::BlockStmt* block) {
    exec_block(block->statements);
}

void Interpreter::exec_block(const std::vector<std::unique_ptr<AST::Stmt>>& stmts) {
    auto previous = env;
    env = std::make_shared<Environment>(previous);
    try {
        for (const auto& stmt : stmts) {
            exec(stmt);
        }
    } catch (...) {
        env = previous;
        throw;
    }
    env = previous;
}


bool interpret(std::istream& input, std::ostream& output) {
    try {
        std::string code((std::istreambuf_iterator<char>(input)),
                          std::istreambuf_iterator<char>());

        Lexer lexer(code);
        Parser parser(lexer);
        auto ast = parser.parse();

        Interpreter interpreter(output);
        for (const auto& stmt : ast->statements) {
            interpreter.exec(stmt);
        }

        return true;
    } catch (const std::exception& e) {
        output << "Runtime error: " << e.what() << '\n';
        return false;
    }
}

Value Interpreter::eval_function(AST::FunctionExpr* expr) {
    auto func = [=,this](std::vector<Value> args) -> Value {
        if (args.size() != expr->params.size())
            throw std::runtime_error("Argument count mismatch in function call");


        call_stack.push_back(expr->name);

        auto previous = env;
        env = std::make_shared<Environment>(previous);

        for (size_t i = 0; i < args.size(); ++i) {
            env->define(expr->params[i], args[i]);
        }

        try {
            for (const auto& stmt : static_cast<AST::BlockStmt*>(expr->body.get())->statements) {
                exec(stmt);
            }
        } catch (const ReturnException& ret) {
            env = previous;
            call_stack.pop_back();
            return ret.value;
        }

        env = previous;
        call_stack.pop_back();
        return Value();
    };

    return Value(Function(func));
}