# Introduction

Este projeto implementa algoritmos de grafos em C++.

# Author(s)

nome: Heitor Fernandes Paes Leme Campos
email: Heitor.campos.711@ufrn.edu.br

nome: Leandro Andrade de Araujo
email: leandro.andrade.401@ufrn.edu.br 

# Implementações

Aqui temos as implementações realizadas na classe Graph:
> As questões estão abreviadas, mas os números são correspondentes aos vistos no arquivo Trabalho_Unidade_1.pdf.

A (Para Grafos):

| Item                                      |    Realizada    |
| ----------------------------------------- | :-------------: |
| 1: Representação a partir da Lista de ADJ |       V         |
| 2: Representação a partir da Matriz de ADJ|       V         |
| 3: Representação a partir da Matriz de INC|       V         |
| 4: Conversões entre Matriz de ADJ e Lista de ADJ           |       V         |
| 5: Função que calcula o grau de vértices  |       V         |
| 6: Função que diz se dois vértices são adjacentes|       V         |
| 7: Função que diz o n⁰ total de vértices  |       V         |
| 8: Função que diz o n⁰ total de arestas   |       V         |
| 9: Inclusão de um novo vértice            |       V         |
| 10: Exclusão de um vértice existente      |       V         |
| 11: Função que diz se o grafo é conexo    |       V         |
| 12: Função que diz se o grafo é bipartido ***(OPC)*** |       V         |
| 13: Busca em Largura a partir de um vértice     |       V         |
| 14: Busca em Profundidade a partir de um vértice|       V         |
| 15: Determinação de articulações e blocos |       V         |

B (Para Digrafos):

| Item                                      |    Realizada    |
| ----------------------------------------- | :-------------: |
| 16: Representação de Digrafos a partir da Matriz de ADJ |       V       |
| 17: Representação de Digrafos a partir da Matriz de INC |       V       |
| 18: Determinar se o grafo é subjacente ***(OPC)***   |       X       |
| 19: Conversão de Matriz de INC para Estrela Direta ***(OPC)***|       X       |
| 20: Busca em Profundidade Com determinação de profundidade e definições |       V       |
| 21: Implementar uma aplicação usando DFT  ***(OPC)***   |       X       |
# Como Compilar e Executar

## Requisitos

- CMake >= 3.10
- Compilador C++ (recomendado: g++ >= 7)

## Como compilar e executar

1. Clone o repositório ou navegue até a pasta do projeto.

2. Gere os arquivos de build com o CMake:

```bash
cmake -S . -B build
```

3. Compile o projeto:

```bash
cmake --build build
```

4. Execute o programa:

```bash
./build/grafos <caminho-do-arquivo> [opções]
```

onde: 

<caminho-do-arquivo> (obrigatório) é o caminho do arquivo de leitura

### Opções de execução:

| Flag | Nome Completo | Descrição |
| :--- | :----------- | :----------------------------------- |
| `-c` | `--char`     | Define vértices como **char**.       |
| `-i` | `--int`      | Define vértices como **int**.        |
| `-d` | `--directed` | Define o grafo como **direcionado**. |

**Exemplo de uso:**
```bash
 ./build/grafos entrada.txt -c -d  
 #(Lê o arquivo entrada.txt, com vértices do tipo char, formatado como um grafo direcionado).
 ```