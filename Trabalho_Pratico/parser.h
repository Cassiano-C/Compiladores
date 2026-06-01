#include "scanner.h"

/*
--- Parte do Felipe ---
Program       → FunctionList EOF
FunctionList  → Function FunctionList | ε

Function      → Type ID ( ParamTypes ) { Declarations StatementList '
| void ID ( ParamTypes ) { Declarations StatementList }

Declarations  → Type VarDeclaration VarDeclarationList ; Declarations
| ε
VarDeclarationList → , VarDeclaration VarDeclarationList | ε
VarDeclaration → ID VarArrayTail
VarArrayTail   → [ integerconstant ] | ε

Type          → char | int

ParamTypes    → void | Type ID ParamArrayTail ParamTypesTail

ParamArrayTail→ [ ] | ε
ParamTypesTail→ , Type ID ParamArrayTail ParamTypesTail | ε

Statement     → if ( Expression ) Statement IfTail
| while ( Expression ) Statement
| for ( ForInit ; ForCondition ; ForUpdate ) Statement
| return ReturnTail
| ID IdStatementTail ;
| '{' StatementList '}'
| ;

IfTail        → else Statement | ε
ReturnTail    → Expression ; | ;
ForInit       → ID IdStatementTail | ε
ForCondition  → Expression | ε
ForUpdate     → ID IdStatementTail | ε
StatementList → Statement StatementList | ε


IdStatementTail → [ Expression ] = Expression
                | = Expression
                | ( ExpressionList )

Expression    → Expr_And Expression’

---- minha parte ----
Expression’   → || Expr_And Expression’ | ε

Expr_And      → Expr_Rel Expr_And’
Expr_And’     → && Expr_Rel Expr_And’ | ε

Expr_Rel      → Expr_Add Expr_Rel’
Expr_Rel’     → RelOp Expr_Add Expr_Rel’ | ε

Expr_Add      → Expr_Mult Expr_Add’
Expr_Add’     → AddOp Expr_Mult Expr_Add’ | ε

Expr_Mult     → Expr_Unary Expr_Mult’
Expr_Mult’    → MultOp Expr_Unary Expr_Mult’ | ε

Expr_Unary    → - Expr_Unary 
              | ! Expr_Unary 
              | Primary

Primary       → ID PrimarTail 
              | ( Expression ) 
              | integerconstant 
              | charconstant 
              | stringconstant

PrimarTail    → ( ExpressionList ) | [ Expression ] | ε

ExpressionList → Expression ExpressionListTail | ε
ExpressionListTail → , Expression ExpressionListTail | ε

AddOp         → + | - 
MultOp        → * | / 
RelOp         → == | != | <= | < | >= | >

*/


class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;
		SymbolTable* globalTable;
		SymbolTable* currentTable;

		void advance();
		void match(int);

		// Parte do Felipe
		void program();
		void functionlist();
		void function();
		void declarations();
		void varDeclarationList();
		void varDeclaration();
		void varArrayTail();
		void type();
		void paramTypes();
		void paramArrayTail();
		void paramTypesTail();
		void statement();
		void ifTail();
		void returnTail();
		void forInit();
		void forCondition();
		void forUpdate();
		void statementList();
		void idStatementTail();
		void expression();

		// Parte cassiano
        void expression1();
		void espr_and();
		void espr_and1();
		void espr_rel();
		void espr_rel1();
		void espr_add();
		void espr_add1();
		void espr_mult();
		void espr_mult1();
		void espr_unary();
		void primary();
		void primary_tail();
		void expression_list();
		void expression_list_tail();
		void add_op();
		void mult_op();
		void rel_op();
		
	public:
		Parser(string);
		void run();
		void initializeSymbolTable();


        void error(string);
};