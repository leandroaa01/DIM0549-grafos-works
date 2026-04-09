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


#include "GraphParser.hpp"

namespace fs = std::filesystem;

template <typename Type>
class TxtParser : public psr::Parser<Type> {

    public:
        Graph<Type> parse(const std::string& filePath) override;
};

#include "TxtParser.tpp"
#endif