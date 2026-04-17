#include "GraphParser.hpp"
#include "TxtParser.hpp"
#include "include/grafos.hpp"
#include "include/parser/CreateParser.tpp"
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
     std::cout << "The graph is conexo: " << (g.is_conexo() ? "Yes" : "No") << std::endl;
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
    std::cout << "--------------------\n";
    Graph<int> g3(6);
    g3.add(1, 4);
    g3.add(1, 5);
    g3.add(2, 5);
    g3.add(2, 6);
    g3.add(3, 4);
    g3.add(3, 6);
    g3.print();
    std::cout << "g3 is bipartite: " << (g3.is_bipartite() ? "Yes" : "No") << std::endl;
    
    


    GraphType gt{GraphType::NONE};
    psr::GraphOption go{psr::GraphOption::GRAPH};
    std::string filePath{""};
    if (argc < 2) {
        std::cerr << "Error: Wrong number of arguments.\n";
        std::cerr << "Usage: " << argv[0] << " <input_file_path>\n";
        return EXIT_FAILURE; // Retorna 1 para o sistema
    }
    if(argc > 2){
        if(!verify_args(argc, argv, gt, go, filePath)){
            return EXIT_FAILURE;
        }
    }
    if(gt == GraphType::CHAR){
        std::unique_ptr<psr::Parser<char>> parser = CreateParser::create<char>( psr::ParserType::TXT); 

        Graph<char> graph = parser->parse(filePath, go);

        graph.to_incMat();
        graph.print();

        graph.to_list();
        graph.print();

        graph.to_matrix();
        graph.print();

        graph.dfs_directed_classification('a');

    }
    else{
        std::unique_ptr<psr::Parser<int>> parser = CreateParser::create<int>( psr::ParserType::TXT); 

        Graph<int> graph = parser->parse(filePath, go);

        graph.to_incMat();
        graph.print();

        graph.dfs_directed_classification(1);

    }
    
    return EXIT_SUCCESS;
}
