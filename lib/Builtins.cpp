#include "Builtins.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <random>
#include <list>
#include <algorithm>

void Builtins::register_all(Environment& env, std::ostream& out, Interpreter& interp) {
    env.define("print", Value(Function{
        [&out](const std::vector<Value>& args) {
            for (const auto& arg : args) out << arg.to_string();
            return Value();
        }
    }));

    env.define("println", Value(Function{
        [&out](const std::vector<Value>& args) {
            for (const auto& arg : args) out << arg.to_string();
            out << '\n';
            return Value();
        }
    }));
    env.define("to_string", Value(Function{ to_string }));
    env.define("abs", Value(Function{ abs }));
    env.define("ceil", Value(Function{ ceil }));
    env.define("floor", Value(Function{ floor }));
    env.define("round", Value(Function{ round }));
    env.define("sqrt", Value(Function{ sqrt }));
    env.define("rnd", Value(Function{ rnd }));
    env.define("parse_num", Value(Function{ parse_num }));
    env.define("len", Value(Function{ len }));
    env.define("lower", Value(Function{ lower }));
    env.define("upper", Value(Function{ upper }));
    env.define("split", Value(Function{ split }));
    env.define("join", Value(Function{ join }));
    env.define("replace", Value(Function{ replace }));
    env.define("range", Value(Function{ range }));
    env.define("push", Value(Function{ push }));
    env.define("pop", Value(Function{ pop }));
    env.define("insert", Value(Function{ insert }));
    env.define("remove", Value(Function{ remove }));
    env.define("sort", Value(Function{ sort }));
    env.define("read", Value(Function{ read }));

    env.define("stacktrace", Value(Function{
    [&interp](const std::vector<Value>&) {
        return Builtins::stacktrace(interp);
    }
}));

}

Value Builtins::print(const std::vector<Value>& args) {
    for (const auto& arg : args) std::cout << arg.to_string();
    return Value();
}

Value Builtins::println(const std::vector<Value>& args) {
    for (const auto& arg : args) std::cout << arg.to_string();
    std::cout << std::endl;
    return Value();
}

Value Builtins::to_string(const std::vector<Value>& args) {
    if (args.size() != 1) throw std::runtime_error("to_string expects 1 argument");
    return Value(args[0].to_string());
}

Value Builtins::abs(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].is_num()) throw std::runtime_error("abs expects 1 number");
    return Value(std::abs(args[0].as_num()));
}

Value Builtins::ceil(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].is_num()) throw std::runtime_error("ceil expects 1 number");
    return Value(std::ceil(args[0].as_num()));
}

Value Builtins::floor(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].is_num()) throw std::runtime_error("floor expects 1 number");
    return Value(std::floor(args[0].as_num()));
}

Value Builtins::round(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].is_num()) throw std::runtime_error("round expects 1 number");
    return Value(std::round(args[0].as_num()));
}

Value Builtins::sqrt(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].is_num()) throw std::runtime_error("sqrt expects 1 number");
    return Value(std::sqrt(args[0].as_num()));
}

Value Builtins::rnd(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].is_num()) throw std::runtime_error("rnd expects 1 number");
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, static_cast<int>(args[0].as_num()) - 1);
    return Value(static_cast<double>(dist(rng)));
}

Value Builtins::parse_num(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].is_str()) throw std::runtime_error("parse_num expects 1 string");
    try {
        return Value(std::stod(args[0].as_str()));
    } catch (...) {
        return Value();
    }
}

Value Builtins::len(const std::vector<Value>& args) {
    if (args.size() != 1) throw std::runtime_error("len expects 1 argument");
    if (args[0].is_str()) return Value(static_cast<double>(args[0].as_str().size()));
    if (args[0].is_list()) return Value(static_cast<double>(args[0].as_list().size()));
    throw std::runtime_error("len expects string or list");
}

Value Builtins::lower(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].is_str()) throw std::runtime_error("lower expects 1 string");
    std::string res = args[0].as_str();
    std::transform(res.begin(), res.end(), res.begin(), ::tolower);
    return Value(res);
}

Value Builtins::upper(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].is_str()) throw std::runtime_error("upper expects 1 string");
    std::string res = args[0].as_str();
    std::transform(res.begin(), res.end(), res.begin(), ::toupper);
    return Value(res);
}

