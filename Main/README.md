# Voice to Code Mini Compiler

This project aims to convert voice statements to syntactically and semantically correct C codes. For now, it handles basic commands like print, declare and set.
The objective is to implement the phases of Compiler Design.
Working:
1. Executing the voice.py file, allows the user to speak the command, which is recorded and normalized through voicelang. 
2. This output is then stored in input.vl file, which then goes through all the phases of compiler and final output as C code is generated.
   
**Lexical and Syntax Analysis Phase**

  The lexical and syntax phases of the compiler are implemented using Flex and Bison.
  
4. The lexer tokenizes the normalized input file (input.vl)
5. The parser validates the grammar using LALR parsing. During parsing, an Abstract Syntax Tree (AST) is generated to represent the program structure.

## Official Grammar

The following grammar describes the current VoiceLang syntax implemented by the lexer and parser.

### Lexical Conventions

- Keywords: `DECLARE`, `SET`, `PRINT`, `INPUT`, `FROM`, `USER`, `IF`, `THEN`, `ELSE`, `BEGIN`, `END`, `AS`, `INT`, `VALUE`
- Operators: `=`, `+`, `-`, `*`, `/`, `<`, `>`
- Identifier: a letter or underscore followed by letters, digits, or underscores
- Number: one or more digits
- Whitespace: spaces, tabs, and newlines are ignored except as token separators

### EBNF Grammar

```ebnf
program         = statement_list ;

statement_list  = statement , { statement } ;

statement       = declaration
                | assignment
                | print_stmt ;

declaration     = "DECLARE" , identifier , "AS" , "INT" , "VALUE" , expression ;

assignment      = "SET" , identifier , "=" , expression ;

print_stmt      = "PRINT" , expression ;

expression      = term , { ("+" | "-") , term } ;

term            = factor , { ("*" | "/") , factor } ;

factor          = number
                | identifier ;

identifier      = letter_or_underscore , { letter_or_digit_or_underscore } ;

number          = digit , { digit } ;
```

### Equivalent BNF Form

```bnf
<program>        ::= <statement_list>

<statement_list> ::= <statement>
                   | <statement_list> <statement>

<statement>      ::= <declaration>
                   | <assignment>
                   | <print_stmt>
                   | <input_stmt>
                   | <if_stmt>

<declaration>    ::= DECLARE IDENTIFIER AS INT VALUE <expression>
                   | DECLARE IDENTIFIER AS INT
                   | DECLARE IDENTIFIER

<assignment>     ::= SET IDENTIFIER = <expression>

<print_stmt>     ::= PRINT <expression>

<input_stmt>     ::= INPUT IDENTIFIER FROM USER

<if_stmt>        ::= IF <expression> THEN <statement> [ ELSE <statement> ]
                   | IF <expression> THEN <statement> ELSE IF <expression> THEN <statement> [ ELSE <statement> ]
                   | IF <expression> THEN BEGIN <statement_list> END
                   | IF <expression> THEN BEGIN <statement_list> END ELSE <statement>
                   | IF <expression> THEN <statement> ELSE BEGIN <statement_list> END
                   | IF <expression> THEN BEGIN <statement_list> END ELSE BEGIN <statement_list> END
                   | IF <expression> THEN <statement> <statement> ... END
                   | IF <expression> THEN <statement> <statement> ... ELSE <statement> <statement> ... END

<expression>     ::= <expression> + <term>
                   | <expression> - <term>
                   | <term>

<term>           ::= <term> * <factor>
                   | <term> / <factor>
                   | <factor>

<factor>         ::= NUMBER
                   | IDENTIFIER
```

### Operator Precedence and Associativity

- Highest precedence: `*`, `/`
- Lower precedence: `+`, `-`
- All arithmetic operators are left-associative

So the expression:

```text
2 + 3 * 4
```

is parsed as:

```text
2 + (3 * 4)
```

### Supported Statement Forms

```text
DECLARE x AS INT VALUE 5
SET x = 10
PRINT x
PRINT x + 2
DECLARE total AS INT VALUE 2 + 3 * 4
IF x > y THEN PRINT x
IF x > y THEN PRINT x ELSE PRINT y
IF x > y THEN PRINT x ELSE IF x = y THEN PRINT 0 ELSE PRINT y
IF x > 0 THEN BEGIN
PRINT x
SET y = 10
END
IF x > 0 THEN BEGIN
PRINT x
END ELSE BEGIN
PRINT 7
END
```

### Voice input examples

Use normalized keywords and comparison symbols in `input.vl`:

- `IF x > y THEN PRINT x`
- `IF x > y THEN PRINT x ELSE PRINT y`
- `IF x > y THEN PRINT x ELSE IF x = y THEN PRINT 0 ELSE PRINT y`

### Notes

- The current implementation supports only the `INT` type in the parser grammar.
- The normalizer may insert `AS INT` automatically for some spoken declarations before parsing.
- Parentheses are not part of the current grammar.
- Float literals are not part of the current grammar, even though semantic type enums include `TYPE_FLOAT` for future extension.
