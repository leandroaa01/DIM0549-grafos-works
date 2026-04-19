# Introdução

Este projeto implementa algoritmos de grafos em C++.

# Autores

nome: Heitor Fernandes Paes Leme Campos
email: Heitor.campos.711@ufrn.edu.br

nome: Leandro Andrade de Araujo
email: leandro.andrade.401@ufrn.edu.br 

# Implementações

Aqui temos as implementações realizadas na classe Graph:
> As questões estão abreviadas, mas os números são correspondentes aos vistos no arquivo Trabalho_Unidade_1.pdf.

A (Para Grafos):

| Item                                      |    Realizada    |   Implementado Por |
| ----------------------------------------- | :-------------: | :---------------: |
| 1: Representação a partir da Lista de ADJ |       V         |  Leandro |
| 2: Representação a partir da Matriz de ADJ|       V         |  Leandro e Heitor |
| 3: Representação a partir da Matriz de INC|       V         |  Heitor |
| 4: Conversões entre Matriz de ADJ e Lista de ADJ           |       V         |                                                       Leandro e Heitor |
| 5: Função que calcula o grau de vértices  |       V         |    Leandro |
| 6: Função que diz se dois vértices são adjacentes|       V         | Leandro |
| 7: Função que diz o n⁰ total de vértices  |       V         | Leandro |
| 8: Função que diz o n⁰ total de arestas   |       V         | Leandro |
| 9: Inclusão de um novo vértice/aresta            |       V         | Leandro e Heitor |
| 10: Exclusão de um vértice existente/aresta      |       V         | Leandro e Heitor |
| 11: Função que diz se o grafo é conexo    |       V         | Leandro |
| 12: Função que diz se o grafo é bipartido ***(OPC)*** |       V         | Leandro |
| 13: Busca em Largura a partir de um vértice     |       V         | Heitor|
| 14: Busca em Profundidade a partir de um vértice|       V         | Heitor|
| 15: Determinação de articulações e blocos |       V         | Heitor|

B (Para Digrafos):

| Item                                      |    Realizada    |   Implementado Por |
| ----------------------------------------- | :-------------: |  :-------------:
| 16: Representação de Digrafos a partir da Matriz de ADJ |       V       | Leandro e Heitor |
| 17: Representação de Digrafos a partir da Matriz de INC |       V       |  Heitor |
| 18: Determinar se o grafo é subjacente ***(OPC)***   |       X       | X |
| 19: Conversão de Matriz de INC para Estrela Direta ***(OPC)***|       X       | X |
| 20: Busca em Profundidade Com determinação de profundidade e definições |       V       | Heitor |
| 21: Implementar uma aplicação usando DFT  ***(OPC)***   |       X       | X |
# Como Compilar e Executar

## Requisitos

- CMake >= 3.10
- Compilador C++ (recomendado: g++ >= 7)

## Como compilar e executar

1. Clone o repositório ou navegue até a pasta do projeto.

```bash
git clone https://github.com/leandroaa01/DIM0549-grafos-works.git
cd DIM0549-grafos-works/
```

2. Gere os arquivos de build com o CMake:


```bash
configure.sh
```
Ou 
```bash
cmake -S . -B build
```

3. Compile o projeto:

```bash
cmake --build build
```

4. Execute o programa:

```bash
./bin/grafos <caminho-do-arquivo> [opções]
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
 ./bin/grafos entrada.txt -c -d  
 #(Lê o arquivo entrada.txt, com vértices do tipo char, formatado como um grafo direcionado).
 ```

 ---

&copy; DIMAp | Departamento de Informática e Matemática Aplicada (2016 - 2025)