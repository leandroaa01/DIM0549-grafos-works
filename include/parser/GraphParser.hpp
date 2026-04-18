/**
 * @file GraphParser.hpp
 * @author  Leandro Andrade(email:)
 * @author  Heitor Campos()
 * @brief  classe abstrata que representa a estrutura de um parser abstrato. 
 * @version 0.1
 * @date 2026-04-07
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef PARSER_HPP
#define PARSER_HPP
#pragma once

#include "../core/grafos.hpp"

namespace psr {

    enum class ParserType { //> Enum que guarda qual arquivo o parser irá ler (apenas .txt neste trabalho :c )
                TXT=0,
                INI,
                XML
            };

    enum GraphOption{   //> Enum que guarda se o grafo é ou não direcionado
        GRAPH=0,
        DIGRAPH    
    };

    template <typename Type>
     /**
      * @brief Classe abstrata que representa uma estrutura "Parser" genérica para a leitura de arquivos
      * 
      * @note Essa classe deverá ser extendida para Parsers específicos de cada leitura
      * @tparam Type tipo do grafo que será lido
      */
    class Parser {
        public:
            Parser() = default; //> Construtor padrão da classe
            virtual ~Parser() = default; //> Destrutor padrão da classe

            /**
             * @brief Função que recebe o caminho do arquivo de leitura e uma flag que diz se o grafo é ou não direcionado e processa
             * cada dado recebido no arquivo.txt e envia para um grafo de mesmo tipo Type
             * 
             * @param filePath representa o caminho do arquivo que será lido
             * @param graphOption flag que diz se o grafo é ou não direcionado
             * @return Graph<Type> com as configurações processadas
             */
            virtual Graph<Type> parse(const std::string& filePath, GraphOption graphOption) = 0;

    };
}
#endif