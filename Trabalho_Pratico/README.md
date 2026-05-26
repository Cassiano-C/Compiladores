# Compilado de C--
![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
Aqui esta um compilado que foi criado na materia de Compiladores 1 da UFMS.

## Arquivos

|Arquivos | Descrição |
| :--- | :---|
|**principal.cpp**| Essa é a main do compilador na onde ela recebe um arquivo como argumento e manda para o **parser** |
|**parser.cpp**| Vai receber o arquivo da **principal.cpp** e verificar se esta sintaticamente correto sempre conversando com o **scaner**|
|**parser.h**| Vai definir a implementação da classe **parser** e dos seus metodos |
|**scaner.cpp**| Vai receber as requisições do **parcer** e retornar os tokens definidos no **token.h**, qualquer outra coisa é erro |
|**scaner.h**| Declaração da classe que vai ser usada na **scaner.cpp** |
|**token.h**| Declaração da classe **token** para que o **scaner** a use |


## Logica do compilador

## Como compilar e executar
Esse projeto tem uma forma diferente de executar.
```bash
# Como vai ser comilado
g++ *.cpp -o main 

# Como deve ser executado
./main teste_da_limguagen.cmm
```