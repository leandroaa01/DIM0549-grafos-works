#include "../include/grafos_adjacency.hpp"


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