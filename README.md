# Лабораторная работа №10: Интерпретатор ITMOScript

[![C++](https://img.shields.io/badge/-C%2B%2B-blue)](https://isocpp.org/)
[![Build](https://img.shields.io/badge/Build-Main-brightgreen)](https://github.com/is-itmo-c-24/labwork10-notakeith/tree/deadline0/bin)
[![License](https://img.shields.io/badge/License-MIT-green)](LICENSE)

---

## 🎯 Описание проекта

**ITMOScript** — это динамически типизированный скриптовый язык, разработанный для обучения основам интерпретации. Язык поддерживает:

* ✨ **Примитивы**: числа (`number`), строки (`string`), логические значения (`bool`), `nil`.
* 🔗 **Составные типы**: списки (`list`), функции первого класса (`function`).
* 🛠 **Операции**: арифметические, логические, сравнения, индексирование и присваивание.
* 🔄 **Управление потоком**: `if/else`, `while`, `for`, `break`, `continue`.
* 🔄 **Автоматическое управление памятью**: сборка мусора, переменные вышедшие из области видимости удаляются автоматически.

Лабораторная работа включает реализацию лексера, парсера, AST и интерпретатора.

## 📖 Документация

Полная спецификация языка и руководство пользователя находятся в разделе `docs`:

* [Спефикация синтаксиса и семантики](docs/spec.md)
* [Примеры использования ITMOScript](docs/examples.md)
* [Описание модулей и архитектуры](docs/architecture.md)

## 📂 Структура проекта

```plaintext
├── include/        # Заголовочные файлы (Lexer, Parser, AST, Builtins, Interpreter)
├── src/            # Исходный код реализации
├── docs/           # Документация (спецификации, примеры, архитектура)
├── example/        # Скрипты-примеры (*.is)
├── CMakeLists.txt  # CMake конфигурация
└── README.md       # Этот файл
```

## 📝 Синтаксис ITMOScript

```is
# Пример функции вычисления факториала
factorial = function(n)
    if n <= 1 then
        return 1
    else
        return n * factorial(n - 1)
    end if
end function

print(factorial(5))  # 120
```

**Основные элементы:**

| Категория          | Примеры                                                                |
| ------------------ | ---------------------------------------------------------------------- |
| Типы               | `number`, `string`, `bool`, `nil`, `list`, `function`                  |
| Арифметика         | `+`, `-`, `*`, `/`, `%`, `^`                                           |
| Логика и сравнения | `and`, `or`, `not`, `<`, `>`, `==`, `!=`                               |
| Управление потоком | `if...then...else...end if`, `while...end while`, `for...in...end for` |
| Функции            | `function` и `end function`, `return`                                  |

## 🎨 Расширение VSC

Специально для данной лабораторной работы был создан [плагин для VSC, который добавляет поддержку синтаксиса ITMOScript](https://github.com/notakeith/itmoscript-syntax)

## 📄 Лицензия

Проект распространяется под лицензией **MIT License**. Подробности в файле [LICENSE](LICENSE).
