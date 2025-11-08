## FACULDADE DE CIÊNCIAS DA UNIVERSIDADE DO PORTO
### Departamento de Ciência de Computadores

### COMP — Compiladores
### Trabalho Prático 1 — Análise Lexical e Sintática

**Grupo: 6**

---

### Conteúdo

1.  [Introdução](#1-introdução)
2.  [Método](#2-método)
3.  [Estruturas de Dados Fundamentais](#3-estruturas-de-dados-fundamentais)
4.  [Estrutura e Funcionamento do Programa](#4-estrutura-e-funcionamento-do-programa)
5.  [Comandos e Estruturas de Controlo](#5-comandos-e-estruturas-de-controlo)
6.  [Expressões](#6-expressões)
7.  [Compilação e Execução](#7-compilação-e-execução)
8.  [Conclusão](#8-conclusão)

---

### 1. Introdução

O objetivo deste trabalho é implementar as fases de análise lexical e análise sintática de um compilador para um subconjunto da linguagem Ada, produzindo uma Árvore Sintática Abstrata (AST) que representa a estrutura do programa.

---

### 2. Método

O desenvolvimento do projeto foi realizado em linguagem C, recorrendo a ferramentas do ecossistema GNU, especialmente o **Flex** e o **Bison**, amplamente usadas na criação de compiladores. O sistema segue uma sequência típica de funcionamento de um compilador, composta por três fases principais:

* **Análise Léxica:**
    Nessa etapa, o Flex (no ficheiro `scanner.flex`) gera automaticamente a função `yylex()`, encarregada de ler o código-fonte e convertê-lo em uma sequência de *tokens* (como `TOK_BEGIN`, `IDENTIFIER`, etc.), que representam os elementos básicos da linguagem.

* **Análise Sintática:**
    O Bison (no ficheiro `parser.bison`) interpreta os *tokens* de acordo com as regras gramaticais definidas, garantindo que a estrutura do código esteja conforme a gramática Ada especificada.

* **Construção da AST:**
    À medida que o analisador sintático reconhece as regras válidas, são executadas ações semânticas que constroem a Árvore Sintática Abstrata (AST). As funções responsáveis pela criação dos nós desta estrutura encontram-se implementadas em `ast.h` e `ast.c`.

---

### 3. Estruturas de Dados Fundamentais

A **AST** é o núcleo da representação interna do programa. Ela descreve a lógica do código por meio de duas estruturas principais: `Expr` (para expressões) e `Stmt` (para comandos/instruções). Esta representação abstrai detalhes puramente sintáticos (como o `end if` ou o `;`), focando-se na estrutura lógica do programa.

A **Tabela de Símbolos**, definida em `symtab.h` e `symtab.c`, é utilizada para a análise semântica básica.
Ao contrário de implementações mais complexas, a nossa abordagem foi:

* Implementação através de uma **lista ligada simples**. Para o âmbito deste projeto, focado num único escopo global, esta é uma solução perfeitamente eficiente para gerir as declarações.
* Fornece duas funções principais: `symbol_add()`, que adiciona um novo identificador à tabela, e `symbol_exists()`, que verifica se um identificador já foi declarado.
* Esta tabela permite-nos executar validações semânticas cruciais: previne **declarações duplicadas** e deteta o uso de **variáveis não declaradas**.

---

### 4. Estrutura e Funcionamento do Programa

A regra principal do compilador, denominada `program` no Bison, segue a sintaxe Ada definida:

```ada
PROCEDURE Main IS
   <opt_decl_list>
BEGIN
   <stmt_list>
END Main;

```
Durante a análise da secção de declarações (`opt_decl_list`), o Bison invoca a função `symbol_add()` para cada identificador encontrado, registando-o na Tabela de Símbolos. Posteriormente, ao processar a lista de comandos (`stmt_list`), o parser utiliza `symbol_exists()` para validar cada variável usada (em atribuições ou expressões), garantindo que esta foi previamente declarada e evitando erros semânticos.

---

### 5. Comandos e Estruturas de Controlo

O parser foi projetado para reconhecer os comandos de controlo definidos no projeto, gerando o nó correspondente na AST.

As construções suportadas são:

* **Atribuição:**
    `ID := expr;` → gera o nó **`STMT_ASSIGN`**.
* **Estruturas condicionais:**
    São aceites as formas `IF...THEN...END IF;` e `IF...THEN...ELSE...END IF;`. Estas são representadas pelo nó **`STMT_IF`** (o campo `else_body` fica `NULL` se não existir). A sintaxe `ELSIF` não foi implementada.
* **Ciclos de repetição:**
    `WHILE expr LOOP cmd_list END LOOP;` → traduzido para o nó **`STMT_WHILE`**.
* **Operações de saída:**
    `PUT_LINE(expr);` → gera o nó **`STMT_PUTLINE`**.
* **Operações de entrada:**
    `GET_LINE` **não é um comando**, mas sim uma *expressão* (nó **`E_GET_LINE`**) que retorna um inteiro, podendo ser usada em qualquer lado onde uma expressão é válida (ex: `a := GET_LINE + 5;`).

---

### 6. Expressões

As expressões aritméticas e booleanas são processadas com base na precedência e associatividade dos operadores. Para garantir a correta ordem de avaliação (ex: `*` antes de `+`), foi utilizada a diretiva `%left` no ficheiro `parser.bison`.

Na AST, tanto as expressões aritméticas (`+`, `-`, `rem`, `mod`, etc.) como as booleanas (`<`, `=`, `/=`, `>=`, etc.) são representadas pelo mesmo nó: **`E_OPERATION`**. Este nó armazena o operador (ex: `PLUS`, `GEQ`) e dois ponteiros (`left` e `right`) para as sub-expressões.

---

### 7. Compilação e Execução

O projeto inclui um `Makefile` que automatiza o processo de compilação (invocando o `bison`, `flex` e `gcc`) e gera o executável principal denominado **`compiler`**.

Os passos para compilar e executar são os seguintes:

* **Compilar o projeto:**
    ```bash
    make
    ```
* **Executar o parser (compilador):**
    ```bash
    ./compiler test.adb
    ```

Este programa **não é um interpretador**; ele não executa o código Ada. A sua função, definida em `main.c`, é invocar o analisador sintático (`yyparse()`) e, se a análise for bem-sucedida, **imprimir a Árvore Sintática Abstrata** resultante no terminal. O objetivo é demonstrar a correção das fases de análise lexical e sintática.

---

### 8. Conclusão

A implementação da Árvore Sintática Abstrata (AST) e da Tabela de Símbolos confirma o correto funcionamento da análise sintática e de uma fase de análise semântica básica. O parser consegue não só validar a estrutura gramatical do subconjunto de Ada, mas também detetar erros de declaração de variáveis.

Este trabalho estabelece uma base sólida para as etapas posteriores de um compilador, como uma análise semântica mais profunda (verificação de tipos) e a eventual geração de código.
