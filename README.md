<div align="center">
    <picture>
    <source media="(prefers-color-scheme: dark)" srcset="banner-dark.svg">
    <source media="(prefers-color-scheme: light)" srcset="banner-light.svg">
    <img alt="ITMOScript preview" src="banner-light.svg">
    </picture>
</div>

> [Русская версия](README_RU.md)

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue?logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/17)
[![CMake](https://img.shields.io/badge/CMake-3.14%2B-064F8C?logo=cmake&logoColor=white)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A dynamically-typed scripting language with a hand-written interpreter in C++17. Implements the full compilation pipeline: **Lexer → Parser → AST → Interpreter**, with lexical scoping, first-class functions, list slicing, and automatic memory management.

## Language

### Types

`number` · `string` · `bool` · `nil` · `list` · `function`

### Syntax

```python
# Variables & arithmetic
x = 10
y = x ^ 2 + 3 * x - 1     # ^ is exponentiation

# Strings
greeting = "Hello, " + "world!"
repeated = "ha" * 3        # "hahaha"

# Lists & slicing
nums = [1, 2, 3, 4, 5]
print(nums[1:4])           # [2, 3, 4]
print(nums[:2])            # [1, 2]

# Control flow
if x > 5 then
    print("big")
else if x == 5 then
    print("five")
else
    print("small")
end if

# Loops
while x > 0
    x -= 1
end while

for i in range(0, 10, 2)
    print(i)               # 0 2 4 6 8
end for

# Functions (first-class, closures)
fib = function(n)
    if n == 0 then return 0 end if
    a = 0
    b = 1
    for i in range(1, n, 1)
        c = a + b
        a = b
        b = c
    end for
    return b
end function

print(fib(10))             # 55

# FizzBuzz — string * bool trick
fizzBuzz = function(n)
    for i in range(1, n)
        s = "Fizz" * (i % 3 == 0) + "Buzz" * (i % 5 == 0)
        if s == "" then print(i) else print(s) end if
    end for
end function
fizzBuzz(20)
```

### Built-ins

| Function | Description |
|----------|-------------|
| `print(x)` | Print value with newline |
| `len(x)` | Length of list or string |
| `range(start, end[, step])` | Generate a numeric range |
| `type(x)` | Return type name as string |

### Operators

| Category | Operators |
|----------|-----------|
| Arithmetic | `+` `-` `*` `/` `%` `^` |
| Comparison | `==` `!=` `<` `>` `<=` `>=` |
| Logical | `and` `or` `not` |
| Assignment | `=` `+=` `-=` `*=` `/=` `%=` `^=` |

## Architecture

```
Source text
    │
    ▼
┌─────────┐    tokens    ┌──────────┐    AST    ┌─────────────┐
│  Lexer  │ ──────────► │  Parser  │ ─────────►│ Interpreter │
└─────────┘              └──────────┘           └─────────────┘
                                                       │
                                                  Environment
                                                  (scope chain)
```

| Module | File | Responsibility |
|--------|------|----------------|
| Lexer | `lib/Lexer.h / .cpp` | Tokenization, keyword recognition, string/number literals |
| Parser | `lib/Parser.h / .cpp` | Recursive descent, operator precedence, block parsing |
| AST | `lib/AST.h` | Node hierarchy: `NumberExpr`, `BinaryExpr`, `CallExpr`, `FunctionExpr`, … |
| Value | `lib/Value.h / .cpp` | Dynamic type wrapper for all runtime values |
| Environment | `lib/Environment.h / .cpp` | Symbol table, lexical scope chain |
| Interpreter | `lib/Interpreter.h / .cpp` | AST traversal, expression evaluation, statement execution |
| Builtins | `lib/Builtins.h / .cpp` | Standard library functions |

Control flow (`return`, `break`, `continue`) is implemented via C++ exceptions caught at the appropriate scope boundary.

## Build

```bash
git clone https://github.com/notakeith/itmoscript.git
cd itmoscript
mkdir build && cd build
cmake ..
cmake --build .
```

## Usage

```bash
./bin/main path/to/script.is

# Bundled examples
./bin/main ../examples/fibonacci.is
./bin/main ../examples/fizzBuzz.is
./bin/main ../examples/slices.is
```

## Tests

```bash
ctest --output-on-failure
```

Tests cover: lexer tokenization, parser grammar, operator precedence, control flow, built-in functions, illegal operations, and type coercion.

## VSCode Extension

Syntax highlighting for `.is` files: [itmoscript-syntax](https://github.com/notakeith/itmoscript-syntax).

## Requirements

- C++17
- CMake 3.14+

## License

MIT — see [LICENSE](LICENSE).
