#include <gtest/gtest.h>
#include <sstream>
#include "Interpreter.h"

inline void run(const std::string& code, const std::string& expected_output) {
    std::istringstream input(code);
    std::ostringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected_output);
}

TEST(StdLibTest, MathFunctions) {
    run("print(abs(-3.14))", "3.14");
    run("print(abs(3.14))", "3.14");
    run("print(ceil(2.1))", "3");
    run("print(ceil(-1.9))", "-1");
    run("print(floor(2.9))", "2");
    run("print(floor(-1.9))", "-2");
    run("print(round(2.4))", "2");
    run("print(round(2.6))", "3");
    run("print(sqrt(4))", "2");
    run("print(sqrt(2))", "1.41421");
    run("print(rnd(10) < 10)", "true");
    run("print(parse_num(\"3.14\"))", "3.14");
    run("print(parse_num(\"abc\"))", "nil");
    run("print(to_string(123))", "123");
    run("print(to_string(3.14))", "3.14");
}

TEST(StdLibTest, StringFunctions) {
    run("print(len(\"hello\"))", "5");
    run("print(len(\"\"))", "0");
    run("print(lower(\"HELLO\"))", "hello");
    run("print(upper(\"hello\"))", "HELLO");
    run("print(split(\"a,b,c\", \",\"))", "[a, b, c]");
    run("print(split(\"a b c\", \" \"))", "[a, b, c]");
    run("print(join([\"a\", \"b\", \"c\"], \",\"))", "a,b,c");
    run("print(replace(\"hello world\", \"world\", \"there\"))", "hello there");
    run("print(replace(\"ababab\", \"a\", \"x\"))", "xbxbxb");
}

TEST(StdLibTest, ListFunctions) {
    run("print(range(1, 5))", "[1, 2, 3, 4]");
    run("print(range(1, 10, 2))", "[1, 3, 5, 7, 9]");
    run("l = [1,2,3]\nprint(len(l))", "3");
    run("l = [1,2]\npush(l, 3)\nprint(l)", "[1, 2, 3]");
    run("l = [1,2,3]\nprint(pop(l))\nprint(l)", "3[1, 2]");
    run("l = [1,3]\ninsert(l, 1, 2)\nprint(l)", "[1, 2, 3]");
    run("l = [1,2,3,4]\nremove(l, 1)\nprint(l)", "[1, 3, 4]");
    run("l = [3,1,2]\nsort(l)\nprint(l)", "[1, 2, 3]");
    run("l = [\"b\", \"a\", \"c\"]\nsort(l)\nprint(l)", "[a, b, c]");
}

TEST(StdLibTest, SystemFunctions) {
    run("print(\"hello\")\nprint(\"world\")", "helloworld");
    run("println(\"hello\")\nprintln(\"world\")", "hello\nworld\n");
    run("foo = function()\nreturn bar()\nend function\nbar = function()\nreturn stacktrace()\nend function\nprint(len(foo()))\nprint(len(stacktrace()))", "20");
}