#include "parser.h"


/*
=== Conjuntos FIRST+ por Produção ===
---------Parte do Felipe---------
Não-Terminal: Program
  -> Regra: Program -> FunctionList EOF | FIRST+ = [EOF, char, int, void]
--------------------------------------------------
Não-Terminal: FunctionList
  -> Regra: FunctionList -> Function FunctionList | FIRST+ = [char, int, void]
  -> Regra: FunctionList -> ε | FIRST+ = [EOF]
--------------------------------------------------
Não-Terminal: Function
  -> Regra: Function -> Type ID ( ParamTypes ) { Declarations StatementList } | FIRST+ = [char, int]
  -> Regra: Function -> void ID ( ParamTypes ) { Declarations StatementList } | FIRST+ = [void]
--------------------------------------------------
Não-Terminal: Declarations
  -> Regra: Declarations -> Type VarDeclaration VarDeclarationList ; Declarations | FIRST+ = [char, int]
  -> Regra: Declarations -> ε | FIRST+ = [;, ID, for, if, return, while, {, }]
--------------------------------------------------
Não-Terminal: VarDeclarationList
  -> Regra: VarDeclarationList -> , VarDeclaration VarDeclarationList | FIRST+ = [,]
  -> Regra: VarDeclarationList -> ε | FIRST+ = [;]
--------------------------------------------------
Não-Terminal: VarDeclaration
  -> Regra: VarDeclaration -> ID VarArrayTail | FIRST+ = [ID]
--------------------------------------------------
Não-Terminal: VarArrayTail
  -> Regra: VarArrayTail -> [ integerconstant ] | FIRST+ = [[]
  -> Regra: VarArrayTail -> ε | FIRST+ = [,, ;]
--------------------------------------------------
Não-Terminal: Type
  -> Regra: Type -> char | FIRST+ = [char]
  -> Regra: Type -> int | FIRST+ = [int]
--------------------------------------------------
Não-Terminal: ParamTypes
  -> Regra: ParamTypes -> void | FIRST+ = [void]
  -> Regra: ParamTypes -> Type ID ParamArrayTail ParamTypesTail | FIRST+ = [char, int]
--------------------------------------------------
Não-Terminal: ParamArrayTail
  -> Regra: ParamArrayTail -> [ ] | FIRST+ = [[]
  -> Regra: ParamArrayTail -> ε | FIRST+ = [), ,]
--------------------------------------------------
Não-Terminal: ParamTypesTail
  -> Regra: ParamTypesTail -> , Type ID ParamArrayTail ParamTypesTail | FIRST+ = [,]
  -> Regra: ParamTypesTail -> ε | FIRST+ = [)]
--------------------------------------------------
Não-Terminal: Statement
  -> Regra: Statement -> if ( Expression ) Statement IfTail | FIRST+ = [if]
  -> Regra: Statement -> while ( Expression ) Statement | FIRST+ = [while]
  -> Regra: Statement -> for ( ForInit ; ForCondition ; ForUpdate ) Statement | FIRST+ = [for]
  -> Regra: Statement -> return ReturnTail | FIRST+ = [return]
  -> Regra: Statement -> ID IdStatementTail ; | FIRST+ = [ID]
  -> Regra: Statement -> { StatementList } | FIRST+ = [{]
  -> Regra: Statement -> ; | FIRST+ = [;]
--------------------------------------------------
Não-Terminal: IfTail
  -> Regra: IfTail -> else Statement | FIRST+ = [else] (OBS: Usar sempre essa regra quando aparecer um ‘else’)
  -> Regra: IfTail -> ε | FIRST+ = [;, ID, else, for, if, return, while, {, }]
--------------------------------------------------
Não-Terminal: ReturnTail
  -> Regra: ReturnTail -> Expression ; | FIRST+ = [!, (, -, ID, charconstant, integerconstant, stringconstant]
  -> Regra: ReturnTail -> ; | FIRST+ = [;]
--------------------------------------------------
Não-Terminal: ForInit
  -> Regra: ForInit -> ID IdStatementTail | FIRST+ = [ID]
  -> Regra: ForInit -> ε | FIRST+ = [;]
--------------------------------------------------
Não-Terminal: ForCondition
  -> Regra: ForCondition -> Expression | FIRST+ = [!, (, -, ID, charconstant, integerconstant, stringconstant]
  -> Regra: ForCondition -> ε | FIRST+ = [;]
--------------------------------------------------
Não-Terminal: ForUpdate
  -> Regra: ForUpdate -> ID IdStatementTail | FIRST+ = [ID]
  -> Regra: ForUpdate -> ε | FIRST+ = [)]
--------------------------------------------------
Não-Terminal: StatementList
  -> Regra: StatementList -> Statement StatementList | FIRST+ = [;, ID, for, if, return, while, {]
  -> Regra: StatementList -> ε | FIRST+ = [}]
--------------------------------------------------
Não-Terminal: IdStatementTail
  -> Regra: IdStatementTail -> [ Expression ] = Expression | FIRST+ = [[]
  -> Regra: IdStatementTail -> = Expression | FIRST+ = [=]
  -> Regra: IdStatementTail -> ( ExpressionList ) | FIRST+ = [(]
--------------------------------------------------
Não-Terminal: Expression
  -> Regra: Expression -> Expr_And Expression’ | FIRST+ = [!, (, -, ID, charconstant, integerconstant, stringconstant]


------------Parte Cassiano-----------------------
Não-Terminal: Expression’
  -> Regra: Expression’ -> || Expr_And Expression’ | FIRST+ = [||]
  -> Regra: Expression’ -> ε | FIRST+ = [), ,, ;, ]]
--------------------------------------------------
Não-Terminal: Expr_And
  -> Regra: Expr_And -> Expr_Rel Expr_And’ | FIRST+ = [!, (, -, ID, charconstant, integerconstant, stringconstant]
--------------------------------------------------
Não-Terminal: Expr_And’
  -> Regra: Expr_And’ -> && Expr_Rel Expr_And’ | FIRST+ = [&&]
  -> Regra: Expr_And’ -> ε | FIRST+ = [), ,, ;, ], ||]
--------------------------------------------------
Não-Terminal: Expr_Rel
  -> Regra: Expr_Rel -> Expr_Add Expr_Rel’ | FIRST+ = [!, (, -, ID, charconstant, integerconstant, stringconstant]
--------------------------------------------------
Não-Terminal: Expr_Rel’
  -> Regra: Expr_Rel’ -> RelOp Expr_Add Expr_Rel’ | FIRST+ = [!=, <, <=, ==, >, >=]
  -> Regra: Expr_Rel’ -> ε | FIRST+ = [&&, ), ,, ;, ], ||]
--------------------------------------------------
Não-Terminal: Expr_Add
  -> Regra: Expr_Add -> Expr_Mult Expr_Add’ | FIRST+ = [!, (, -, ID, charconstant, integerconstant, stringconstant]
--------------------------------------------------
Não-Terminal: Expr_Add’
-> Regra: Expr_Add’ -> AddOp Expr_Mult Expr_Add’ | FIRST+ = [+, -]
-> Regra: Expr_Add’ -> ε | FIRST+ = [!=, &&, ), ,, ;, <, <=, ==, >, >=, ], ||]
--------------------------------------------------
Não-Terminal: Expr_Mult
  -> Regra: Expr_Mult -> Expr_Unary Expr_Mult’ | FIRST+ = [!, (, -, ID, charconstant, integerconstant, stringconstant]
--------------------------------------------------
Não-Terminal: Expr_Mult’
  -> Regra: Expr_Mult’ -> MultOp Expr_Unary Expr_Mult’ | FIRST+ = [*, /]
  -> Regra: Expr_Mult’ -> ε | FIRST+ = [!=, &&, ), +, ,, -, ;, <, <=, ==, >, >=, ], ||]
--------------------------------------------------
Não-Terminal: Expr_Unary
  -> Regra: Expr_Unary -> - Expr_Unary | FIRST+ = [-]
  -> Regra: Expr_Unary -> ! Expr_Unary | FIRST+ = [!]
  -> Regra: Expr_Unary -> Primary | FIRST+ = [(, ID, charconstant, integerconstant, stringconstant]
--------------------------------------------------
Não-Terminal: Primary
  -> Regra: Primary -> ID PrimarTail | FIRST+ = [ID]
  -> Regra: Primary -> ( Expression ) | FIRST+ = [(]
  -> Regra: Primary -> integerconstant | FIRST+ = [integerconstant]
  -> Regra: Primary -> charconstant | FIRST+ = [charconstant]
  -> Regra: Primary -> stringconstant | FIRST+ = [stringconstant]
--------------------------------------------------
Não-Terminal: PrimarTail
  -> Regra: PrimarTail -> ( ExpressionList ) | FIRST+ = [(]
  -> Regra: PrimarTail -> [ Expression ] | FIRST+ = [[]
  -> Regra: PrimarTail -> ε | FIRST+ = [!=, &&, ), *, +, ,, -, /, ;, <, <=, ==, >, >=, ], ||]
--------------------------------------------------
Não-Terminal: ExpressionList
  -> Regra: ExpressionList -> Expression ExpressionListTail | FIRST+ = [!, (, -, ID, charconstant, integerconstant, stringconstant]
  -> Regra: ExpressionList -> ε | FIRST+ = [)]
--------------------------------------------------
Não-Terminal: ExpressionListTail
  -> Regra: ExpressionListTail -> , Expression ExpressionListTail | FIRST+ = [,]
  -> Regra: ExpressionListTail -> ε | FIRST+ = [)]
--------------------------------------------------
Não-Terminal: AddOp
  -> Regra: AddOp -> + | FIRST+ = [+]
  -> Regra: AddOp -> - | FIRST+ = [-]
--------------------------------------------------
Não-Terminal: MultOp
  -> Regra: MultOp -> * | FIRST+ = [*]
  -> Regra: MultOp -> / | FIRST+ = [/]
--------------------------------------------------
Não-Terminal: RelOp
  -> Regra: RelOp -> == | FIRST+ = [==]
  -> Regra: RelOp -> != | FIRST+ = [!=]
  -> Regra: RelOp -> <= | FIRST+ = [<=]
  -> Regra: RelOp -> < | FIRST+ = [<]
  -> Regra: RelOp -> >= | FIRST+ = [>=]
  -> Regra: RelOp -> > | FIRST+ = [>]
--------------------------------------------------

*/

