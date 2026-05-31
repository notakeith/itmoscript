#pragma once
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

struct Value;

using List = std::vector<Value>;
using Function = std::function<Value(std::vector<Value>&)>;

struct Value {
    enum class Type { NIL,
                      NUMBER,
                      STRING,
                      BOOL,
                      LIST,
                      FUNCTION };

    Type type;

    std::variant<
        std::monostate,            // nil
        double,                    // number
        std::string,               // string
        bool,                      // bool
        std::shared_ptr<List>,     // list
        std::shared_ptr<Function>  // function
    > data;

    Value() : type(Type::NIL), data(std::monostate{}) {}
    Value(double num) : type(Type::NUMBER), data(num) {}
    Value(const std::string& str) : type(Type::STRING), data(str) {}
    Value(bool b) : type(Type::BOOL), data(b) {}
    Value(const List& list) : type(Type::LIST), data(std::make_shared<List>(list)) {}
    Value(const Function& fn) : type(Type::FUNCTION), data(std::make_shared<Function>(fn)) {}

    bool is_nil() const { return type == Type::NIL; }
    bool is_num() const { return type == Type::NUMBER; }
    bool is_str() const { return type == Type::STRING; }
    bool is_bool() const { return type == Type::BOOL; }
    bool is_list() const { return type == Type::LIST; }
    bool is_func() const { return type == Type::FUNCTION; }

    double as_num() const;
    bool as_bool() const;
    std::string& as_str();
    const std::string& as_str() const;
    List& as_list();
    const List& as_list() const;
    Function& as_func();
    const Function& as_func() const;

    std::string to_string() const;
};

bool operator==(const Value& lhs, const Value& rhs);
bool operator<(const Value& lhs, const Value& rhs);