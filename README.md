# JAVA-Compiler-Phase1-and-Phase2

# Phase 1:
A suitable Syntax Directed Translation Scheme to convert Java code to Java bytecode, performing necessary lexical, syntax and static semantic analysis (such as type checking and Expressions Evaluation).


# Phase 2:
Parser Generator Objective This phase of the assignment aims to practice techniques for building automatic parser generator tools. Description Your task in this phase of the assignment is to design and implement an LL (1) parser generator tool. The parser generator expects an LL (1) grammar as input. It should compute First and Follow sets and uses them to construct a predictive parsing table for the grammar. The table is to be used to drive a predictive top-down parser. If the input grammar is not LL (1), an appropriate error message should be produced. The generated parser is required to produce some representation of the leftmost derivation for a correct input. If an error is encountered, a panic-mode error recovery routine is to be called to print an error message and to resume parsing. The parser generator is required to be tested using the given context free grammar of a small subset of Java. Of course, you have to modify the grammar to allow predictive parsing. Combine the lexical analyzer generated in phase1 and parser such that the lexical analyzer is to be called by the parser to find the next token. Use the simple program given in phase 1 to test the combined lexical analyzer and parser. Bonus Task Automatically eliminating grammar left recursion and performing left factoring before generating the parser will be considered a bonus work.

