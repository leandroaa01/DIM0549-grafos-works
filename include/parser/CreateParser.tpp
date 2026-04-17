#ifndef CREATE_PARSER_TPP
#define CREATE_PARSER_TPP

#pragma once

#include "CreateParser.hpp"

template <typename Type>
std::unique_ptr<psr::Parser<Type>> CreateParser::create(psr::ParserType type){
    switch(type){
        case psr::ParserType::TXT : return std::make_unique<TxtParser<Type>>();     //> Caso a escolha seja TXT, cria um parser do tipo TxtParser
        default                   : throw std::invalid_argument("Error: Invalid file type! Try to use a .txt or .INI file!"); //> Inválido.
    }
}

#endif