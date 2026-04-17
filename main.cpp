#include "include/parser/GraphParser.hpp"
#include "include/parser/TxtParser.hpp"
#include "include/grafos.hpp"
#include "include/parser/CreateParser.tpp"
#include <cstdlib>

int main(int argc, char const *argv[])
{

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
