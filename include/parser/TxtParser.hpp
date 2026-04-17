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
class TxtParser : public psr::Parser<Type> {

    public:
        Graph<Type> parse(const std::string& filePath, psr::GraphOption graphOption) override;
};

#include "TxtParser.tpp"
#endif