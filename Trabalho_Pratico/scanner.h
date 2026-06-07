#include <fstream>
#include "symboltable.h"

class Scanner 
{
    private: 
        string input;//Armazena o texto de entrada
        int pos;//Posição atual
        int line;
        SymbolTable* st;
        
    public:
        //Construtor
        int contagen_erros = 0;
        Scanner(string, SymbolTable*);

        int getLine();
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();        
    
        //Método para manipular erros
        void lexicalError(string);
};
