## This is the grammar file for JuCC
## Edit this file to make changes to the parsing grammar
## Epsilon is represented by special string EPSILON

## Terminals
%terminals
else float if int void
( ) { } * + - / %
<< >> < > <= >= = == != ;
identifier integer_constant float_constant
%

## Non Terminals
%non_terminals
<primary_expression> <constant>
%

## Start Symbol
%start
<primary_expression>
%

## Grammar for the language
%rules
<primary_expression> : identifier
<primary_expression> : <constant>
<primary_expression> : EPSILON
<constant>
<constant> : float_constant
%
