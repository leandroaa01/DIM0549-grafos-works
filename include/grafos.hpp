/**
 * @file grafos.hpp
 * @author  Leandro Andrade(email:)
 * @author  Heitor Campos()
 * @brief  classe que representa um grafo
 * @version 0.1
 * @date 2026-04-07
 * @copyright Copyright (c) 2026
 * 
 */


#pragma once
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <algorithm>
#include <vector>
#include <list>
#include <unordered_map>
#include <iostream>
#include <queue>
#include <stack>


template <typename T>
using Matriz = std::vector<std::vector<T>>; //> Define um alises para uma matriz de adjacências de tipo T

template <typename T>
using Lista = std::vector<std::list<T>>; //> Define um alises para uma lista de adjacências de tipo T

enum class Representation { //> Enum que guarda todas as possíveis representações do Grafo
      ADJACENCY_MATRIX=0,
      ADJACENCY_LIST,
      INCIDENCY_MATRIX
};


/**
 * @brief Classe que representa um grafo
 * O grafo pode ser representado tanto por uma matriz de adjacências quanto por uma lista de adjacências. 
 * A classe possui métodos para converter entre as duas representações, adicionar e remover arestas, e imprimir o grafo...    
 * @tparam Type O tipo dos vértices do grafo 
 */
template <typename Type>
class Graph{
private:
   std::vector<std::vector<int>> m_inc_matrix; //> Matriz de Incidências com -1/0/1 para representar o grafo
   std::vector<std::vector<int>> m_matrix; //> Matriz de adjacências com -1/0/1 para representar o grafo
   Lista<Type> m_list; //> Lista de adjacências para representar o grafo
   int m_vertices; //> Número de vértices no grafo
   int m_edges{0}; //> Número de arestas no grafo
   Representation graphRep{Representation::ADJACENCY_MATRIX}; //> Flag para indicar se o grafo está usando lista de adjacências, matriz de adjacências ou matriz de incidências
   bool is_targeted{false}; //> Flag para indicar se o grafo é direcionado ou não
   std::unordered_map<Type, int> m_vertex_index; //> Mapa para associar vértices a índices na matriz de adjacências

   int get_vertex_index(Type vertex, bool create = true); //> Método privado para obter ou criar o índice de um vértice
   int get_newest_vertex(int vertex_index); //> Método privado para pegar a aresta a menor distância de um vértice específico
   Type get_vertex_label(int index);
   void dfs_rec(int index, std::vector<Type>& visit_order, std::vector<bool>& visited);

public:

    /**
     * @brief Construtor da classe Graph, que inicializa a matriz de adjacências com o número de vértices fornecido.
     * @note Caso o user queira usar a lista de adjacências, ele pode chamar o método to_list() para converter a matriz em lista.
     * @param vertices  representa o número de vértices do grafo.
     * @param targeted  flag que diz se o grafo é direcionado
     */
   Graph(int vertices, bool targeted = false) : m_vertices(vertices), is_targeted(targeted) {
      m_matrix.resize(vertices, std::vector<int>(vertices, 0)); //> Inicializa a matriz de adjacências com zeros
   }

    /** * @brief Converte a representação do grafo para lista de adjacências.
     * Caso o grafo seja representado por matriz de adjacências, o método percorre a matriz e para cada vértice,
     * adiciona os vértices adjacentes à lista de adjacências correspondente. 
     * Caso o grafo seja representado por matriz de incidência, o método percorre a matriz e para cada aresta,
     * liga seus respectivos vértices na lista de adjacência correspondente.
     */
   void to_list();

    /** * @brief  Converte a representação do grafo para matriz de adjacências.
     * O método percorre a lista de adjacências e para cada vértice, marca os vértices adjacentes na matriz de adjacências correspondente.
     * 
     */
    void to_matrix();

     /** * @brief  Converte a representação do grafo de lista para matriz de adjacências.
     * O método percorre a lista de adjacências e para cada vértice, marca os vértices adjacentes na matriz de adjacências correspondente.
     * 
     */
    void to_incMat();

