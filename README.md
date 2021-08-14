# Coco/R for C++

Coco/R is a compiler generator, which takes an attributed grammar of a source language and generates a scanner and a parser for this language. The scanner works as a deterministic finite automaton. The parser uses recursive descent. LL(1) conflicts can be resolved by a multi-symbol lookahead or by semantic checks. Thus the class of accepted grammars is LL(k) for an arbitrary k.

http://ssw.jku.at/coco/

And this are my main modifications to the original:

- Fix all known memory leaks

- Enhance left recursion detection

- Allow semantic actions on `token declaration` similar to `pragmas` but the code executes on the Scanner

- Allow till 8 characters as comment delimiters

- Add option `-genRREBNF` to generate an EBNF grammar to crate railroad diagrams at https://www.bottlecaps.de/rr/ui

- Add option `-geAST` to generate code to generate `parser syntax tree` based on https://github.com/rochus-keller/EbnfStudio

- Add option `-ignoreGammarErrors` to make easier to develop grammars, like commenting one non terminal and still generating the parser and scanner even with sevral non reachable non terminals

- Add a `TERMINALS` section to generate user define tokens not managed by the Scanner (from cocoxml)

- Refactor the code to allow compile with and without wchar_t depending on the definition of `PARSER_WITH_AST` compiler macro

- Generate between comments the correspondent representation of several magic numbers (mainly Tokens)
- Add the `_NT` suffix to non terminal functions to prevent name collision
- Add token inheritance from https://github.com/Lercher/CocoR

See also https://github.com/mingodad/CocoR-Java and https://github.com/mingodad/CocoR-CSharp
