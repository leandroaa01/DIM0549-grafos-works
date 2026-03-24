#include "include/grafos_adjacency.hpp"
#include "include/grafos_list.hpp"
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

     std::cout << "\n";
     g.dfs(g, 4);
    std::cout << "\n\n";
     GraphList gl(5);
    gl.add(1, 2);
    gl.add(1, 3);
    gl.add(2, 3);
    gl.add(3, 4);
    gl.add(3, 5);
    gl.print();

     
    return 0;
}