    /**
     * @brief  Adiciona uma aresta entre os vértices de origem e destino.
     * O método verifica se o grafo está usando lista ou matriz de adjacências e adiciona a aresta na estrutura correspondente.
     * @param origin  representa o vértice de origem da aresta.
     * @param destiny representa o vértice de destino da aresta.
     */
    void add(Type origin, Type destiny);

    /**
     * @brief  Adiciona uma aresta entre os vértices de origem e destino.
     * O método verifica se o grafo está usando lista ou matriz de adjacências e adiciona a aresta na estrutura correspondente.
     * @param edge representa o par ordenado com a origem e o destino respectivamente
     */
    void add(std::pair<Type, Type> edge);

    /**
     * @brief  Remove uma aresta entre os vértices de origem e destino.
     * O método verifica se o grafo está usando lista ou matriz de adjacências e remove a aresta na estrutura correspondente.
     * @param origin  representa o vértice de origem da aresta.
     * @param destiny representa o vértice de destino da aresta.
     */
    void remove(Type origin, Type destiny);

    /**
     * @brief Imprime a representação do grafo, seja ela em matriz ou lista de adjacências.
     * O método verifica se o grafo está usando lista ou matriz de adjacências e imprime a estrutura correspondente de forma legível.
     */
    void print() const;
    /**
     * @brief Calcula o grau de um vértice específico no grafo. 
     * O método percorre a estrutura de adjacências (seja lista ou matriz)
     *  para contar o número de arestas conectadas ao vértice fornecido.
     * 
     * @param vertex  representa o vértice para o qual o grau será calculado.
     * @return int  representa o grau do vértice fornecido, ou -1 se o vértice não existir no grafo.
     */
    int degree(Type vertex);

    /**
     * @brief Verifica se dois vértices são adjacentes no grafo. 
     * O método verifica a estrutura de adjacências (seja lista ou matriz) para determinar se existe uma aresta entre os dois vértices fornecidos.
     * 
     * @param vertex1 representa o primeiro vértice a ser verificado.
     * @param vertex2 representa o segundo vértice a ser verificado.
     * @return true   se os vértices forem adjacentes, ou false caso contrário.
     */
    bool is_adjacent(Type vertex1, Type vertex2); 
    
    /**
      * @brief Utiliza do algoritmo de Busca em Largura (Breadth-First-Search) para explorar o grafo
      * e encontrar um caminho entre o vértice de "raiz" da busca e todos os outros do grafo.
      *
      * @note o algoritmo foca em explorar todos os visinhos de cada vértice antes de ver os vértices que estão em um nível abaixo.
      * @param start_vertex representa o vértice "raiz" da busca
      * @return vetor com cada vértice encontrado em ordem
      */
    std::vector<Type> bfs(Type start_vertex);

    /**
      * @brief Utiliza do algoritmo de Busca em Profundidade (Depth-First-Search) para explorar o grafo
      * e encontrar um caminho entre o vértice de "raiz" da busca e todos os outros grafos
      *
      * @note o algoritmo foca em explorar o mais fundo possível antes de retroceder
      * @param start_vertex representa o vértice "raiz" da busca
      * @return vetor com cada vértice encontrado em ordem
      */
    std::vector<Type> dfs(Type start_vertex);
    void dfs_directed_classification(Type start_vertex);
    void find_articulations();

    int total_edges() const { return m_edges; } //> Retorna o número total de arestas no grafo
    int total_vertices() const { return m_vertices; } //> Retorna o número total de vértices no grafo

    bool is_conexo(); //> Verifica se o grafo é conexo, ou seja, se existe um caminho entre qualquer par de vértices no grafo
    bool is_bipartite(); //> Verifica se o grafo é bipartido, ou seja, se os vértices do grafo podem ser divididos em dois conjuntos disjuntos onde cada aresta conecta um vértice de um conjunto a um vértice do outro conjunto
};

#include "grafos.tpp"
#endif