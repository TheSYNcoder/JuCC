## This is the grammar file for JuCC
## Edit this file to make changes to the parsing grammar
## Epsilon is represented by special string EPSILON

## Terminals
%terminals
else float if int void
( ) { } * + - / % ,
<< >> < > <= >= = == != ;
identifier integer_constant float_constant
main cin cout
%

## Non Terminals
%non_terminals
<primary_expression> <constant> <unary_operator> <type_specifier> 
<unary_expression> <multiplicative_expression> <additive_expression>
<shift_expression> <relational_expression>
<equality_expression> <assignment_expression> <expression>
<declaration> <init_declarator_list> <init_declarator>
<initializer> <declarator> <direct_declarator>
<statement> <compound_statement> <block_item_list> <block_item>
<expression_statement> <selection_statement> <program>
%

## Start Symbol
%start
<program>
%

## Grammar for the language
%rules
## Expressions
<primary_expression> : identifier
<primary_expression> : <constant>
<primary_expression> : ( <expression> )
<constant> : integer_constant
<constant> : float_constant
<unary_operator> : +
<unary_operator> : -
<unary_expression> : <primary_expression>
<unary_expression> : <unary_operator> <primary_expression>
<multiplicative_expression> : <unary_expression>
<multiplicative_expression> : <multiplicative_expression> * <unary_expression>
<multiplicative_expression> : <multiplicative_expression> / <unary_expression>
<multiplicative_expression> : <multiplicative_expression> % <unary_expression>
<additive_expression> : <multiplicative_expression>
<additive_expression> : <additive_expression> + <multiplicative_expression>
<additive_expression> : <additive_expression> - <multiplicative_expression>
<shift_expression> : <additive_expression>
<shift_expression> : cin >> <additive_expression>
<shift_expression> : cout << <additive_expression>
<shift_expression> : <shift_expression> << <additive_expression>
<shift_expression> : <shift_expression> >> <additive_expression>
<relational_expression> : <shift_expression>
<relational_expression> : <relational_expression> < <shift_expression>
<relational_expression> : <relational_expression> > <shift_expression>
<relational_expression> : <relational_expression> <= <shift_expression>
<relational_expression> : <relational_expression> >= <shift_expression>
<equality_expression> : <relational_expression>
<equality_expression> : <equality_expression> == <relational_expression>
<equality_expression> : <equality_expression> != <relational_expression>
<assignment_expression> : <equality_expression>
<expression> : <assignment_expression>

## Declarations
<declaration> : <type_specifier> <init_declarator_list> ;
<init_declarator_list> : <init_declarator>
<init_declarator_list> : <init_declarator> , <init_declarator_list>
<init_declarator_list> : EPSILON
<init_declarator> : <declarator>
<init_declarator> : <declarator> = <initializer>
<type_specifier> : void
<type_specifier> : int
<type_specifier> : float
<declarator> : <direct_declarator>
<direct_declarator> : identifier
<direct_declarator> : ( <declarator> )
<initializer> : <assignment_expression>

## Statements
<statement> : <compound_statement>
<statement> : <expression_statement>
<statement> : <selection_statement>
<compound_statement> : { <block_item_list> }
<block_item_list> : <block_item>
<block_item_list> : <block_item> <block_item_list>
<block_item_list> : EPSILON
<block_item> : <declaration>
<block_item> : <statement>
<expression_statement> : <expression> ;
<expression_statement> : ;
<selection_statement> : if ( <expression> ) <compound_statement>
<selection_statement> : if ( <expression> ) <compound_statement> else <compound_statement>

## Main
<program> : <type_specifier> main ( ) <compound_statement>
%
