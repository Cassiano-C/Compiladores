#include "scanner.h"    

// Construtor que recebe uma string com o nome do arquivo 
// de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input, SymbolTable* st)
{
    pos = 0;
    line = 1;

    this->st = st;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile, line))
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else 
    {
        cout << "Unable to open file\n"; 
    }
}

int
Scanner::getLine()
{
    return line;
}

// Método que retorna o próximo token da entrada
Token* Scanner::nextToken()
{
    Token* tok = nullptr;
    string lexeme;

    while (pos < input.length() && isspace(input[pos])) // Ignora espaços em branco e as quebras de linha
    {
        if (input[pos] == '\n')
            line++;
        pos++;
    }
    
    if (pos >= input.length() || input[pos] == '\0')
    {
        tok = new Token(END_OF_FILE, "EOF");
    }
    else if (input[pos] == '"')
    {
        pos++; // pula a aspa inicial
        while (pos < input.length() && isprint(input[pos]) && input[pos] != '"')
        {
            if (input[pos] == '\n')
                line++;
            lexeme += input[pos];
            pos++;
        }
        pos++; // pula a aspa final
        tok = new Token(STRINGCONST, lexeme);
    }
    else if (input[pos] == '\'') // encontra as aspas simples
    {
        pos++; // pula a aspa inicial
        if (pos < input.length() && input[pos] == '\\')
        {
            pos++; // pula a barra invertida
            if (pos < input.length() && (input[pos] == 'n' || input[pos] == '0'))
            {
                lexeme += '\\'; // adiciona a barra invertida ao lexema
                lexeme += input[pos]; // adiciona o caractere de escape ao lexema
                pos++; // pula o caractere de escape
            }
            else
            {
                lexicalError("Caractere de escape inválido: \\" + string(1, input[pos]));
            }
        }
        else if (pos < input.length() && isprint(input[pos]) && input[pos] != '\'' && input[pos] != '\\')
        {
            lexeme += input[pos];
            pos++;
        }

        if (pos < input.length() && input[pos] == '\'')
        {
            pos++; // pula a aspa final
            tok = new Token(CHARCONST, lexeme);
        }
        else
        {
            lexicalError("Caractere inválido em literal de caractere: " + string(1, input[pos]));
        }
    }
    else if (input[pos] == '/')
    {
        if (pos + 1 < input.length() && input[pos + 1] == '/')
        {
            pos += 2; // vai pra posição do primeiro caractere após //
            while (pos < input.length() && input[pos] != '\n')
                pos++;
            if (pos < input.length() && input[pos] == '\n') {
                line++;
                pos++;
            }
            return nextToken();
        }
        else if (pos + 1 < input.length() && input[pos + 1] == '*')
        {
            pos += 2; // vai pra posição do primeiro caractere após /*
            while (pos + 1 < input.length() && (input[pos] != '*' || input[pos + 1] != '/'))
            {
                if (input[pos] == '\n')
                    line++;
                pos++;
            }
            pos += 2; // vai pra posição do primeiro caractere após */
            return nextToken();
        }
        else
        {
            tok = new Token(OPERATOR, "/");
            tok->attribute = DIVIDE;
            pos++;
        }
    }
    else if (isdigit(input[pos]))
    {
        while (pos < input.length() && isdigit(input[pos]))
        {
            lexeme += input[pos];
            pos++;
        }
        tok = new Token(INTCONST, lexeme);
    }
    else if (isalpha(input[pos]))
    {
        while (pos < input.length() && (isalpha(input[pos]) || isalnum(input[pos]) || input[pos] == '_'))
        {
            lexeme += input[pos];
            pos++;
        }
        
        STEntry* entry = st->get(lexeme);
        if (!entry)
        {
            tok = new Token(IDENTIFIER, lexeme);
        }
        else
        {
            tok = new Token(entry->token->name, lexeme);
        }
    }
    else if (input[pos] == '+')
    {
        tok = new Token(OPERATOR, "+");
        tok->attribute = PLUS;
        pos++;
    }
    else if (input[pos] == '-')
    {
        tok = new Token(OPERATOR, "-");
        tok->attribute = MINUS;
        pos++;
    }
    else if (input[pos] == '*')
    {
        tok = new Token(OPERATOR, "*");
        tok->attribute = MULTIPLY;
        pos++;
    }
    else if (input[pos] == '=')
    {
        if (pos + 1 < input.length() && input[pos + 1] == '=')
        {
            tok = new Token(OPERATOR, "==");
            tok->attribute = EQUALS_EQUALS;
            pos += 2;
        }
        else
        {
            tok = new Token(OPERATOR, "=");
            tok->attribute = EQUALS;
            pos++;
        }
    }
    else if (input[pos] == '!')
    {
        if (pos + 1 < input.length() && input[pos + 1] == '=')
        {
            tok = new Token(OPERATOR, "!=");
            tok->attribute = NOT_EQUALS;
            pos += 2;
        }
        else
        {
            tok = new Token(OPERATOR, "!");
            tok->attribute = NOT;
            pos++;
        }
    }
    else if (input[pos] == '<')
    {
        if (pos + 1 < input.length() && input[pos + 1] == '=')
        {
            tok = new Token(OPERATOR, "<=");
            tok->attribute = LESS_EQUALS;
            pos += 2;
        }
        else
        {
            tok = new Token(OPERATOR, "<");
            tok->attribute = LESS;
            pos++;
        }
    }
    else if (input[pos] == '>')
    {
        if (pos + 1 < input.length() && input[pos + 1] == '=')
        {
            tok = new Token(OPERATOR, ">=");
            tok->attribute = GREATER_EQUALS;
            pos += 2;
        }
        else
        {
            tok = new Token(OPERATOR, ">");
            tok->attribute = GREATER;
            pos++;
        }
    }
    else if (pos + 1 < input.length() && input[pos] == '&' && input[pos + 1] == '&')
    {
        tok = new Token(OPERATOR, "&&");
        tok->attribute = AND;
        pos += 2;
    }
    else if (pos + 1 < input.length() && input[pos] == '|' && input[pos + 1] == '|')
    {
        tok = new Token(OPERATOR, "||");
        tok->attribute = OR;
        pos += 2;
    }
    else if (input[pos] == '(')
    {
        tok = new Token(LPAREN, "(");
        pos++;
    }
    else if (input[pos] == ')')
    {
        tok = new Token(RPAREN, ")");
        pos++;
    }
    else if (input[pos] == '{')
    {
        tok = new Token(LBRACE, "{");
        pos++;
    }
    else if (input[pos] == '}')
    {
        tok = new Token(RBRACE, "}");
        pos++;
    }
    else if (input[pos] == '[')
    {
        tok = new Token(LBRACKET, "[");
        pos++;
    }
    else if (input[pos] == ']')
    {
        tok = new Token(RBRACKET, "]");
        pos++;
    }
    else if (input[pos] == ',')
    {
        tok = new Token(COMMA, ",");
        pos++;
    }
    else if (input[pos] == ';')
    {
        tok = new Token(SEMICOLON, ";");
        pos++;
    }
    else
    {
        // REPORTA O ERRO: Passa o caractere inválido para a mensagem
        lexicalError("Erro lexico: Caractere invalido '" + string(1, input[pos]) + "'");
        
        // Tenta ler o próximo token válido para não quebrar o Parser
        return nextToken();
    }
    return tok;
}
/*
Se acontecer um erro lexico, ele vai pular aque token e tentar ler o próximo token válido, ao invés de abortar a compilação.
mas fazendo uma contagem da quantidade de erros encontrados, para mostrar no final da compilação junto com os erros sintáticos encontrados pelo Parser.
*/
void 
Scanner::lexicalError(string msg)
{
    cout << "Linha " << line << ": " << msg << endl;
    contagen_erros++; // Incrementa o contador de erros
    
    // RECUPERAÇÃO: Avança a posição para ignorar o caractere problemático
    pos++; 
    //exit(EXIT_FAILURE);
}