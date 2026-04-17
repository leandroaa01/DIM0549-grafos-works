/**
 * @file TxtParser.hpp
 * @author  Leandro Andrade(email:)
 * @author  Heitor Campos()
 * @brief  classe que representa um parser para arquivos .txt
 * @version 0.1
 * @date 2026-04-07
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef TXT_PARSER_HPP
#define TXT_PARSER_HPP

#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <stdexcept>
#include <utility>


#include "../grafos.hpp"
#include "GraphParser.hpp"

namespace fs = std::filesystem;

#ifndef GRAPH_TYPE
#define GRAPH_TYPE
enum class GraphType{
    INT,
    CHAR,
    NONE
};
#endif


template <typename Type>
    /**
      * @brief Classe que implementa a estrutura "Parser" para a leitura dos arquivos .txt.
      * 
      * @tparam Type tipo do grafo que será lido
      */
class TxtParser : public psr::Parser<Type> {

    public:

        /**
             * @brief Função que recebe o caminho do arquivo de leitura e ua flag que diz se o grafo é ou não direcionado e processa
             * cada dado recebido no arquivo e envia para um grafo de mesmo tipo
             * 
             * @param filePath representa o caminho do arquivo que será lido
             * @param graphOption flag que diz se o grafo é ou não direcionado
             * @return Graph<Type> com as configurações processadas
             */
        Graph<Type> parse(const std::string& filePath, psr::GraphOption graphOption) override;
};

#include "TxtParser.tpp"
#endif