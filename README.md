**Voice to Code Mini Compiler**

This project aims to convert voice statements to syntactically and semantically correct C codes. For now, it handles basic commands like print, declare and set.
The objective is to implement the phases of Compiler Design.
Working:
1. Executing the voice.py file, allows the user to speak the command, which is recorded and normalized through voicelang. 
2. This output is then stored in input.vl file, which then goes through all the phases of compiler and final output as C code is generated.
3. Flex and Bison are used for Lexical and Parsing phases.

Document containing grammar will be released after finalization.
