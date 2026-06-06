# 🚀 Compilador C-- (C-Minus-Minus)

![C++](https://img.shields.io/badge/C++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![UFMS](https://img.shields.io/badge/UFMS-FaCOM-%23004b87?style=for-the-badge)

Este projeto consiste no desenvolvimento de um compilador front-end para a linguagem **C--**, construído como requisito prático para a disciplina de **Compiladores I** do curso de Engenharia de Computação / Ciência da Computação na **Universidade Federal de Mato Grosso do Sul (UFMS)**.

---

## 👥 Alunos
1. **Cassiano Carvalho de Souza**
2. **Felipe Camargo Machado Figueiredo** 

---

## 📁 Arquitetura e Estrutura de Arquivos

O projeto está dividido de forma modular seguindo o padrão clássico de Engenharia de Compiladores:

| Arquivo | Componente | Descrição |
| :--- | :--- | :--- |
| **`principal.cpp`** | Ponto de Entrada | Contém a função `main`. Recebe o arquivo de código fonte (`.mj` ou `.cmm`) via linha de comando, instancia o Parser e inicia o processo de compilação. |
| **`scanner.cpp` / `.h`** | Analisador Léxico | Lê o fluxo de caracteres do arquivo, ignora espaços/comentários (`//` e `/* */`) e agrupa caracteres em estruturas chamadas **Tokens** legítimos. |
| **`parser.cpp` / `.h`** | Analisador Sintático | O "coração" do compilador. Implementa uma estratégia **Preditiva LL(1) Descedente Analítica** através de funções recursivas que validam a estrutura gramatical. |
| **`token.h`** | Estrutura de Dados | Define o dicionário de categorias léxicas (`enum Names`) e mapeia constantes como palavras reservadas, operadores, delimitadores e literais. |
| **`symboltable.cpp` / `.h`** | Tabela de Símbolos | Gerencia os escopos do programa utilizando um mapeamento hash hierárquico (`std::map`), permitindo aninhamento e ligação de escopo pai/filho. |
| **`stentry.cpp` / `.h`** | Entrada da Tabela | Representa os nós e registros individuais guardados dentro da Tabela de Símbolos, separando identificadores comuns de palavras reservadas. |

---

## 🧠 Lógica e Tratamento de Erros do Compilador

Este compilador foi projetado para ser **altamente tolerante a falhas**, o que significa que ele não interrompe a execução no primeiro erro encontrado. Ele tenta se recuperar para varrer o arquivo inteiro e exibir todos os problemas de uma só vez.

### 1. Analisador Léxico (Recuperação de Erros)
Caso encontre um caractere inválido (ex: `@`, `$`, `§`), o `Scanner` reporta o erro imediatamente na tela indicando a linha, descarta o caractere problemático (`pos++`), e continua a leitura do arquivo de forma recursiva sem quebrar a execução do Parser.

### 2. Analisador Sintático (Panic Mode / Modo Pânico)
O Parser implementa **Sincronização Avançada por Modo Pânico** orientada por conjuntos **FIRST+**:
* **Método `match(token)` Seguro:** Se o Parser esperar um token e receber outro, ele imprime o erro sintático estruturado e chama o método `synchronize(...)`.
* **Sincronização Localizada por Comandos:** Nas expressões matemáticas e lógicas, o compilador sincroniza em barreiras de fim de instrução ou delimitadores como `;`, `,`, `)`, `}` e `]`. Isso evita que um erro de digitação simples em uma conta descarte o resto do arquivo.
* **Salvaguarda contra Loops:** O compilador tolera até **10 erros estruturais**. Se ultrapassar esse limite, ele aborta a compilação por segurança para evitar loops infinitos na entrada.

---

## 📜 Gramática do Compilador

A especificação formal da linguagem de programação C--, contendo as produções livres de contexto (BNF/EBNF) e definições léxicas, pode ser acessada através do link oficial da disciplina.

> ⚠️ *Nota: O documento abaixo requer autenticação institucional com o passaporte da UFMS.*

🔗 [Acessar Documento da Gramática Oficial da Linguagem](https://docs.google.com/document/d/1dhHmLc58T4pNHowBL2XEHwUvSWKWVjCdRKFNF6Dc1Tg/edit?usp=sharing)

---

## 🛠️ Como Compilar e Executar

Certifique-se de possuir um compilador que suporte a especificação moderna do C++ (G++ ou Clang instalado).

### 1. Compilação do Projeto
Para compilar todos os módulos do projeto de uma única vez gerando o binário executável final:
Verção do compilador g++ utilizada: 
```
g++ (GCC) 16.1.1 20260515 (Red Hat 16.1.1-2)
Copyright (C) 2026 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
```bash
# Compilar o projeto
g++ *.cpp -o compilador

# Executar o projeto
./compilador teste.cmm
```