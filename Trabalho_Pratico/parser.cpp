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
    if (lToken->name == t || lToken->attribute == t) {
        advance();
    } else {
		// Mensagem de erro mais detalhada, mostrando o token esperado e o token recebido
        error("Erro sintatico: Esperado o token ID (" + to_string(t) + "), mas recebeu '" + lToken->lexeme + "'"); 
		// E chama a função de sincronização para tentar recuperar o erro e continuar a análise sintática
		// Na onde o conjunto de sincronização inclui os tokens que poderiam seguir o token esperado, para tentar encontrar um ponto de recuperação
        synchronize({t, SEMICOLON, RBRACE});
    }
}

void Parser::run()
{
    advance();  

    program();
    
    if (errorCount == 0) {
        cout << "\nCompilacao encerrada com sucesso!\n";
    } else {
        cout << "\nCompilacao encerrada. Foram encontrados " << errorCount + scanner->contagen_erros << " erro(s).\n";
    }
}

// Parte do Felipe
// ==========================================

void Parser::program() {
    if (lToken->name == END_OF_FILE || lToken->name == CHAR || lToken->name == INT || lToken->name == VOID) {
        functionlist();
        match(END_OF_FILE);
    } else {
        error("Erro no escopo principal do programa: " + lToken->lexeme);
        synchronize({END_OF_FILE});
    }
}

void Parser::functionlist() {
    if (lToken->name == CHAR || lToken->name == INT || lToken->name == VOID) {
        function();
        functionlist();
    } else if (lToken->name == END_OF_FILE) {
        // ε
    } else {
        error("Erro na declaracao da lista de funcoes: " + lToken->lexeme);
        synchronize({END_OF_FILE});
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
        error("Erro sintatico na definicao da funcao: " + lToken->lexeme);
        synchronize({END_OF_FILE});
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
        error("Erro nas declaracoes de variaveis locais: " + lToken->lexeme);
        synchronize({SEMICOLON, IDENTIFIER, FOR, IF, RETURN, WHILE, LBRACE, RBRACE});
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
        error("Erro na lista de delimitadores de declaracoes de variaveis: " + lToken->lexeme);
        synchronize({SEMICOLON, IDENTIFIER, FOR, IF, RETURN, WHILE, LBRACE});
    }
}

void Parser::vardeclaration() {
    if (lToken->name == IDENTIFIER) {
        match(IDENTIFIER);
        vararraytail();
    } else {
        error("Erro na declaracao: Esperado um identificador, mas obteve " + lToken->lexeme);
        synchronize({SEMICOLON, IDENTIFIER, FOR, IF, RETURN, WHILE, LBRACE});
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
        error("Erro na definicao de dimensao do arranjo (array): " + lToken->lexeme);
        synchronize({SEMICOLON, IDENTIFIER, FOR, IF, RETURN, WHILE, LBRACE});
    }
}

void Parser::type() {
    if (lToken->name == CHAR) {
        match(CHAR);
    } else if (lToken->name == INT) {
        match(INT);
    } else {
        error("Erro de tipo: Esperado 'int' ou 'char', mas obteve " + lToken->lexeme);
        synchronize({SEMICOLON, IDENTIFIER, FOR, IF, RETURN, WHILE, LBRACE});
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
        error("Erro nos parametros da funcao: " + lToken->lexeme);
        synchronize({RPAREN, SEMICOLON, LBRACE});
    }
}

void Parser::paramarraytail() {
    if (lToken->name == LBRACKET) {
        match(LBRACKET);
        match(RBRACKET);
    } else if (lToken->name == RPAREN || lToken->name == COMMA) {
        // ε
    } else {
        error("Erro no fechamento do colchete do parametro: " + lToken->lexeme);
        synchronize({RPAREN, COMMA, SEMICOLON});
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
        error("Erro na sintaxe de delimitacao de parametros da funcao: " + lToken->lexeme);
        synchronize({RPAREN, SEMICOLON});
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
        error("Erro no comando (statement): Comando invalido ou token inesperado '" + lToken->lexeme + "'");
        synchronize({SEMICOLON, IDENTIFIER, FOR, IF, RETURN, WHILE, LBRACE, RBRACE});
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
        error("Erro na clausula alternativa if-else (iftail): " + lToken->lexeme);
        synchronize({SEMICOLON, IDENTIFIER, FOR, IF, RETURN, WHILE, LBRACE, RBRACE});
    }
}

