#ifndef PARSER_HPP
#define PARSER_HPP
#pragma once

#include "../grafos.hpp"

namespace psr {

    enum class ParserType {
                TXT=0,
                INI,
                XML
            };

    enum class GraphOption{
            DIGRAPH=0,
            GRAPH
        };

    template <typename Type>
    class Parser {
        public:
            Parser() = default;
            virtual ~Parser() = default;
            virtual Graph<Type> parse(const std::string& filePath) = 0;

    };
}
#endif