Parser::Parser(string input)
{
	globalTable = currentTable = new SymbolTable();
	initializeSymbolTable();

	scanner = new Scanner(input, globalTable);
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

void
Parser::match(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
}

void
Parser::run()
{
	advance();	

	program();
	
	cout << "Compilação encerrada com sucesso!\n";
}

// Parte do Felipe
void Parser::program()
{
	if (lToken->name == CHAR || lToken->name == INT || lToken->name == VOID)
	{
		functionlist();
		match(END_OF_FILE);
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::functionlist()
{
	if (lToken->name == CHAR || lToken->name == INT || lToken->name == VOID)
	{
		function();
		functionlist();
	}
	else if (lToken->name == END_OF_FILE)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::function()
{
	if (lToken->name == CHAR || lToken->name == INT)
	{
		type();
		match(IDENTIFIER);
		match(LPAREN);
		paramTypes();
		match(RPAREN);
		match(LBRACE);
		declarations();
		statementList();
		match(RBRACE);
	}
	else if (lToken->name == VOID)
	{
		match(VOID);
		match(IDENTIFIER);
		match(LPAREN);
		paramTypes();
		match(RPAREN);
		match(LBRACE);
		declarations();
		statementList();
		match(RBRACE);
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::declarations()
{
	if (lToken->name == CHAR || lToken->name == INT)
	{
		type();
		varDeclaration();
		varDeclarationList();
		match(SEMICOLON);
		declarations();
	}
	else if (lToken->name == SEMICOLON || lToken->name == IDENTIFIER || lToken->name == FOR || lToken->name == IF || lToken->name == RETURN || lToken->name == WHILE || lToken->name == LBRACE || lToken->name == RBRACE)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::varDeclarationList()
{
	if (lToken->name == COMMA)
	{
		match(COMMA);
		varDeclaration();
		varDeclarationList();
	}
	else if (lToken->name == SEMICOLON)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::varDeclaration()
{
	if (lToken->name == IDENTIFIER)
	{
		match(IDENTIFIER);
		varArrayTail();
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::varArrayTail()
{
	if (lToken->name == LBRACKET)
	{
		match(LBRACKET);
		match(INTCONST);
		match(RBRACKET);
	}
	else if (lToken->name == COMMA || lToken->name == SEMICOLON)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::type()
{
	if (lToken->name == CHAR)
	{
		match(CHAR);
	}
	else if (lToken->name == INT)
	{
		match(INT);
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::paramTypes()
{
	if (lToken->name == VOID)
	{
		match(VOID);
	}
	else if (lToken->name == CHAR || lToken->name == INT)
	{
		type();
		match(IDENTIFIER);
		paramArrayTail();
		paramTypesTail();
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::paramArrayTail()
{
	if (lToken->name == LBRACKET)
	{
		match(LBRACKET);
		match(RBRACKET);
	}
	else if (lToken->name == RPAREN || lToken->name == COMMA)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::paramTypesTail()
{
	if (lToken->name == COMMA)
	{
		match(COMMA);
		type();
		match(IDENTIFIER);
		paramArrayTail();
		paramTypesTail();
	}
	else if (lToken->name == RPAREN)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::statement()
{
	if (lToken->name == IF)
	{
		match(IF);
		match(LPAREN);
		expression();
		match(RPAREN);
		statement();
		ifTail();
	}
	else if (lToken->name == WHILE)
	{
		match(WHILE);
		match(LPAREN);
		expression();
		match(RPAREN);
		statement();
	}
	else if (lToken->name == FOR)
	{
		match(FOR);
		match(LPAREN);
		forInit();
		match(SEMICOLON);
		forCondition();
		match(SEMICOLON);
		forUpdate();
		match(RPAREN);
		statement();
	}
	else if (lToken->name == RETURN)
	{
		match(RETURN);
		returnTail();
	}
	else if (lToken->name == IDENTIFIER)
	{
		match(IDENTIFIER);
		idStatementTail();
		match(SEMICOLON);
	}
	else if (lToken->name == LBRACE)
	{
		match(LBRACE);
		statementList();
		match(RBRACE);
	}
	else if (lToken->name == SEMICOLON)
	{
		match(SEMICOLON);
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::ifTail()
{
	if (lToken->name == ELSE)
	{
		match(ELSE);
		statement();
	}
	else if (lToken->name == SEMICOLON || lToken->name == IDENTIFIER || lToken->name == ELSE || lToken->name == FOR || lToken->name == IF || lToken->name == RETURN || lToken->name == WHILE || lToken->name == LBRACE || lToken->name == RBRACE)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::returnTail()
{
	if (lToken->name == NOT || lToken->name == LPAREN || lToken->name == MINUS || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		expression();
		match(SEMICOLON);
	}
	else if (lToken->name == SEMICOLON)
	{
		match(SEMICOLON);
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::forInit()
{
	if (lToken->name == IDENTIFIER)
	{
		match(IDENTIFIER);
		idStatementTail();
	}
	else if (lToken->name == SEMICOLON)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::forCondition()
{
	if (lToken->name == NOT || lToken->name == LPAREN || lToken->name == MINUS || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		expression();
	}
	else if (lToken->name == SEMICOLON)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::forUpdate()
{
	if (lToken->name == IDENTIFIER)
	{
		match(IDENTIFIER);
		idStatementTail();
	}
	else if (lToken->name == RPAREN)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::statementList()
{
	if (lToken->name == SEMICOLON || lToken->name == IDENTIFIER || lToken->name == FOR || lToken->name == IF || lToken->name == RETURN || lToken->name == WHILE || lToken->name == LBRACE)
	{
		statement();
		statementList();
	}
	else if (lToken->name == RBRACE)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::idStatementTail()
{
	if (lToken->name == LBRACKET)
	{
		match(LBRACKET);
		expression();
		match(RBRACKET);
		match(EQUALS);
		expression();
	}
	else if (lToken->name == EQUALS)
	{
		match(EQUALS);
		expression();
	}
	else if (lToken->name == LPAREN)
	{
		match(LPAREN);
		expression_list();
		match(RPAREN);
	}
	else if (lToken->name == SEMICOLON || lToken->name == RBRACKET || lToken->name == EQUALS || lToken->name == LPAREN)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::expression()
{
	if (lToken->name == NOT || lToken->name == LPAREN || lToken->name == MINUS || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		expression1();
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

// Parte do Cassiano
void Parser::expression1()
{
	if (lToken->name == OR)
	{
		match(OR);
		espr_and();
		expression1();
	}
	else if (lToken->name == SEMICOLON || lToken->name == COMMA || lToken->name == RPAREN || lToken->name == RBRACKET || lToken->name == RBRACE)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::espr_and()
{
	if (lToken->name == NOT || lToken->name == LPAREN || lToken->name == MINUS || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		espr_rel();
		espr_and1();
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::espr_and1()
{
	if (lToken->name == AND)
	{
		match(AND);
		espr_rel();
		espr_and1();
	}
	else if (lToken->name == SEMICOLON || lToken->name == COMMA || lToken->name == RPAREN || lToken->name == RBRACKET || lToken->name == RBRACE || lToken->name == OR)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}


void Parser::espr_rel()
{
	if (lToken->name == NOT || lToken->name == LPAREN || lToken->name == MINUS || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		espr_add();
		espr_rel1();
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}


void Parser::espr_rel1()
{
	if (lToken->name == EQUALS_EQUALS || lToken->name == NOT_EQUALS || lToken->name == LESS || lToken->name == LESS_EQUALS || lToken->name == GREATER || lToken->name == GREATER_EQUALS)
	{
		match(lToken->name); // RelOp
		espr_add();
		espr_rel1();
	}
	else if (lToken->name == AND || lToken->name == RPAREN || lToken->name == COMMA || lToken->name == SEMICOLON || lToken->name == RBRACKET || lToken->name == RBRACE || lToken->name == OR)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::espr_add()
{
	if (lToken->name == NOT || lToken->name == LPAREN || lToken->name == MINUS || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		espr_mult();
		espr_add1();
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::espr_add1()
{
	if (lToken->name == PLUS || lToken->name == MINUS)
	{
		match(lToken->name); // AddOp
		espr_mult();
		espr_add1();
	}
	else if (lToken->name == EQUALS_EQUALS || lToken->name == NOT_EQUALS || lToken->name == LESS || 
		lToken->name == LESS_EQUALS || lToken->name == GREATER || lToken->name == GREATER_EQUALS || 
		lToken->name == AND || lToken->name == RPAREN || lToken->name == COMMA || lToken->name == SEMICOLON || 
		lToken->name == RBRACKET || lToken->name == RBRACE || lToken->name == OR)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::espr_mult()
{
	if (lToken->name == NOT || lToken->name == LPAREN || lToken->name == MINUS || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		espr_unary();
		espr_mult1();
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::espr_mult1()
{
	if (lToken->name == MULTIPLY || lToken->name == DIVIDE)
	{
		match(lToken->name); // MultOp
		espr_unary();
		espr_mult1();
	}
	else if (lToken->name == EQUALS_EQUALS || lToken->name == NOT_EQUALS || lToken->name == LESS || 
		lToken->name == LESS_EQUALS || lToken->name == GREATER || lToken->name == GREATER_EQUALS || 
		lToken->name == AND || lToken->name == RPAREN || lToken->name == PLUS || lToken->name == COMMA || 
		lToken->name == MINUS || lToken->name == SEMICOLON || lToken->name == RBRACKET || lToken->name == RBRACE || 
		lToken->name == OR)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::espr_unary()
{
	if (lToken->name == MINUS)
	{
		match(MINUS);
		espr_unary();
	}
	else if (lToken->name == NOT)
	{
		match(NOT);
		espr_unary();
	}
	else if (lToken->name == LPAREN || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		primary();
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::primary()
{
	if (lToken->name == IDENTIFIER)
	{
		match(IDENTIFIER);
		primary_tail();
	}
	else if (lToken->name == LPAREN)
	{
		match(LPAREN);
		expression1();
		match(RPAREN);
	}
	else if (lToken->name == INTCONST)
	{
		match(INTCONST);
	}
	else if (lToken->name == CHARCONST)
	{
		match(CHARCONST);
	}
	else if (lToken->name == STRINGCONST)
	{
		match(STRINGCONST);
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}


void Parser::primary_tail()
{
	if (lToken->name == LPAREN)
	{
		match(LPAREN);
		expression_list();
		match(RPAREN);
	}
	else if (lToken->name == LBRACKET)
	{
		match(LBRACKET);
		expression1();
		match(RBRACKET);
	}
	else if (lToken->name == EQUALS_EQUALS || lToken->name == NOT_EQUALS || lToken->name == LESS || 
		lToken->name == LESS_EQUALS || lToken->name == GREATER || lToken->name == GREATER_EQUALS || 
		lToken->name == AND || lToken->name == RPAREN || lToken->name == MULTIPLY || lToken->name == PLUS || 
		lToken->name == COMMA || lToken->name == MINUS || lToken->name == DIVIDE || lToken->name == SEMICOLON || 
		lToken->name == RBRACKET || lToken->name == RBRACE || lToken->name == OR)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::expression_list()
{
	if (lToken->name == NOT || lToken->name == LPAREN || lToken->name == MINUS || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		expression1();
		expression_list_tail();
	}
	else if (lToken->name == RPAREN)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::expression_list_tail()
{
	if (lToken->name == COMMA)
	{
		match(COMMA);
		expression1();
		expression_list_tail();
	}
	else if (lToken->name == RPAREN)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::add_op()
{
	if (lToken->name == PLUS)
	{
		match(PLUS);
	}
	else if (lToken->name == MINUS)
	{
		match(MINUS);
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::mult_op()
{
	if (lToken->name == MULTIPLY)
	{
		match(MULTIPLY);
	}
	else if (lToken->name == DIVIDE)
	{
		match(DIVIDE);
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::rel_op()
{
	if (lToken->name == EQUALS_EQUALS)
	{
		match(EQUALS_EQUALS);
	}
	else if (lToken->name == NOT_EQUALS)
	{
		match(NOT_EQUALS);
	}
	else if (lToken->name == LESS)
	{
		match(LESS);
	}
	else if (lToken->name == LESS_EQUALS)
	{
		match(LESS_EQUALS);
	}
	else if (lToken->name == GREATER)
	{
		match(GREATER);
	}
	else if (lToken->name == GREATER_EQUALS)
	{
		match(GREATER_EQUALS);
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::initializeSymbolTable()
{
	Token* t;

	t = new Token(CHAR, "char");
	globalTable->add(new STEntry(t,true));
	t = new Token(INT, "int");
	globalTable->add(new STEntry(t,true));
	t = new Token(VOID, "void");
	globalTable->add(new STEntry(t,true));
	t = new Token(IF, "if");
	globalTable->add(new STEntry(t,true));
	t = new Token(ELSE, "else");
	globalTable->add(new STEntry(t,true));
	t = new Token(WHILE, "while");
	globalTable->add(new STEntry(t,true));
	t = new Token(FOR, "for");
	globalTable->add(new STEntry(t,true));
	t = new Token(RETURN, "return");
	globalTable->add(new STEntry(t,true));
}

void
Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}
