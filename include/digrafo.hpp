#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <iostream>
#include <fstream>
#include <vector>

using value = int;
using matriz = std::vector<std::vector<value>>;

class DiGraph {
    public:
    value m_vertices{1};
    std::vector<std::vector<value>> m_matAdjacency;

    DiGraph(value vertices);
    void add(value origin, value destiny);
    void remove( value origin, value destiny );
    void print() const;
    void printInc() const;
    void printRStar() const;
};


#endif