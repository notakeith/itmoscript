#include "Parser.h"

#include "gtest/gtest.h"

static std::unique_ptr<AST::BlockStmt> parseSrc(const std::string& src) {
    Lexer lexer(src);
    Parser parser(lexer);
    return parser.parse();
}

TEST(ParserTest, IntegerAssignment) {
    auto block = parseSrc("x = 10");
    ASSERT_EQ(block->statements.size(), 1);
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    ASSERT_NE(stmt, nullptr);
    EXPECT_EQ(stmt->name, "x");
    EXPECT_EQ(stmt->op, "=");
    auto num = dynamic_cast<AST::NumberExpr*>(stmt->value.get());
    ASSERT_NE(num, nullptr);
    EXPECT_DOUBLE_EQ(num->value, 10);
}

TEST(ParserTest, FloatAssignment) {
    auto block = parseSrc("y = 20.5");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    auto num = dynamic_cast<AST::NumberExpr*>(stmt->value.get());
    ASSERT_NE(num, nullptr);
    EXPECT_DOUBLE_EQ(num->value, 20.5);
}

TEST(ParserTest, StringAssignment) {
    auto block = parseSrc("name = \"ITMO\"");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    auto s = dynamic_cast<AST::StringExpr*>(stmt->value.get());
    ASSERT_NE(s, nullptr);
    EXPECT_EQ(s->value, "ITMO");
}

TEST(ParserTest, BoolAssignment) {
    auto block = parseSrc("flag = true");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    auto b = dynamic_cast<AST::BoolExpr*>(stmt->value.get());
    ASSERT_NE(b, nullptr);
    EXPECT_TRUE(b->value);
}

TEST(ParserTest, NilAssignment) {
    auto block = parseSrc("empty = nil");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    auto n = dynamic_cast<AST::NilExpr*>(stmt->value.get());
    EXPECT_NE(n, nullptr);
}

TEST(ParserTest, ListLiteral) {
    auto block = parseSrc("list = [1,2,3]");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    auto listExpr = dynamic_cast<AST::ListExpr*>(stmt->value.get());
    ASSERT_NE(listExpr, nullptr);
    EXPECT_EQ(listExpr->elements.size(), 3);
}

TEST(ParserTest, SliceExpressions) {
    auto check = [&](const std::string& src) {
        auto block = parseSrc(src);
        auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
        auto sl = dynamic_cast<AST::SliceExpr*>(stmt->value.get());
        EXPECT_NE(sl, nullptr);
    };
    check("slice1 = name[1:]");
    check("slice2 = list[1:2]");
    check("slice3 = name[:3]");
    check("slice4 = name[:]");
}

TEST(ParserTest, ArithmeticExpression) {
    auto block = parseSrc("a = x + y * 2 - 4 / 2 % 3 ^ 2");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    auto expr = dynamic_cast<AST::BinaryExpr*>(stmt->value.get());
    ASSERT_NE(expr, nullptr);
    EXPECT_EQ(expr->op, "-");
}

TEST(ParserTest, LogicalExpression) {
    auto block = parseSrc("b = not false and true or false");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    auto expr = stmt->value.get();
    EXPECT_NE(dynamic_cast<AST::BinaryExpr*>(expr), nullptr);
}

TEST(ParserTest, PlusAssign) {
    auto block = parseSrc("x += 1");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    EXPECT_EQ(stmt->op, "+=");
}

TEST(ParserTest, MinusAssign) {
    auto block = parseSrc("y -= 2");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    EXPECT_EQ(stmt->op, "-=");
}

TEST(ParserTest, MulAssign) {
    auto block = parseSrc("x *= 3");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    EXPECT_EQ(stmt->op, "*=");
}

TEST(ParserTest, DivAssign) {
    auto block = parseSrc("y /= 4");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    EXPECT_EQ(stmt->op, "/=");
}

TEST(ParserTest, ModAssign) {
    auto block = parseSrc("x %= 5");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    EXPECT_EQ(stmt->op, "%=");
}

TEST(ParserTest, PowAssign) {
    auto block = parseSrc("y ^= 2");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    EXPECT_EQ(stmt->op, "^=");
}

TEST(ParserTest, IfElseIfElse) {
    auto block = parseSrc(
        "if x < 5 then msg = \"low\""
        " else if x == 10 then msg = \"perfect\""
        " else msg = \"high\" end if");
    auto ifs = dynamic_cast<AST::IfStmt*>(block->statements[0].get());
    ASSERT_NE(ifs, nullptr);
    EXPECT_EQ(ifs->branches.size(), 2);
    EXPECT_EQ(ifs->else_body.size(), 1);
}

TEST(ParserTest, WhileBreakContinue) {
    auto block = parseSrc(
        "count = 0 while count < 5 count += 1"
        " if count == 3 then continue else if count == 4 then break end if end while");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    auto w = dynamic_cast<AST::WhileStmt*>(block->statements[1].get());
    ASSERT_NE(w, nullptr);
}

TEST(ParserTest, ForLoopRange) {
    auto block = parseSrc("for i in range(5) sum += i end for");
    auto f = dynamic_cast<AST::ForStmt*>(block->statements[0].get());
    ASSERT_NE(f, nullptr);
    EXPECT_EQ(f->var, "i");
}

TEST(ParserTest, FunctionDefinition) {
    auto block = parseSrc("add = function(a,b) return a + b end function");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    ASSERT_NE(stmt, nullptr);
    auto fnExpr = dynamic_cast<AST::FunctionExpr*>(stmt->value.get());
    ASSERT_NE(fnExpr, nullptr);
    EXPECT_EQ(fnExpr->params.size(), 2);
}

TEST(ParserTest, FunctionCall) {
    auto block = parseSrc("res = add(3,4)");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    auto call = dynamic_cast<AST::CallExpr*>(stmt->value.get());
    ASSERT_NE(call, nullptr);
    EXPECT_EQ(call->args.size(), 2);
}

TEST(ParserTest, NoopFunction) {
    auto block = parseSrc("noop = function() end function");
    auto stmt = dynamic_cast<AST::AssignStmt*>(block->statements[0].get());
    ASSERT_NE(stmt, nullptr);
    auto fnExpr = dynamic_cast<AST::FunctionExpr*>(stmt->value.get());
    ASSERT_NE(fnExpr, nullptr);

    EXPECT_TRUE(fnExpr->params.empty());
}

TEST(ParserTest, AnonymousHigherOrder) {
    auto block = parseSrc(
        "twice = function(f,x) return f(f(x)) end function"
        " inc = function(n) return n + 1 end function"
        " v = twice(inc,5)");
    auto assign = dynamic_cast<AST::AssignStmt*>(block->statements[2].get());
    ASSERT_NE(assign, nullptr);
    auto call = dynamic_cast<AST::CallExpr*>(assign->value.get());
    ASSERT_NE(call, nullptr);
    EXPECT_EQ(call->args.size(), 2);
}

TEST(ParserTest, NestedCallWithArgs) {
    auto block = parseSrc(
        "concat = function(a,b,c) return a+b+c end function"
        " s = concat(\"A\",\"B\",\"C\")");
    auto assign = dynamic_cast<AST::AssignStmt*>(block->statements[1].get());
    auto call = dynamic_cast<AST::CallExpr*>(assign->value.get());
    ASSERT_NE(call, nullptr);
    EXPECT_EQ(call->args.size(), 3);
}