Value Builtins::split(const std::vector<Value>& args) {
    if (args.size() != 2 || !args[0].is_str() || !args[1].is_str()) throw std::runtime_error("split expects 2 strings");
    std::string s = args[0].as_str(), delim = args[1].as_str();
    std::vector<Value> result;
    size_t start = 0, end;
    while ((end = s.find(delim, start)) != std::string::npos) {
        result.emplace_back(s.substr(start, end - start));
        start = end + delim.length();
    }
    result.emplace_back(s.substr(start));
    return Value(result);
}

Value Builtins::join(const std::vector<Value>& args) {
    if (args.size() != 2 || !args[0].is_list() || !args[1].is_str()) throw std::runtime_error("join expects list and delimiter string");
    const auto& list = args[0].as_list();
    std::string delim = args[1].as_str();
    std::ostringstream oss;
    for (size_t i = 0; i < list.size(); ++i) {
        if (i > 0) oss << delim;
        oss << list[i].to_string();
    }
    return Value(oss.str());
}

Value Builtins::replace(const std::vector<Value>& args) {
    if (args.size() != 3 || !args[0].is_str() || !args[1].is_str() || !args[2].is_str()) throw std::runtime_error("replace expects 3 strings");
    std::string s = args[0].as_str();
    std::string from = args[1].as_str();
    std::string to = args[2].as_str();
    size_t start = 0;
    while ((start = s.find(from, start)) != std::string::npos) {
        s.replace(start, from.length(), to);
        start += to.length();
    }
    return Value(s);
}

Value Builtins::range(std::vector<Value>& args) {
    if (args.size() < 2 || args.size() > 3 || !args[0].is_num() || !args[1].is_num())
        throw std::runtime_error("range expects 2 or 3 numeric arguments");
    double start = args[0].as_num();
    double end = args[1].as_num();
    double step = args.size() == 3 ? args[2].as_num() : 1.0;
    std::vector<Value> result;
    for (double i = start; (step > 0 ? i < end : i > end); i += step) {
        result.emplace_back(i);
    }
    return Value(result);
}

Value Builtins::push(std::vector<Value>& args) {
    if (args.size() != 2 || !args[0].is_list()) throw std::runtime_error("push expects list and value");
    args[0].as_list().push_back(args[1]);
    return Value();
}

Value Builtins::pop(std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].is_list()) throw std::runtime_error("pop expects 1 list");
    auto& list = args[0].as_list();
    if (list.empty()) throw std::runtime_error("pop from empty list");
    Value val = list.back();
    list.pop_back();
    return val;
}

Value Builtins::insert(std::vector<Value>& args) {
    if (args.size() != 3 || !args[0].is_list() || !args[1].is_num()) throw std::runtime_error("insert expects list, index, value");
    auto& list = args[0].as_list();
    int index = static_cast<int>(args[1].as_num());
    if (index < 0 || index > static_cast<int>(list.size())) throw std::runtime_error("index out of bounds");
    list.insert(list.begin() + index, args[2]);
    return list;
}

Value Builtins::remove(std::vector<Value>& args) {
    if (args.size() != 2 || !args[0].is_list() || !args[1].is_num()) throw std::runtime_error("remove expects list and index");
    auto& list = args[0].as_list();
    int index = static_cast<int>(args[1].as_num());
    if (index < 0 || index >= static_cast<int>(list.size())) throw std::runtime_error("index out of bounds");
    list.erase(list.begin() + index);
    return list;
}

Value Builtins::sort(std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].is_list()) throw std::runtime_error("sort expects 1 list");
    auto& list = args[0].as_list();
    std::sort(list.begin(), list.end(), [](const Value& a, const Value& b) {
        return a.to_string() < b.to_string();
    });
    return list;
}

Value Builtins::read(const std::vector<Value>& args) {
    if (!args.empty()) throw std::runtime_error("read expects no arguments");
    std::string line;
    std::getline(std::cin, line);
    return Value(line);
}

Value Builtins::stacktrace(const Interpreter& interp) {
    List result;
    for (const auto& frame : interp.call_stack) {
        result.emplace_back(frame);
    }
    return Value(result);
}
