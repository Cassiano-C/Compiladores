#include "scanner.h"


// é um analisador sintático preditivo recursivo descendente, ou seja, é um analisador sintático que utiliza uma abordagem de análise top-down para construir a árvore de derivação da gramática. Ele é chamado de "previsível" porque pode prever qual produção usar com base no próximo token de entrada, sem a necessidade de backtracking. O "recursivo descendente" refere-se ao fato de que o analisador chama recursivamente funções para processar as regras da gramática.
class Parser
{
    private:
        Scanner* scanner;
        Token* lTocken;

        void advance();
        void match(int);

        void gool(); //1
        void E(); //2
        void E1(); //3
        void T(); //4
        void T1(); //5
        void F(); //6

        void error(string);

    public:
        Parser(string); 
        void run();

};
