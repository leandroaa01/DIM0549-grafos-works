/**
 * @file CreateParser.hpp
 * @author  Leandro Andrade(email:)
 * @author  Heitor Campos()
 * @brief  classe que constrói o parser com base no tipo de arquivo lido
 * @version 0.1
 * @date 2026-04-07
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef CREATE_PARSER_HPP
#define CREATE_PARSER_HPP

#pragma once

#include "GraphParser.hpp"
#include "TxtParser.hpp"

#include <memory>
#include <stdexcept>

    /**
      * @brief Classe responsável por criar o parser com base no tipo passado
      * 
      * @note Apenas implementado o tipo TXT neste trabalho
      */
class CreateParser {
    public:

    /**
      * @brief Função que cria o parser com base no tipo selecionado
      * 
      * @note Apenas implementado o tipo TXT neste trabalho
      * @tparam Type tipo do grafo que será extraído pelo parser
      * @param type representa o tipo de arquivo que o parser irá ler
      */
    template <typename Type>
    static std::unique_ptr<psr::Parser<Type>> create(psr::ParserType type);
};

#include "CreateParser.tpp"
#endif