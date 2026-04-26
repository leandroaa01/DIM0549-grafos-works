#ifndef TXT_PARSER_TPP
#define TXT_PARSER_TPP

#pragma once

#include "TxtParser.hpp"

#include <cctype>

template <typename Type>
Graph<Type> TxtParser<Type>::parse(const std::string& filePath, psr::GraphOption graphOption){
    if(filePath.empty()){           //> Caso seja passado uma string vazia
        throw std::invalid_argument("Error! The file path provided is empty.");
    }
    fs::path p{filePath}; 

    if(!fs::exists(p)){ //> Caso não ache o arquivo passado
        throw std::invalid_argument("Error! File not found: " + filePath + '\n');
    }



    std::ifstream ifs{filePath};    //> Representação do arquivo que será lido
    std::string line;                 //> Linha recebida por ifs

    std::getline(ifs, line);

    int vertex = std::stoi(line); //> O primeiro argumento tem que ser um inteiro (quantidade de vertices)

    bool directed_set{false};
    bool weighted_set{false};
    bool weighted{false};
    int edges_parsed{0};

    auto is_digraph = [&](psr::GraphOption opt) -> bool {
        return opt == psr::GraphOption::DIGRAPH;
    };

    bool directed = is_digraph(graphOption);
    Graph<Type> graph(vertex, directed, weighted); //> Cria um grafo.

    auto parse_vertex_token = [&](const std::string& token) -> Type {
        if constexpr (std::is_same_v<Type, char>) {
            if (token.size() != 1) {
                throw std::invalid_argument(
                    "Error! Expected a single character vertex token, got: '" + token + "'"
                );
            }
            return token[0];
        } else if constexpr (std::is_same_v<Type, int>) {
            //> 1) tenta número
            try {
                size_t pos = 0;
                int value = std::stoi(token, &pos);
                if (pos == token.size()) {
                    return value;
                }
            } catch (const std::exception&) {
                // continua
            }

            //> 2) tenta letra (A->1, B->2, ...)
            if (token.size() == 1) {
                unsigned char ch = static_cast<unsigned char>(token[0]);
                if (std::isalpha(ch)) {
                    char up = static_cast<char>(std::toupper(ch));
                    return static_cast<int>(up - 'A' + 1);
                }
            }

            throw std::invalid_argument(
                "Error! Could not parse vertex token as int nor as letter (A->1...). Token: '" + token + "'"
            );
        } else {
            static_assert(!sizeof(Type), "TxtParser: unsupported vertex Type");
        }
    };

    while(std::getline(ifs, line)){ //> Leitura do arquivo inteiro

        if(line.empty())continue;   //> Caso seja uma linha em branco, ignora
        if(line[0] == '#')continue; //> Caso seja um comentário, ignora

        std::replace(line.begin(), line.end(), ',', ' '); //> Transforma vírgulas em espaços para facilitar leitura

        std::stringstream ss{line}; //> Stringstream para facilitar a leitura

        std::string tok1, tok2;
        if (!(ss >> tok1)) {
            continue;
        }

        //> Caso a linha tenha só 1 token, pode ser uma flag (0/1)
        if (!(ss >> tok2)) {
            int flag = -1;
            try {
                flag = std::stoi(tok1);
            } catch (const std::exception&) {
                throw std::invalid_argument(
                    "Error! Could not parse a 0/1 flag nor an edge. Line: " + line
                );
            }
            if (flag != 0 && flag != 1) {
                throw std::invalid_argument(
                    "Error! Flag lines must be 0 or 1. Line: " + line
                );
            }
            if (edges_parsed > 0) {
                throw std::invalid_argument(
                    "Error! Flags (directed/weighted) must appear before edges. Line: " + line
                );
            }

            if (!directed_set) {
                graphOption = flag ? psr::GraphOption::DIGRAPH : psr::GraphOption::GRAPH;
                directed = is_digraph(graphOption);
                directed_set = true;
                graph = Graph<Type>(vertex, directed, weighted);
                continue;
            }
            if (!weighted_set) {
                weighted = (flag == 1);
                weighted_set = true;
                graph = Graph<Type>(vertex, directed, weighted);
                continue;
            }

            throw std::invalid_argument(
                "Error! Unexpected extra flag line (already read directed and weighted flags). Line: " + line
            );
        }

        //> Caso tenha 2+ tokens, é uma aresta (com custo opcional)
        Type u = parse_vertex_token(tok1);
        Type v = parse_vertex_token(tok2);

        int cost = 0;
        bool has_cost = static_cast<bool>(ss >> cost);

        if (weighted) {
            if (!has_cost) {
                throw std::invalid_argument(
                    "Error! Weighted graph expects edges in format: U,V,COST (COST>=0). Line: " + line
                );
            }
            graph.add(u, v, cost);
        } else {
            graph.add(u, v);
        }

        edges_parsed++;
    }
    ifs.close(); //> Fecha o arquivo.

    return graph; //> Retorna o grafo gerado
}


bool verify_args(const int argc, const char* argv[], GraphType& graphType, psr::GraphOption& graphOption, std::string& filePath){

    bool has_input_file{false};
    for(int i{1}; i < argc; ++i){
        if((std::string(argv[i]) == "-i" || std::string(argv[i]) == "--int") && graphType == GraphType::NONE){
            graphType = GraphType::INT;
        }
        else if((std::string(argv[i]) == "-c" || std::string(argv[i]) == "--char") && graphType == GraphType::NONE){
            graphType = GraphType::CHAR;
        }
        else if((std::string(argv[i]) == "-d" || std::string(argv[i]) == "--directed")){
            graphOption = psr::GraphOption::DIGRAPH;
        }
        else if(!has_input_file && argv[i][0] != '-'){
            filePath = std::string(argv[i]);
            has_input_file = true;
        }
        else{
            return false;
        }
    }
    if (!has_input_file) {
        std::cerr << "Erro: Caminho do arquivo nao fornecido.\n";
        return false;
    }

    if(graphType == GraphType::NONE){
        graphType = GraphType::INT;
    }

    return true;
}

#endif