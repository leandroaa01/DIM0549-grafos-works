#ifndef TXT_PARSER_TPP
#define TXT_PARSER_TPP

#include "GraphParser.hpp"
#include <string>
#pragma once

#include "TxtParser.hpp"

template <typename Type>
Graph<Type> TxtParser<Type>::parse(const std::string& filePath){
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

    Graph<Type> graph(vertex);

    while(std::getline(ifs, line)){

        if(line.empty())continue;

        std::stringstream ss{line};
        std::pair<Type, Type> pair;

        ss >> pair.first;
        if(ss.peek() == ',') ss.ignore();
        
        if(ss >> pair.second){
            graph.add(pair);
        }
        else{
            if(isFirstLineAfterVertex){
                Graph<Type> graph(vertex, std::stoi(line));
            }
        }
        isFirstLineAfterVertex = false;
    }
    ifs.close();

    return graph;
}

#endif