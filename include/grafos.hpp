/**
 * @file grafos.hpp
 * @author  Leandro Andrade(email:)
 * @author  Heitor ...
 * @brief  classe que representa um grafo
 * @version 0.1
 * @date 2026-04-07
 * @copyright Copyright (c) 2026
 * 
 */
#pragma  once

#include <algorithm>
#include <vector>
#include <list>
#include <unordered_map>
#include <iostream>

template <typename T>
using Matriz = std::vector<std::vector<T>>; //> Define um alises para uma matriz de adjacências de tipo T

template <typename T>
using Lista = std::vector<std::list<T>>; //> Define um alises para uma lista de adjacências de tipo T

/**
 * @brief Classe que representa um grafo
 * O grafo pode ser representado tanto por uma matriz de adjacências quanto por uma lista de adjacências. 
 * A classe possui métodos para converter entre as duas representações, adicionar e remover arestas, e imprimir o grafo...    
 * @tparam Type O tipo dos vértices do grafo 
 */
template <typename Type>
class Graph{
private:
   std::vector<std::vector<int>> m_matrix; //> Matriz de adjacências com 0/1 para representar o grafo
   Lista<Type> m_list; //> Lista de adjacências para representar o grafo
   int m_vertices; //> Número de vértices no grafo
   int m_edges{0}; //> Número de arestas no grafo
   bool use_list{false}; //> Flag para indicar se o grafo está usando lista de adjacências ou matriz de adjacências
   std::unordered_map<Type, int> m_vertex_index; //> Mapa para associar vértices a índices na matriz de adjacências

   int get_vertex_index(Type vertex, bool create = true); //> Método privado para obter ou criar o índice de um vértice
public:

    /**
     * @brief Construtor da classe Graph, que inicializa a matriz de adjacências com o número de vértices fornecido.
     * @note Caso o user queira usar a lista de adjacências, ele pode chamar o método to_list() para converter a matriz em lista.
     * @param vertices  representa o número de vértices do grafo.
     */
   Graph(int vertices) : m_vertices(vertices) {
      m_matrix.resize(vertices, std::vector<int>(vertices, 0)); //> Inicializa a matriz de adjacências com zeros
   }
    /** * @brief Converte a representação do grafo de matriz para lista de adjacências.
     * O método percorre a matriz de adjacências e para cada vértice, adiciona os vértices adjacentes à lista de adjacências correspondente. 
     */
   void to_list();

    /** * @brief  Converte a representação do grafo de lista para matriz de adjacências.
     * O método percorre a lista de adjacências e para cada vértice, marca os vértices adjacentes na matriz de adjacências correspondente.
     * 
     */
    void to_matrix();

    /**
     * @brief  Adiciona uma aresta entre os vértices de origem e destino.
     * O método verifica se o grafo está usando lista ou matriz de adjacências e adiciona a aresta na estrutura correspondente.
     * @param origin  representa o vértice de origem da aresta.
     * @param destiny representa o vértice de destino da aresta.
     */
    void add(Type origin, Type destiny);

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
};

#include "grafos.tpp"
