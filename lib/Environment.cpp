#include "Environment.h"

Environment::Environment(std::shared_ptr<Environment> parent) : parent(std::move(parent)) {}

Value Environment::get(const std::string& name) {
    if (values.count(name)) return values[name];
    if (parent) return parent->get(name);
    throw std::runtime_error("Undefined variable: " + name);
}

Value& Environment::get_ref(const std::string& name) {
    if (values.count(name)) {
        return values[name];
    } else if (parent) {
        return parent->get_ref(name);
    }
    throw std::runtime_error("Undefined variable: " + name);
}

void Environment::define(const std::string& name, const Value& val) {
    values[name] = val;
}

void Environment::assign(const std::string& name, const Value& val) {
    if (values.count(name)) {
        values[name] = val;
        return;
    }
    if (parent) {
        parent->assign(name, val);
        return;
    }
    throw std::runtime_error("Undefined variable: " + name);
}
