#include "../include/grafos_adjacency.hpp"
#include <stack>


    void Graph::add( value origin, value destiny ) {
    if(origin < 1 || origin > m_vertices || destiny < 1 || destiny > m_vertices) {
      std::cerr << "Error: Vertex index out of bounds." << std::endl;
      return;
    }
    m_adjacency[origin-1][destiny-1] = 1;
    m_adjacency[destiny-1][origin-1] = 1; 
    // Para grafos não direcionados, adicionamos a aresta em ambas as direções
  }
    void Graph:: remove( value origin, value destiny ) {
    if(origin < 1 || origin > m_vertices || destiny < 1 || destiny > m_vertices) {
      std::cerr << "Error: Vertex index out of bounds." << std::endl;
      return;
    }
    m_adjacency[origin-1][destiny-1] = 0;
    m_adjacency[destiny-1][origin-1] = 0; 
    // Para grafos não direcionados, removemos a aresta em ambas as direções
  }

    void Graph:: print() const {
    std::cout << " | ";
    for ( value i = 0; i < m_vertices; ++i ) {
      std::cout << i + 1 << " ";
    }
    std::cout << "|\n";
    for ( value i = 0; i < m_vertices; ++i ) {
     std::cout << i + 1 << "| ";
      for ( value j = 0; j < m_vertices; ++j ) {
        std::cout << m_adjacency[i][j] << " ";
      }

      std::cout << "| \n";
    }
    }

    value get_menor_ixd_g(const Graph &g, value vertex, const std::vector<int> &visited) {
    if (vertex < 1 || vertex > g.m_vertices) {
      return -1;
    }

    for (value i = 0; i < g.m_vertices; ++i) {
      if (g.m_adjacency[vertex - 1][i] == 1 && visited[i] == 0) {
        return i + 1; 
      }
    }

    return -1;
  }

    void Graph::dfs(Graph& g, value vertex ){
    if (g.m_vertices <= 0) {
      std::cerr << "Error: Empty graph." << std::endl;
      return;
    }

    if (vertex < 1 || vertex > g.m_vertices) {
      std::cerr << "Error: Vertex index out of bounds." << std::endl;
      return;
    }

    std::vector<int> visited(g.m_vertices, 0);
    std::stack<value> p;

    visited[vertex - 1] = 1;
    p.push(vertex);
    std::cout << "Começando DFS com vértice: " << vertex << "\n";
    while (!p.empty()) {
      value current = p.top();
      std::cout <<" valor no topo da pilha: " << current << " ";

      value u = get_menor_ixd_g(g, current, visited);
      
      if (u != -1) {
        std::cout <<" colocando o " << u << " na pilha \n";
        visited[u - 1] = 1;
        p.push(u);
      } else {
        std::cout << "removendo o  " << current << " do topo \n";
        p.pop();
      }
    }
    std::cout << "\n";
  }