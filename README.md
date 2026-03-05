**Voice to Code Mini Compiler**

This project aims to convert voice statements to syntactically and semantically correct C codes. For now, it handles basic commands like print, declare and set.
The objective is to implement the phases of Compiler Design.
Working:
1. Executing the voice.py file, allows the user to speak the command, which is recorded and normalized through voicelang. 
2. This output is then stored in input.vl file, which then goes through all the phases of compiler and final output as C code is generated.
   
# Lexical and Syntax Analysis Phase.
  The lexical and syntax phases of the compiler are implemented using Flex and Bison.
  
4. The lexer tokenizes the normalized input file (input.vl)
5. The parser validates the grammar using LALR parsing. During parsing, an Abstract Syntax Tree (AST) is generated to represent the program structure.
