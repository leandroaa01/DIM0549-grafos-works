#include "include/grafos_adjacency.hpp"
#include <cstdlib>

int main(int argc, char const *argv[])
{
    //exemplo do slide
    Graph g(6);
     g.add(1, 4);
     g.add(2, 4);
     g.add(2, 3);
     g.add(3, 4);
     g.add(4, 5);
     g.add(5, 6);
     g.add(6, 4);
    g.print();
    return 0;
}
