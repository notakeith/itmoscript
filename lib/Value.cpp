#include "Value.h"
#include <sstream>

double Value::as_num() const {
    if (is_num()) return std::get<double>(data);
    if (is_bool()) return std::get<bool>(data) ? 1.0 : 0.0;
    throw std::runtime_error("Value is not a number");
}

std::string& Value::as_str() {
    if (!is_str()) throw std::runtime_error("Value is not a string");
    return std::get<std::string>(data);
}

const std::string& Value::as_str() const {
    if (!is_str()) throw std::runtime_error("Value is not a string");
    return std::get<std::string>(data);
}

bool Value::as_bool() const {
    if (!is_bool()) throw std::runtime_error("Value is not a bool");
    return std::get<bool>(data);
}

List& Value::as_list() {
    if (!is_list()) throw std::runtime_error("Value is not a list");
    return *std::get<std::shared_ptr<List>>(data);
}

const List& Value::as_list() const {
    if (!is_list()) throw std::runtime_error("Value is not a list");
    return *std::get<std::shared_ptr<List>>(data);
}

Function& Value::as_func() {
    if (!is_func()) throw std::runtime_error("Value is not a function");
    return *std::get<std::shared_ptr<Function>>(data);
}

const Function& Value::as_func() const {
    if (!is_func()) throw std::runtime_error("Value is not a function");
    return *std::get<std::shared_ptr<Function>>(data);
}

std::string Value::to_string() const {
    std::ostringstream oss;
    switch (type) {
        case Type::NIL: return "nil";
        case Type::NUMBER: {
            double val = as_num();
            if (std::floor(val) == val) {
                oss << static_cast<long long>(val);
            } else {
                oss << std::defaultfloat << val;
            }
            return oss.str();
        }
        case Type::STRING: return as_str();
        case Type::BOOL: return as_bool() ? "true" : "false";
        case Type::LIST: {
            const auto& list = *std::get<std::shared_ptr<List>>(data);

            if (list.size() == 1) {
                return list[0].to_string();
            }

            std::string out = "[";
            for (size_t i = 0; i < list.size(); ++i) {
                out += list[i].to_string();
                if (i + 1 < list.size()) out += ", ";
            }
            out += "]";
            return out;
        }
        case Type::FUNCTION: return "[function]";
        default: return "[unknown]";
    }
}


bool operator==(const Value& a, const Value& b) {
    if (a.type != b.type) return false;

    switch (a.type) {
        case Value::Type::NIL: return true;
        case Value::Type::NUMBER: return a.as_num() == b.as_num();
        case Value::Type::STRING: return a.as_str() == b.as_str();
        case Value::Type::BOOL: return a.as_bool() == b.as_bool();
        case Value::Type::LIST: return a.as_list() == b.as_list();
        case Value::Type::FUNCTION: return a.data == b.data;
        default: return false;
    }
}

bool operator<(const Value& a, const Value& b) {
    if (a.type != b.type) throw std::runtime_error("Cannot compare different types");

    switch (a.type) {
        case Value::Type::NUMBER: return a.as_num() < b.as_num();
        case Value::Type::STRING: return a.as_str() < b.as_str();
        case Value::Type::BOOL: return a.as_num() < b.as_num();
        case Value::Type::LIST: return a.as_list() < b.as_list();
        default: throw std::runtime_error("Type not comparable with '<'");
    }
}
