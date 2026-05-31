#include "Value.h"
#include <iostream>

class Environment {
public:
    Environment(std::shared_ptr<Environment> parent = nullptr);

    Value get(const std::string& name);
    Value& get_ref(const std::string& name);
    void define(const std::string& name, const Value& val);
    void assign(const std::string& name, const Value& val);

private:
    std::unordered_map<std::string, Value> values;
    std::shared_ptr<Environment> parent;
};
