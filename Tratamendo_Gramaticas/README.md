# Códigos de Validação Gramatical

Este repositório contém scripts desenvolvidos para processar gramáticas e adaptá-las para os formatos exigidos por diferentes tipos de analisadores sintáticos, como o **LL(1)**.

---

## 📐 Analisador LL(1)

O script `Checkout_LL(1).py` foi projetado especificamente para processar e tratar a especificação sintática da linguagem definida para o **Trabalho Prático**.

### Gramática Original da Linguagem (EBNF)

Abaixo está a especificação formal e original da linguagem:

1. **Program** → `(Function)* EOF`
2. **Function** → `Type ID ( ParamTypes ) { (Type VarDeclaration (, VarDeclaration)* ;)* (Statement)* }`  
   | `void ID ( ParamTypes ) { (Type VarDeclaration (, VarDeclaration)* ;)* (Statement)* }`
3. **VarDeclaration** → `ID ([integerconstant])?`
4. **Type** → `char` | `int`
5. **ParamTypes** → `void` | `Type ID ([])* (, Type ID ([])*)*`
6. **Statement** → `if ( Expression ) Statement (else Statement)?`  
   | `while ( Expression ) Statement`  
   | `for ( (Assign)? ; (Expression)? ; (Assign)? ) Statement`  
   | `return (Expression)? ;`  
   | `Assign ;`  
   | `ID ( (Expression (, Expression)*)? ) ;`  
   | `{ (Statement)* }`  
   | `;`
7. **Assign** → `ID ([ Expression ])? = Expression`
8. **Expression** → `- Expression`  
   | `! Expression`  
   | `Expression BinOp Expression`  
   | `Expression RelOp Expression`  
   | `Expression LogOp Expression`  
   | `ID ( ((Expression (, Expression)*)?) | [ Expression ] )?`  
   | `( Expression )`  
   | `integerconstant`  
   | `charconstant`  
   | `stringconstant`
9. BinOp → `+` | `-` | `*` | `/`
10. RelOp → `==` | `!=` | `<=` | `<` | `>=` | `>`
11. LogOp → `&&` | `||`

> 💡 **Nota de Implementação:** Para otimizar o desempenho do algoritmo de análise e facilitar o mapeamento de dados, a gramática estendida (EBNF) acima foi previamente expandida para o formato estruturado de listas em **BNF Pura** e alocada dentro do dicionário `gramatica_linguagem`.

---

## 🚀 Execução e Fluxo de Saída

Para rodar o script de análise preditiva no terminal, utilize o interpretador padrão do Python 3:

```bash
python3 "Checkout_LL(1).py"
```
