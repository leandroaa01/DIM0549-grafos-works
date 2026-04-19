# DIM0549-grafos-works

Este projeto implementa algoritmos de grafos em C++.

## Requisitos

- CMake >= 3.10
- Compilador C++ (recomendado: g++ >= 7)

## Como compilar e executar

1. Clone o repositório e navegue até a pasta

```bash
git clone https://github.com/leandroaa01/DIM0549-grafos-works.git
cd DIM0549-grafos-works/
```

2. Gere os arquivos de build com o CMake:

```bash
./configure.sh
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
## Estrutura do arquivo de leitura

Os arquivos de leitura devem possuir a extensão .txt. 
A primeira linha deverá conter o número total de vértices. A segundo **PODERÁ** ter uma flag (0 ou 1) que irá definir se o grafo é direcionado (0 -> Grafo, 1 -> DiGrafo), sendo completamente opcional para o funcionamento do programa. Por padrão, essa flag sobrescreverá a opção `-d` ou `--directed` da linha de comando. Caso nenhuma dessas flags seja utilizada, o grafo será gerado como um grafo não direcionado.
Após isso, as linhas seguintes deverão conter as arestas separadas com vírgula.

<div align="center">
<img src="pics/exemploDeEntrada1.png" alt="Estilo de entrada" width="500">
</div>

Exemplo:

<div align="center">
<img src="pics/exemploDeEntrada2.png" alt="Exemplo 1"> 
</div>

## Estrutura geral do programa

```
DIM0549-grafos-works/
├── data/docs/                  # Pasta que possui todos os arquivos de teste .txt 
├── include/
│   ├── parser/
│   │   ├── CreateParser.hpp    # Classe que cria o parser com base na escolha do tipo do arquivo de leitura (neste projeto, apenas .TXT)
│   │   ├── CreateParser.tpp    # Implementação em template da classe CreateParser
│   │   ├── GraphParser.hpp     # Classe abstrada que poderá ser extendida para parsers de vários tipos (neste projeto, apenas .TXT)
│   │   ├── TxtParser.hpp       # Classe filha de GraphParser que implementa o parser para arquivos .TXT
│   │   └── TxtParser.tpp       # Implementação em template da classe TxtParser
│   └── core/              
│       ├── grafos.hpp          # Classe principal que implementa um Grafo
│       └── grafos.tpp          # Implementação em template da classe Graph
├── pics/                       # Imagens utilizadas nesta documentação
├── main.cpp                    # Função Main que irá criar os objetos Parser e Grafo implementados e aplicar suas funções
├── configure.sh                # Arquivo de compilação otimizada
├── CMakeLists.txt              # Configuração de compilação
├── AUTHOR.md                   # Lista de implementações realizadas
└── README.md                   # Esta documentação
```
## Autores

<ul>
    <li> Heitor Campos  </li>
    <li> Leandro Andrade</li>
</ul>

## Licença

Este projeto é acadêmico e livre para uso.
