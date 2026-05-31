#pragma once
#include "Value.h"
#include "Interpreter.h"

class Builtins {
public:
    static void register_all(Environment& env, std::ostream& out, Interpreter& interp);

private:
    // Системные
    static Value print(const std::vector<Value>&);
    static Value println(const std::vector<Value>&);
    static Value read(const std::vector<Value>&);
    static Value stacktrace(const Interpreter& interp);

    // Преобразование
    static Value to_string(const std::vector<Value>&);
    static Value parse_num(const std::vector<Value>&);

    // Числовые
    static Value abs(const std::vector<Value>&);
    static Value ceil(const std::vector<Value>&);
    static Value floor(const std::vector<Value>&);
    static Value round(const std::vector<Value>&);
    static Value sqrt(const std::vector<Value>&);
    static Value rnd(const std::vector<Value>&);

    // Строковые
    static Value len(const std::vector<Value>&);
    static Value lower(const std::vector<Value>&);
    static Value upper(const std::vector<Value>&);
    static Value split(const std::vector<Value>&);
    static Value join(const std::vector<Value>&);
    static Value replace(const std::vector<Value>&);

    // Списковые
    static Value range(std::vector<Value>&);
    static Value push(std::vector<Value>&);
    static Value pop(std::vector<Value>&);
    static Value insert(std::vector<Value>&);
    static Value remove(std::vector<Value>&);
    static Value sort(std::vector<Value>&);
};
