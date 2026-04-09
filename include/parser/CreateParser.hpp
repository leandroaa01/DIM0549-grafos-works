#ifndef CREATE_PARSER_HPP
#define CREATE_PARSER_HPP

#pragma once

#include "GraphParser.hpp"
// #include "IniParser.hpp"
#include "TxtParser.hpp"

#include <memory>
#include <stdexcept>

class CreateParser {
    public:

    template <typename Type>
    static std::unique_ptr<psr::Parser<Type>> create(psr::ParserType type);
};

#include "CreateParser.tpp"
#endif