void Parser::returntail() {
    if (lToken->attribute == NOT || lToken->name == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || 
        lToken->name == IDENTIFIER || lToken->name == CHARCONST || 
        lToken->name == INTCONST || lToken->name == STRINGCONST) {
        expression();
        match(SEMICOLON);
    } else if (lToken->name == SEMICOLON) {
        match(SEMICOLON);
    } else {
        error("Erro no retorno da funcao: Expressao invalida apos 'return': " + lToken->lexeme);
        synchronize({SEMICOLON, RBRACE, IDENTIFIER});
    }
}

void Parser::forinit() {
    if (lToken->name == IDENTIFIER) {
        match(IDENTIFIER);
        idstatementtail();
    } else if (lToken->name == SEMICOLON) {
        // ε
    } else {
        error("Erro na inicializacao do laço for: " + lToken->lexeme);
        synchronize({SEMICOLON, RPAREN});
    }
}

void Parser::forcondition() {
    if (lToken->attribute == NOT || lToken->name == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || 
        lToken->name == IDENTIFIER || lToken->name == CHARCONST || 
        lToken->name == INTCONST || lToken->name == STRINGCONST) {
        expression();
    } else if (lToken->name == SEMICOLON) {
        // ε
    } else {
        error("Erro na condicao do laço for: " + lToken->lexeme);
        synchronize({SEMICOLON, RPAREN});
    }
}

void Parser::forupdate() {
    if (lToken->name == IDENTIFIER) {
        match(IDENTIFIER);
        idstatementtail();
    } else if (lToken->name == RPAREN) {
        // ε
    } else {
        error("Erro no incremento/passo do laço for: " + lToken->lexeme);
        synchronize({RPAREN, SEMICOLON});
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
        error("Erro na lista de comandos do bloco: " + lToken->lexeme);
        synchronize({RBRACE, END_OF_FILE});
    }
}

void Parser::idstatementtail() {
    if (lToken->name == LBRACKET) {
        match(LBRACKET);
        expression();
        match(RBRACKET);
        match(EQUALS); 
        expression();
    } else if (lToken->attribute == EQUALS || lToken->name == EQUALS) {
        match(lToken->name == EQUALS ? EQUALS : lToken->attribute);
        expression();
    } else if (lToken->name == LPAREN) {
        match(LPAREN);
        expression_list();
        match(RPAREN);
    } else {
        error("Erro na atribuicao ou chamada de funcao a partir de ID: " + lToken->lexeme);
        synchronize({SEMICOLON, RPAREN, RBRACE});
    }
}

// Parte do Cassiano (Expressões com Sincronização Localizada)
// ==========================================

void Parser::expression() {
    if (lToken->attribute == NOT || lToken->name == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || 
        lToken->name == IDENTIFIER || lToken->name == CHARCONST || 
        lToken->name == INTCONST || lToken->name == STRINGCONST) {
        espr_and();
        expression1();
    } else {
        error("Erro expression: Expressao mal formada perto de '" + lToken->lexeme + "'");
        synchronize({SEMICOLON, COMMA, RPAREN, RBRACE, RBRACKET});
    }
}

void Parser::expression1()
{
    if (lToken->attribute == OR || lToken->name == OR)
    {
        match(lToken->name == OR ? OR : lToken->attribute);
        espr_and();
        expression1();
    }
    else if (lToken->name == SEMICOLON || lToken->name == COMMA || lToken->name == RPAREN || lToken->name == RBRACKET || lToken->name == RBRACE)
    {
        // ε
    }
    else {
        error("Erro expression1: Token inesperado em expressao ou logica '||': " + lToken->lexeme);   
        synchronize({SEMICOLON, COMMA, RPAREN, RBRACE, RBRACKET});
    }
}

