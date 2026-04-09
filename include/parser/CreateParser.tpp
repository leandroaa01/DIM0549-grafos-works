#ifndef CREATE_PARSER_TPP
#define CREATE_PARSER_TPP

#pragma once

#include "CreateParser.hpp"

template <typename Type>
std::unique_ptr<psr::Parser<Type>> CreateParser::create(psr::ParserType type){
    switch(type){
        case psr::ParserType::TXT : return std::make_unique<TxtParser<Type>>();
        // case psr::ParserType::INI : return std::make_unique<IniParser<Type>>();
        default                   : throw std::invalid_argument("Error: Invalid file type! Try to use a .txt or .INI file!");
    }
}

#endif