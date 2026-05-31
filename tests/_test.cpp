#include <gtest/gtest.h>
#include <sstream>
#include "Interpreter.h"

void run(const std::string& code, const std::string& expected_output) {
    std::istringstream input(code);
    std::ostringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected_output);
}

TEST(InterpreterTest, Arithmetic) {
    run("print(2 + 3)", "5");
    run("print(2 * 3 + 4)", "10");
    run("print(2 + 3 * 4)", "14");
    run("print(2 ^ 3 ^ 2)", "512");
}

TEST(InterpreterTest, LogicPriority) {
    run("print(not false)", "true");
    run("print(true and false)", "false");
    run("print(true or false)", "true");
    run("print(not true or false)", "false");
}

TEST(InterpreterTest, StringConcatSubtractRepeat) {
    run("print(\"ab\" + \"cd\")", "abcd");
    run("print(\"abcdef\" - \"def\")", "abc");
    run("print(\"a\" * 3)", "aaa");
    run("print(3 * \"a\")", "aaa");
}

TEST(InterpreterTest, ListOps) {
    run("print([1, 2] + [3, 4])", "[1, 2, 3, 4]");
    run("print([1, 2] * 2)", "[1, 2, 1, 2]");
    run("print(2 * [3])", "[3, 3]");
}

TEST(InterpreterTest, Comparison) {
    run("print(2 == 2)", "true");
    run("print(2 != 3)", "true");
    run("print(2 < 3)", "true");
    run("print(\"abc\" < \"b\")", "true");
    run("print([1,2] == [1,2])", "true");
}

TEST(InterpreterTest, IndexingAndSlicing) {
    run("print(\"abc\"[1])", "b");
    run("print(\"abcdef\"[2:5])", "cde");
    run("print([10, 20, 30][1])", "20");
    run("print([1,2,3,4][1:3])", "[2, 3]");
    run("list = [\"abc\", \"bce\", \"cde\"]\nprint(list[0])", "abc");
    run("print([\"abc\", \"bce\", \"cde\"][1:])", "[bce, cde]");
    run("print([\"abc\", \"bce\", \"cde\"][:2])", "[abc, bce]");
    run("print([\"abc\", \"bce\", \"cde\"][:])", "[abc, bce, cde]");
}

TEST(InterpreterTest, FunctionCalls) {
    run("f = function(x) return x * 2 end function\nprint(f(10))", "20");
    run("funcs = [function() return 1 end function, function() return 2 end function]\nprint(funcs[0]())", "1");
}

TEST(InterpreterTest, ControlFlow) {
    run("if 1 == 1 then print(\"yes\") else print(\"no\") end if", "yes");
    run("i = 0\nwhile i < 3\n  print(i)\n  i = i + 1\nend while", "012");
    run("for x in [\"a\", \"b\"]\n  print(x)\nend for", "ab");
}

TEST(InterpreterTest, BuiltinFunctions) {
    run("print(abs(-5))", "5");
    run("print(ceil(2.1))", "3");
    run("print(floor(2.9))", "2");
    run("print(round(2.4))", "2");
    run("print(to_string(123))", "123");
    run("print(parse_num(\"456\"))", "456");
    run("print(len([1,2,3]))", "3");
    run("print(len(\"abc\"))", "3");
}

TEST(InterpreterTest, AssignmentsAndAugmentations) {
    run("x = 5\nx += 2\nprint(x)", "7");
    run("s = \"hi\"\ns += \"!\"\nprint(s)", "hi!");
    run("l = [1]\nl += [2]\nprint(l)", "[1, 2]");
    run("l = [1]\nl *= 2\nprint(l)", "[1, 1]");
}
