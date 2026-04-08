#include "include/grafos.hpp"
#include <cstdlib>

int main(int argc, char const *argv[])
{
    Graph<int> g(6);
     g.add(1, 4);
     g.add(2, 4);
     g.add(2, 3);
     g.add(3, 4);
     g.add(4, 5);
     g.add(5, 6);
     g.add(6, 4);
     g.print(); 
     std::cout << "total edges: " << g.total_edges() << std::endl;
     std::cout << "total vertices: " << g.total_vertices() << std::endl;
     std::cout << "-------------------\n";
     std::cout<<"Degree of vertex 4: " <<g.degree(4) << std::endl;
     g.is_adjacent(1, 4) ? std::cout << "Vertices 1 and 4 are adjacent.\n" : std::cout << "Vertices 1 and 4 are not adjacent.\n";
     g.is_adjacent(1, 2) ? std::cout << "Vertices 1 and 2 are adjacent.\n" : std::cout << "Vertices 1 and 2 are not adjacent.\n";
     std::cout << "-------------------\n";
     g.to_list();
     g.print();
    std::cout << "-------------------\n";
     Graph<char> g2(3);
        g2.add('A', 'B');
        g2.add('B', 'C');
        g2.add('C', 'A');
        g2.print();
        g2.to_list();
    std::cout << "-------------------\n";
        g2.print();
     
    return 0;
}
