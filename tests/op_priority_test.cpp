#include <gtest/gtest.h>
#include <sstream>
#include "Interpreter.h"

void run_test(const std::string& code, const std::string& expected) {
    std::istringstream input(code);
    std::ostringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(OperatorsTest, ArithmeticOperators) {
    run_test("print(1 + 2 * 3)", "7");
    run_test("print((1 + 2) * 3)", "9");
    run_test("print(10 / 2)", "5");
    run_test("print(7 % 4)", "3");
    run_test("print(2 ^ 3)", "8");
}

TEST(OperatorsTest, UnaryOperators) {
    run_test("print(-5)", "-5");
    run_test("print(+5)", "5");
    run_test("print(not false)", "true");
}
TEST(OperatorsTest, StringOperators) {
    run_test("print(\"a\" + \"b\")", "ab");
    run_test("print(\"a\" * 3)", "aaa");
    run_test("print(\"abc\" - \"c\")", "ab");
}

TEST(OperatorsTest, ListOperators) {
    run_test(R"(
        a = [1]
        a += 2
        print(a))", "[1, 2]");
    run_test("print([1,2] + [3,4])", "[1, 2, 3, 4]");
    run_test("print([\"a\"] * 3)", "[a, a, a]");
}

TEST(OperatorsTest, ComparisonOperators) {
    run_test("print(1 == 1)", "true");
    run_test("print(1 != 2)", "true");
    run_test("print(3 > 2)", "true");
    run_test("print(3 >= 3)", "true");
    run_test("print(1 < 2)", "true");
    run_test("print(2 <= 2)", "true");
    run_test("print(\"abc\" < \"abd\")", "true");
    run_test("print([1,2] == [1,2])", "true");
}

TEST(OperatorsTest, LogicAndPriority) {
    run_test("print(not true and false)", "false");
    run_test("print(true or false and false)", "true");
    run_test("print(not false or false and true)", "true");
    run_test("print((not false) and (false or true))", "true");
}

TEST(OperatorsTest, FunctionCall) {
    std::string code = R"(
        funcs = [
            function() return 1 end function,
            function() return 2 end function
        ]
        print(funcs[0]())
        print(funcs[1]())
    )";
    run_test(code, "12");
}

TEST(OperatorsTest, SliceAndIndex) {
    run_test("print(\"abc\"[1])", "b");
    run_test("print(\"abcdef\"[2:5])", "cde");
    run_test("print([10, 20, 30][1])", "20");
    run_test("print([1,2,3,4][1:3])", "[2, 3]");
}