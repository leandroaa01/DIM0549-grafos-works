#ifndef TXT_PARSER_TPP
#define TXT_PARSER_TPP

#pragma once

#include "TxtParser.hpp"

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

    bool isFirstLineAfterVertex{true}; //> Flag que diz se a leitura do parâmetro "direcionado" foi realizado.

    Graph<Type> graph(vertex, graphOption); //> Cria um grafo.

    while(std::getline(ifs, line)){ //> Leitura do arquivo inteiro

        if(line.empty())continue;   //> Caso seja uma linha em branco, ignora

        std::replace(line.begin(), line.end(), ',', ' '); //> Transforma vírgulas em espaços para facilitar leitura

        std::stringstream ss{line}; //> Stringstream para facilitar a leitura
        std::pair<Type, Type> pair;     //> Par que representa a aresta que será lida

        
        if(ss >> pair.first >> pair.second){ //> Caso consiga ler dois seguidos, é uma aresta e ele lê como uma aresta
            graph.add(pair.first, pair.second);    
        }
        else if(isFirstLineAfterVertex){    //> Se não conseguir ler e a flag for true, então tem um parâmetro faltando na segunda linha,
                                            //> Quando possui apenas 1 parâmetro nela, é a flag que diz se o grafo é ou não direcionado
            int i = std::stoi(line);
            graphOption = (psr::GraphOption) std::stoi(line);
            graph = Graph<Type>(vertex, graphOption);   //> Transforma o grafo com base na flag recebida
        
        }

        
        isFirstLineAfterVertex = false; //> A flag se torna falsa a partir do momento em que não é mais a segunda linha
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