#include "../include/digrafo.hpp"
#include <utility>

using value = int;
using  matriz = std::vector<std::vector<value>>;

DiGraph::DiGraph( value vertices ) : m_vertices( vertices ) {
    m_matAdjacency.resize( vertices, std::vector<value>( vertices, 0 ) );
}
  void DiGraph::add( value origin, value destiny ) {
    if(origin < 1 || origin > m_vertices || destiny < 1 || destiny > m_vertices) {
      std::cerr << "Error: Vertex index out of bounds." << std::endl;
      return;
    }
    m_matAdjacency[origin-1][destiny-1] = 1;
    m_matAdjacency[destiny-1][origin-1] = -1;
  }
  void DiGraph::remove( value origin, value destiny ) {
    if(origin < 1 || origin > m_vertices || destiny < 1 || destiny > m_vertices) {
      std::cerr << "Error: Vertex index out of bounds." << std::endl;
      return;
    }
    m_matAdjacency[origin-1][destiny-1] = 0;
    m_matAdjacency[destiny-1][origin-1] = 0;
  }

  

  void DiGraph::print() const {
    std::cout << " | ";
    for ( value i = 0; i < m_vertices; ++i ) {
      std::cout << i + 1 << " ";
    }
    std::cout << "|\n";
    for ( value i = 0; i < m_vertices; ++i ) {
     std::cout << i + 1 << "| ";
      for ( value j = 0; j < m_vertices; ++j ) {
        std::cout << m_matAdjacency[i][j] << " ";
      }

      std::cout << "| \n";
    }
  }

  void DiGraph::printInc() const {

    std::vector<std::pair<value, value>> edges;

    for(value i{0}; i < m_vertices; ++i){
      for(value j{0}; j < m_vertices; ++j){
        if(m_matAdjacency[i][j] == 1){
          edges.push_back(std::pair(i, j));
        }
      }
    }
    std::cout << " | ";
    for ( value i = 0; i < edges.size(); ++i ) {
      std::cout << "E" << i + 1 << " ";
    }
    std::cout << "|\n";

    for ( value i = 0; i < m_vertices; ++i ) {
     std::cout << i + 1 << "| ";
      for ( value j = 0; j < edges.size(); ++j ) {
        if(edges[j].first == i){
            std::cout << 1 << " ";
        }
        else if(edges[j].second == i){
          std::cout << -1 << " ";
        }
        else{
            std::cout << 0 << " ";
        }
      }

      std::cout << "| \n";
    }
  }

void DiGraph::printRStar() const {
    std::vector<std::pair<value, value>> edges;
    std::vector<value> point(m_vertices + 1, 1); 
    
    value current_edge_index = 1;
    for(value i{0}; i < m_vertices; ++i){
        point[i] = current_edge_index; 
        
        for(value j{0}; j < m_vertices; ++j){
            if(m_matAdjacency[i][j] == 1){
                edges.push_back({i, j});
                current_edge_index++;
            }
        }
    }
    point[m_vertices] = current_edge_index;

    std::cout << "A | <B, E> | P | V |\n";
    
    value max_rows = std::max(static_cast<size_t>(m_vertices), edges.size());
    
    for(value i{0}; i < max_rows; ++i) {
        std::cout << (i + 1) << " | ";
        
        if (i < edges.size()) {
            std::cout << "<" << edges[i].first + 1 << ", " << edges[i].second + 1 << "> | ";
        } else {
            std::cout << "       | ";
        }
        
        if (i <= m_vertices) {
            std::cout << point[i] << " | ";
            if (i < m_vertices) {
                std::cout << (i + 1) << " |";
            } else {
                std::cout << "  |";
            }
        } else {
            std::cout << "  |   |";
        }
        std::cout << "\n";
    }
}