void Parser::espr_and()
{
    if (lToken->attribute == NOT || lToken->name == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || 
        lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
    {
        espr_rel();
        espr_and1();
    }
    else {
        error("Erro espr_and: Operando ou termo logico invalido perto de '" + lToken->lexeme + "'");  
        synchronize({SEMICOLON, COMMA, RPAREN, RBRACE, RBRACKET});
    }
}

void Parser::espr_and1()
{
    if (lToken->attribute == AND || lToken->name == AND)
    {
        match(lToken->name == AND ? AND : lToken->attribute);
        espr_rel();
        espr_and1();
    }
    else if (lToken->name == SEMICOLON || lToken->name == COMMA || lToken->name == RPAREN || lToken->name == RBRACKET || lToken->name == RBRACE || lToken->attribute == OR || lToken->name == OR)
    {
        // ε
    }
    else {
        error("Erro espr_and1: Token inesperado apos operador logico '&&': " + lToken->lexeme); 
        synchronize({SEMICOLON, COMMA, RPAREN, RBRACE, RBRACKET});
    }
}

void Parser::espr_rel()
{
    if (lToken->attribute == NOT || lToken->name == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || 
        lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
    {
        espr_add();
        espr_rel1();
    }
    else {
        error("Erro espr_rel: Expressao relacional invalida perto de '" + lToken->lexeme + "'");  
        synchronize({SEMICOLON, COMMA, RPAREN, RBRACE, RBRACKET});
    }
}

void Parser::espr_rel1()
{
    if (lToken->attribute == EQUALS_EQUALS || lToken->attribute == NOT_EQUALS || lToken->attribute == LESS || lToken->attribute == LESS_EQUALS || lToken->attribute == GREATER || lToken->attribute == GREATER_EQUALS)
    {
        match(lToken->attribute); // RelOp
        espr_add();
        espr_rel1();
    }
    else if (lToken->attribute == AND || lToken->name == AND || lToken->name == RPAREN || lToken->name == COMMA || lToken->name == SEMICOLON || lToken->name == RBRACKET || lToken->name == RBRACE || lToken->attribute == OR || lToken->name == OR)
    {
        // ε
    }
    else {
        error("Erro espr_rel1: Operador relacional inesperado ou mal formado: " + lToken->lexeme); 
        synchronize({SEMICOLON, COMMA, RPAREN, RBRACE, RBRACKET});
    }
}

void Parser::espr_add()
{
    if (lToken->attribute == NOT || lToken->name == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || 
        lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
    {
        espr_mult();
        espr_add1();
    }
    else {
        error("Erro espr_add: Expressao aritmetica aditiva invalida perto de '" + lToken->lexeme + "'");  
        synchronize({SEMICOLON, COMMA, RPAREN, RBRACE, RBRACKET});
    }
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
             lToken->attribute == AND || lToken->name == AND || lToken->name == RPAREN || lToken->name == COMMA || lToken->name == SEMICOLON ||
             lToken->name == RBRACKET || lToken->name == RBRACE || lToken->attribute == OR || lToken->name == OR)
    {
        // ε
    }
    else {
        error("Erro espr_add1: Token inesperado em operacao de adicao/subtracao: " + lToken->lexeme); 
        synchronize({SEMICOLON, COMMA, RPAREN, RBRACE, RBRACKET});
    }
}

void Parser::espr_mult()
{
    if (lToken->attribute == NOT || lToken->name == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || 
        lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
    {
        espr_unary();
        espr_mult1();
    }
    else {
        error("Erro espr_mult: Fator aritmetico multiplicativo invalido perto de '" + lToken->lexeme + "'"); 
        synchronize({SEMICOLON, COMMA, RPAREN, RBRACE, RBRACKET});
    }
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
             lToken->attribute == AND || lToken->name == AND || lToken->name == RPAREN || lToken->attribute == PLUS || lToken->name == COMMA ||
             lToken->attribute == MINUS || lToken->name == SEMICOLON || lToken->name == RBRACKET || lToken->name == RBRACE ||
             lToken->attribute == OR || lToken->name == OR)
    {
        // ε
    }
    else {
        error("Erro espr_mult1: Token inesperado em operacao de multiplicacao/divisao: " + lToken->lexeme);    
        synchronize({SEMICOLON, COMMA, RPAREN, RBRACE, RBRACKET});
    }
}

void Parser::espr_unary()
{
    if (lToken->attribute == MINUS)
    {
        match(MINUS);
        espr_unary();
    }
    else if (lToken->attribute == NOT || lToken->name == NOT)
    {
        match(lToken->name == NOT ? NOT : lToken->attribute);
        espr_unary();
    }
    else if (lToken->name == LPAREN || lToken->name == IDENTIFIER || lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
    {
        primary();
    }
    else {
        error("Erro espr_unary: Operador unario ('-' ou '!') invalido ou mal posicionado: " + lToken->lexeme);    
        synchronize({SEMICOLON, COMMA, RPAREN, RBRACE, RBRACKET});
    }
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
    else if (lToken->name == INTCONST || lToken->name == CHARCONST || lToken->name == STRINGCONST)
    {
        advance(); // Consome o token primitivo de constante com seguranca
    }
    else {
        error("Erro primary: Esperado um identificador, numero literal ou '(' mas obteve '" + lToken->lexeme + "'");   
        synchronize({SEMICOLON, COMMA, RPAREN, RBRACE, RBRACKET});
    }
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
             lToken->attribute == AND || lToken->name == AND || lToken->name == RPAREN || lToken->attribute == MULTIPLY || lToken->attribute == PLUS ||
             lToken->name == COMMA || lToken->attribute == MINUS || lToken->attribute == DIVIDE || lToken->name == SEMICOLON ||
             lToken->name == RBRACKET || lToken->name == RBRACE || lToken->attribute == OR || lToken->name == OR)
    {
        // ε
    }
    else {
        error("Erro primary_tail: Sufixo invalido em expressao de chamada ou arranjo: " + lToken->lexeme);  
        synchronize({SEMICOLON, COMMA, RPAREN, RBRACE, RBRACKET});
    }
}

void Parser::expression_list()
{
    if (lToken->attribute == NOT || lToken->name == NOT || lToken->name == LPAREN || lToken->attribute == MINUS || lToken->name == IDENTIFIER || 
        lToken->name == CHARCONST || lToken->name == INTCONST || lToken->name == STRINGCONST)
    {
        expression();
        expression_list_tail();
    }
    else if (lToken->name == RPAREN)
    {
        // ε
    }
    else {
        error("Erro expression_list: Argumento invalido na lista de parametros de chamada: " + lToken->lexeme);   
        synchronize({RPAREN, SEMICOLON});
    }
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
    else {
        error("Erro expression_list_tail: Esperado ',' ou ')' na listagem de argumentos: " + lToken->lexeme);  
        synchronize({RPAREN, SEMICOLON});
    }
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
    else {
        error("Erro add_op: Esperado '+' ou '-', mas obteve: " + lToken->lexeme);    
        synchronize({SEMICOLON, IDENTIFIER, LPAREN});
    }
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
    else {
        error("Erro mult_op: Esperado '*' ou '/', mas obteve: " + lToken->lexeme);   
        synchronize({SEMICOLON, IDENTIFIER, LPAREN});
    }
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
    else {
        error("Erro rel_op: Operador relacional invalido ou inesperado: " + lToken->lexeme);    
        synchronize({SEMICOLON, IDENTIFIER, LPAREN});
    }
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

/*
Modo de recuperação de erros: Modo Panic Mode

Vai mostra imadiatamente o erro encontrado,
mas ao invés de abortar a compilação, 
ele vai tentar recuperar o erro e continuar a análise sintática para encontrar mais erros no código-fonte.
*/

void Parser::error(string str)
{
    cout << "Linha " << scanner->getLine() << ": " << str << endl;
    errorCount++; // Incrementa, mas NAO fecha o programa imediatamente!
    
    // Se passarmos de 10 erros estruturais, paramos para evitar loops infinitos por seguranca
    if (errorCount >= 10) {
        cout << "Muitos erros sintaticos encontrados no arquivo. Abortando compilacao.\n";
        exit(EXIT_FAILURE);
    }
}

void Parser::synchronize(const std::vector<int>& syncTokens)
{
    while (lToken->name != END_OF_FILE) 
    {
        // Verifica se o token atual (name ou attribute) serve para sincronizar
        if (std::find(syncTokens.begin(), syncTokens.end(), lToken->name) != syncTokens.end() ||
            std::find(syncTokens.begin(), syncTokens.end(), lToken->attribute) != syncTokens.end())
        {
            // Encontrou um token de barreira de sincronia! Avança para reatar na proxima instrucao valida
            advance();
            return;
        }
        advance(); // Descarta o token invalido da pilha de entrada
    }
}