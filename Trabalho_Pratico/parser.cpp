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
// ==========================================
// Parte do Felipe
// ==========================================

void Parser::program() {
    if (lToken->name == END_OF_FILE || lToken->name == CHAR || lToken->name == INT || lToken->name == VOID) {
        functionlist();
        match(END_OF_FILE);
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::functionlist() {
    if (lToken->name == CHAR || lToken->name == INT || lToken->name == VOID) {
        function();
        functionlist();
    } else if (lToken->name == END_OF_FILE) {
        // ε
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::function() {
    if (lToken->name == CHAR || lToken->name == INT) {
        type();
        match(IDENTIFIER);
        match(LPAREN);
        paramtypes();
        match(RPAREN);
        match(LBRACE);
        declarations();
        statementlist();
        match(RBRACE);
    } else if (lToken->name == VOID) {
        match(VOID);
        match(IDENTIFIER);
        match(LPAREN);
        paramtypes();
        match(RPAREN);
        match(LBRACE);
        declarations();
        statementlist();
        match(RBRACE);
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::declarations() {
    if (lToken->name == CHAR || lToken->name == INT) {
        type();
        vardeclaration();
        vardeclarationlist();
        match(SEMICOLON);
        declarations();
    } else if (lToken->name == SEMICOLON || lToken->name == IDENTIFIER || 
               lToken->name == FOR || lToken->name == IF || 
               lToken->name == RETURN || lToken->name == WHILE || 
               lToken->name == LBRACE || lToken->name == RBRACE) {
        // ε
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::vardeclarationlist() {
    if (lToken->name == COMMA) {
        match(COMMA);
        vardeclaration();
        vardeclarationlist();
    } else if (lToken->name == SEMICOLON) {
        // ε
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::vardeclaration() {
    if (lToken->name == IDENTIFIER) {
        match(IDENTIFIER);
        vararraytail();
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::vararraytail() {
    if (lToken->name == LBRACKET) {
        match(LBRACKET);
        match(INTCONST);
        match(RBRACKET);
    } else if (lToken->name == COMMA || lToken->name == SEMICOLON) {
        // ε
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::type() {
    if (lToken->name == CHAR) {
        match(CHAR);
    } else if (lToken->name == INT) {
        match(INT);
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::paramtypes() {
    if (lToken->name == VOID) {
        match(VOID);
    } else if (lToken->name == CHAR || lToken->name == INT) {
        type();
        match(IDENTIFIER);
        paramarraytail();
        paramtypestail();
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::paramarraytail() {
    if (lToken->name == LBRACKET) {
        match(LBRACKET);
        match(RBRACKET);
    } else if (lToken->name == RPAREN || lToken->name == COMMA) {
        // ε
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::paramtypestail() {
    if (lToken->name == COMMA) {
        match(COMMA);
        type();
        match(IDENTIFIER);
        paramarraytail();
        paramtypestail();
    } else if (lToken->name == RPAREN) {
        // ε
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::statement() {
    if (lToken->name == IF) {
        match(IF);
        match(LPAREN);
        expression();
        match(RPAREN);
        statement();
        iftail();
    } else if (lToken->name == WHILE) {
        match(WHILE);
        match(LPAREN);
        expression();
        match(RPAREN);
        statement();
    } else if (lToken->name == FOR) {
        match(FOR);
        match(LPAREN);
        forinit();
        match(SEMICOLON);
        forcondition();
        match(SEMICOLON);
        forupdate();
        match(RPAREN);
        statement();
    } else if (lToken->name == RETURN) {
        match(RETURN);
        returntail();
    } else if (lToken->name == IDENTIFIER) {
        match(IDENTIFIER);
        idstatementtail();
        match(SEMICOLON);
    } else if (lToken->name == LBRACE) {
        match(LBRACE);
        statementlist();
        match(RBRACE);
    } else if (lToken->name == SEMICOLON) {
        match(SEMICOLON);
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::iftail() {
    if (lToken->name == ELSE) {
        match(ELSE);
        statement();
    } else if (lToken->name == SEMICOLON || lToken->name == IDENTIFIER || 
               lToken->name == ELSE || lToken->name == FOR || 
               lToken->name == IF || lToken->name == RETURN || 
               lToken->name == WHILE || lToken->name == LBRACE || 
               lToken->name == RBRACE) {
        // ε
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::returntail() {
    if (lToken->attribute == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || 
        lToken->name == IDENTIFIER || lToken->name == CHARCONST || 
        lToken->name == INTCONST || lToken->name == STRINGCONST) {
        expression();
        match(SEMICOLON);
    } else if (lToken->name == SEMICOLON) {
        match(SEMICOLON);
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::forinit() {
    if (lToken->name == IDENTIFIER) {
        match(IDENTIFIER);
        idstatementtail();
    } else if (lToken->name == SEMICOLON) {
        // ε
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::forcondition() {
    if (lToken->attribute == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || 
        lToken->name == IDENTIFIER || lToken->name == CHARCONST || 
        lToken->name == INTCONST || lToken->name == STRINGCONST) {
        expression();
    } else if (lToken->name == SEMICOLON) {
        // ε
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::forupdate() {
    if (lToken->name == IDENTIFIER) {
        match(IDENTIFIER);
        idstatementtail();
    } else if (lToken->name == RPAREN) {
        // ε
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::statementlist() {
    if (lToken->name == SEMICOLON || lToken->name == IDENTIFIER || lToken->name == FOR || 
        lToken->name == IF || lToken->name == RETURN || lToken->name == WHILE || 
        lToken->name == LBRACE) {
        statement();
        statementlist();
    } else if (lToken->name == RBRACE) {
        // ε
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::idstatementtail() {
    if (lToken->name == LBRACKET) {
        match(LBRACKET);
        expression();
        match(RBRACKET);
        match(EQUALS); 
        expression();
    } else if (lToken->name == EQUALS || lToken->attribute == EQUALS) {
        match(EQUALS);
        expression();
    } else if (lToken->name == LPAREN) {
        match(LPAREN);
        expression_list();
        match(RPAREN);
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}

void Parser::expression() {
    if (lToken->attribute == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || 
        lToken->name == IDENTIFIER || lToken->name == CHARCONST || 
        lToken->name == INTCONST || lToken->name == STRINGCONST) {
        espr_and();
        expression1();
    } else {
        error("Token inesperado: " + lToken->lexeme);
    }
}
// Parte do Cassiano
void Parser::expression1()
{
	if (lToken->attribute == OR)
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
	if (lToken->attribute == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		espr_rel();
		espr_and1();
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::espr_and1()
{
	if (lToken->attribute == AND)
	{
		match(AND);
		espr_rel();
		espr_and1();
	}
	else if (lToken->name == SEMICOLON || lToken->name == COMMA || lToken->name == RPAREN || lToken->name == RBRACKET || lToken->name == RBRACE || lToken->attribute == OR)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}


void Parser::espr_rel()
{
	if (lToken->attribute == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		espr_add();
		espr_rel1();
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}


void Parser::espr_rel1()
{
	if (lToken->attribute == EQUALS_EQUALS || lToken->attribute == NOT_EQUALS || lToken->attribute == LESS || lToken->attribute == LESS_EQUALS || lToken->attribute == GREATER || lToken->attribute == GREATER_EQUALS)
	{
		match(lToken->attribute); // RelOp
		espr_add();
		espr_rel1();
	}
	else if (lToken->attribute == AND || lToken->name == RPAREN || lToken->name == COMMA || lToken->name == SEMICOLON || lToken->name == RBRACKET || lToken->name == RBRACE || lToken->attribute == OR)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::espr_add()
{
	if (lToken->attribute == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		espr_mult();
		espr_add1();
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::espr_add1()
{
	if (lToken->attribute == PLUS || lToken->attribute == MINUS)
	{
		match(lToken->attribute); // AddOp
		espr_mult();
		espr_add1();
	}
	else if (lToken->attribute == EQUALS_EQUALS || lToken->attribute == NOT_EQUALS || lToken->attribute == LESS ||
			 lToken->attribute == LESS_EQUALS || lToken->attribute == GREATER || lToken->attribute == GREATER_EQUALS ||
			 lToken->attribute == AND || lToken->name == RPAREN || lToken->name == COMMA || lToken->name == SEMICOLON ||
			 lToken->name == RBRACKET || lToken->name == RBRACE || lToken->attribute == OR)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::espr_mult()
{
	if (lToken->attribute == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		espr_unary();
		espr_mult1();
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::espr_mult1()
{
	if (lToken->attribute == MULTIPLY || lToken->attribute == DIVIDE)
	{
		match(lToken->attribute); // MultOp
		espr_unary();
		espr_mult1();
	}
	else if (lToken->attribute == EQUALS_EQUALS || lToken->attribute == NOT_EQUALS || lToken->attribute == LESS ||
			 lToken->attribute == LESS_EQUALS || lToken->attribute == GREATER || lToken->attribute == GREATER_EQUALS ||
			 lToken->attribute == AND || lToken->name == RPAREN || lToken->attribute == PLUS || lToken->name == COMMA ||
			 lToken->attribute == MINUS || lToken->name == SEMICOLON || lToken->name == RBRACKET || lToken->name == RBRACE ||
			 lToken->attribute == OR)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::espr_unary()
{
	if (lToken->attribute == MINUS)
	{
		match(MINUS);
		espr_unary();
	}
	else if (lToken->attribute == NOT)
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
		expression();
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
		expression();
		match(RBRACKET);
	}
	else if (lToken->attribute == EQUALS_EQUALS || lToken->attribute == NOT_EQUALS || lToken->attribute == LESS ||
			 lToken->attribute == LESS_EQUALS || lToken->attribute == GREATER || lToken->attribute == GREATER_EQUALS ||
			 lToken->attribute == AND || lToken->name == RPAREN || lToken->attribute == MULTIPLY || lToken->attribute == PLUS ||
			 lToken->name == COMMA || lToken->attribute == MINUS || lToken->attribute == DIVIDE || lToken->name == SEMICOLON ||
			 lToken->name == RBRACKET || lToken->name == RBRACE || lToken->attribute == OR)
	{
		// ε
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::expression_list()
{
	if (lToken->attribute == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || lToken->name == IDENTIFIER || 
		lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
	{
		expression();
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
		expression();
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
	if (lToken->attribute == PLUS)
	{
		match(PLUS);
	}
	else if (lToken->attribute == MINUS)
	{
		match(MINUS);
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::mult_op()
{
	if (lToken->attribute == MULTIPLY)
	{
		match(MULTIPLY);
	}
	else if (lToken->attribute == DIVIDE)
	{
		match(DIVIDE);
	}
	else
		error("Token inesperado: " + lToken->lexeme);
}

void Parser::rel_op()
{
	if (lToken->attribute == EQUALS_EQUALS)
	{
		match(EQUALS_EQUALS);
	}
	else if (lToken->attribute == NOT_EQUALS)
	{
		match(NOT_EQUALS);
	}
	else if (lToken->attribute == LESS)
	{
		match(LESS);
	}
	else if (lToken->attribute == LESS_EQUALS)
	{
		match(LESS_EQUALS);
	}
	else if (lToken->attribute == GREATER)
	{
		match(GREATER);
	}
	else if (lToken->attribute == GREATER_EQUALS)
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

void Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;
	exit(EXIT_FAILURE);
}
