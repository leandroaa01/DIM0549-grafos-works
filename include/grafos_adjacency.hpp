#ifndef GRAFOS_HPP
#define GRAFOS_HPP

#include <array>
#include <iostream>
#include <vector>

using value = int;
using  matriz = std::vector<std::vector<value>>;
class Graph {
  public:
  value m_vertices{};
  matriz  m_adjacency;

   Graph( value vertices ) : m_vertices( vertices ) {
    m_adjacency.resize( vertices, std::vector<value>( vertices, 0 ) );
}
  void add(value origin, value destiny);

  void remove( value origin, value destiny );

  void print() const;

  void dfs(Graph &g, value vertex);
};

#endif // GRAFOS_HPP
