#ifndef TXT_PARSER_TPP
#define TXT_PARSER_TPP



#pragma once

#include "TxtParser.hpp"

template <typename Type>
Graph<Type> TxtParser<Type>::parse(const std::string& filePath, psr::GraphOption graphOption){
    if(filePath.empty()){
        throw std::invalid_argument("Error! The file path provided is empty.");
    }
    fs::path p{filePath};

    if(!fs::exists(p)){
        throw std::invalid_argument("Error! File not found: " + filePath + '\n');
    }



    std::ifstream ifs{filePath};
    std::string line;

    std::getline(ifs, line);

    int vertex = std::stoi(line);

    bool isFirstLineAfterVertex{true};

    Graph<Type> graph(vertex, graphOption);

    while(std::getline(ifs, line)){

        if(line.empty())continue;

        std::replace(line.begin(), line.end(), ',', ' ');

        std::stringstream ss{line};
        std::pair<Type, Type> pair;

        
        if(ss >> pair.first >> pair.second){
            graph.add(pair);    
        }
        else if(isFirstLineAfterVertex){
            int i = std::stoi(line);
            graphOption = (psr::GraphOption) std::stoi(line);
            graph = Graph<Type>(vertex, graphOption);
        
        }

        
        isFirstLineAfterVertex = false;
    }
    ifs.close();

    return graph;
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