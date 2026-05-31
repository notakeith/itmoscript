```plaintext
program                : statement_list

statement_list         : statement
                       | statement statement_list

statement              : assignment_statement
                       | if_statement
                       | while_loop
                       | for_loop
                       | function_definition
                       | function_call
                       | compound_statement
                       | BREAK
                       | CONTINUE
                       | empty

compound_statement     : block_start statement_list block_end
block_start            : (IF condition THEN)?
block_end              : END IF | END WHILE | END FOR | END FUNCTION

assignment_statement   : ID assignment_operator expr

assignment_operator    : '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '^='

if_statement           : IF condition THEN statement_list else_part END IF
else_part              : ELSE IF condition THEN statement_list else_part
                       | ELSE statement_list
                       | empty

while_loop             : WHILE condition statement_list END WHILE

for_loop               : FOR ID IN expr statement_list END FOR

function_definition    : ID '=' FUNCTION LPAREN parameter_list RPAREN statement_list END FUNCTION

parameter_list         : ID
                       | ID COMMA parameter_list
                       | empty

function_call          : ID LPAREN argument_list RPAREN

argument_list          : expr
                       | expr COMMA argument_list
                       | empty

condition              : expr comparison_operator expr

comparison_operator    : '==' | '!=' | '<' | '>' | '<=' | '>='

expr                   : term (( '+' | '-' | 'or' ) term)*

term                   : factor (( '*' | '/' | '%' | 'and' ) factor)*

factor                 : ('+' | '-' | 'not') factor
                       | NUMBER
                       | STRING
                       | 'true' | 'false' | 'nil'
                       | list_literal
                       | string_slice
                       | LPAREN expr RPAREN
                       | function_call
                       | ID

list_literal           : '[' list_elements ']'
list_elements          : expr (',' expr)*
                       | empty

string_slice           : ID '[' slice_index ']'
slice_index            : expr
                       | expr ':' expr
                       | ':' expr
                       | expr ':'
                       | ':'

empty                  :
```