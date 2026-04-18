#include "include/core/grafos.hpp"
#include "include/parser/CreateParser.tpp"
#include "include/parser/GraphParser.hpp"
#include "include/parser/TxtParser.hpp"
#include <cstdlib>
#include <iostream>

using raw_str = const char*;
using str = std::string;

inline raw_str MENU_OPCOES = R"(  
Slecione a opção que deseja utilizar:
    1 - Usar matriz de adjacências
    2 - Usar lista de adjacências
    3 - Usar matriz de incidências
    4 - Tamanho do grafo
    5 - Quantidade de arestas
    6 - Verificr se  o grfos é conexo
    7 - Verificar se o grafo é bipartido
    8 - Verificar se  dois vértices são adjacentes
    9 - Diz o grau de um vértice específico
    10 - Adicionar uma aresta 
    11 - Remover uma aresta 
    12 - Adicionar um vértice
    13 - Remover um vértice
    14 - Imprimir o grafo
    15 - Fazer busca em profundidade (DFS)
    16 - Fazer busca em largura (BFS)
    17 - Classificar as arestas de um grafo direcionado usando DFS
    18 - Imprimir as aticulações do grafo
    0 - Sair
)";

void print_menu() { std::cout << MENU_OPCOES << "\n"; }

void run(int argc, char const* argv[])
{
  GraphType gt{ GraphType::NONE };
  psr::GraphOption go{ psr::GraphOption::GRAPH };
  std::string filePath{ "" };
  if (argc < 2) {
    std::cerr << "Error: Wrong number of arguments.\n";
    std::cerr << "Usage: " << argv[0] << " <input_file_path>\n";
    return; // Retorna 1 para o sistema
  }
  if (argc > 2) {
    if (!verify_args(argc, argv, gt, go, filePath)) {
      return;
    }
  }
  if (gt == GraphType::CHAR) {
    std::unique_ptr<psr::Parser<char>> parser = CreateParser::create<char>(psr::ParserType::TXT);
    Graph<char> graph = parser->parse(filePath, go);

    int option;
    print_menu();
    std::cout << "Digite a opção desejada: ";
    std::cin >> option;
    while (option != 0) {
      print_menu();
      switch (option) {
        case 1:
          graph.to_matrix();
          std::cout << "Grafo em forma de " << graph.getRepresentation() << "\n";
          break;
        case 2:
          graph.to_list();
          std::cout << "Grafo em forma de " << graph.getRepresentation() << "\n";
          break;
        case 3:
          graph.to_incMat();
          std::cout << "Grafo em forma de " << graph.getRepresentation() << "\n";
          break;
        case 4:
          std::cout << "Tamanho do grafo: " << graph.total_vertices() << "\n";
          break;
        case 5:
          std::cout << "Quantidade de arestas: " << graph.total_edges() << "\n";
          break;
        case 6:
          std::cout << "O grafo é conexo? " << (graph.is_conexo() ? "Sim" : "Não") << "\n";
          break;
        case 7:
          std::cout << "O grafo é bipartido? " << (graph.is_bipartite() ? "Sim" : "Não") << "\n";
          break;
        case 8: {
          char v1, v2;
          std::cout << "Digite os dois vértices para verificar se são adjacentes (ex: A B): ";
          std::cin >> v1 >> v2;
          std::cout << "Os vértices " << v1 << " e " << v2 << " são adjacentes? "
                    << (graph.is_adjacent(v1, v2) ? "Sim" : "Não") << "\n";
          break;
        }
        case 9: {
          std::cout << "Digite o vértice para verificar seu grau: ";
          char vertex;
          std::cin >> vertex;
          int degree = graph.degree(vertex);
          std::cout << "O grau do vértice " << vertex << " é: " << degree << "\n";
          break;
        }
        case 10: {
          std::cout << "Digite os vértices de origem e destino para adicionar uma aresta (ex: A B): ";
          char origin, destiny;
          std::cin >> origin >> destiny;
          graph.add(origin, destiny);
          break;
        }
        case 11: {
          std::cout << "Digite os vértices de origem e destino para remover uma aresta (ex: A B): ";
          char originR, destinyR;
          std::cin >> originR >> destinyR;
          graph.remove(originR, destinyR);
          break;
        }
        case 12: {
          std::cout << "Digite o vértice a ser adicionado: ";
          char vertex;
          std::cin >> vertex;
          graph.add(vertex);
          break;
        }
        case 13: {
          std::cout << "Digite o vértice a ser removido: ";
          char vertexR;
          std::cin >> vertexR;
          graph.remove(vertexR);
          break;
        }
        case 14: {
          std::cout << "Imprimindo o grafo em forma de " << graph.getRepresentation() << "\n";
          graph.print();
          break;
        }
        case 15: {
          std::cout << "Digite o vértice de início para a busca em profundidade (DFS): ";
          char startDFS;
          std::cin >> startDFS;
          std::cout << "Resultado da busca em profundidade (DFS) a partir do vértice " << startDFS << ": ";
          graph.dfs(startDFS);
          break;
        }
        case 16: {
          std::cout << "Digite o vértice de início para a busca em largura (BFS): ";
          char startBFS;
          std::cin >> startBFS;
          std::cout << "Resultado da busca em largura (BFS) a partir do vértice " << startBFS << ": ";
          graph.bfs(startBFS);
          break;
        }
        case 17: {
          std::cout << "Digite o vértice de início para classificar as arestas usando DFS: ";
          char startDFSClassify;
          std::cin >> startDFSClassify;
          std::cout << "Classificação das arestas a partir do vértice " << startDFSClassify << ": ";
          graph.dfs_directed_classification(startDFSClassify);
          break;
        }
        case 18: {
          std::cout << "Articulações do grafo: ";
          graph.find_articulations();
          break;
        }
        default:
          std::cout << "Opção inválida. Por favor, tente novamente.\n";
          std::cin >> option;
          continue;
      }
      print_menu();
      std::cout << "Digite a opção desejada: ";
      std::cin >> option;
    }
  } else {
    std::unique_ptr<psr::Parser<int>> parser = CreateParser::create<int>(psr::ParserType::TXT);
    Graph<int> graph = parser->parse(filePath, go);
     int option;
    print_menu();
    std::cout << "Digite a opção desejada: ";
    std::cin >> option;
    while (option != 0) {
      print_menu();
      switch (option) {
        case 1:
          graph.to_matrix();
          std::cout << "Grafo em forma de " << graph.getRepresentation() << "\n";
          break;
        case 2:
          graph.to_list();
          std::cout << "Grafo em forma de " << graph.getRepresentation() << "\n";
          break;
        case 3:
          graph.to_incMat();
          std::cout << "Grafo em forma de " << graph.getRepresentation() << "\n";
          break;
        case 4:
          std::cout << "Tamanho do grafo: " << graph.total_vertices() << "\n";
          break;
        case 5:
          std::cout << "Quantidade de arestas: " << graph.total_edges() << "\n";
          break;
        case 6:
          std::cout << "O grafo é conexo? " << (graph.is_conexo() ? "Sim" : "Não") << "\n";
          break;
        case 7:
          std::cout << "O grafo é bipartido? " << (graph.is_bipartite() ? "Sim" : "Não") << "\n";
          break;
        case 8: {
          int v1, v2;
          std::cout << "Digite os dois vértices para verificar se são adjacentes (ex: 1 2): ";
          std::cin >> v1 >> v2;
          std::cout << "Os vértices " << v1 << " e " << v2 << " são adjacentes? "
                    << (graph.is_adjacent(v1, v2) ? "Sim" : "Não") << "\n";
          break;
        }
        case 9: {
          std::cout << "Digite o vértice para verificar seu grau: ";
          int vertex;
          std::cin >> vertex;
          int degree = graph.degree(vertex);
          std::cout << "O grau do vértice " << vertex << " é: " << degree << "\n";
          break;
        }
        case 10: {
          std::cout << "Digite os vértices de origem e destino para adicionar uma aresta (ex: 1 2): ";
          int origin, destiny;
          std::cin >> origin >> destiny;
          graph.add(origin, destiny);
          break;
        }
        case 11: {
          std::cout << "Digite os vértices de origem e destino para remover uma aresta (ex: 1 2): ";
          int originR, destinyR;
          std::cin >> originR >> destinyR;
          graph.remove(originR, destinyR);
          break;
        }
        case 12: {
          std::cout << "Digite o vértice a ser adicionado: ";
          int vertex;
          std::cin >> vertex;
          graph.add(vertex);
          break;
        }
        case 13: {
          std::cout << "Digite o vértice a ser removido: ";
          int vertexR;
          std::cin >> vertexR;
          graph.remove(vertexR);
          break;
        }
        case 14: {
          std::cout << "Imprimindo o grafo em forma de " << graph.getRepresentation() << "\n";
          graph.print();
          break;
        }
        case 15: {
          std::cout << "Digite o vértice de início para a busca em profundidade (DFS): ";
          int startDFS;
          std::cin >> startDFS;
          std::cout << "Resultado da busca em profundidade (DFS) a partir do vértice " << startDFS << ": ";
          graph.dfs(startDFS);
          break;
        }
        case 16: {
          std::cout << "Digite o vértice de início para a busca em largura (BFS): ";
          int startBFS;
          std::cin >> startBFS;
          std::cout << "Resultado da busca em largura (BFS) a partir do vértice " << startBFS << ": ";
          graph.bfs(startBFS);
          break;
        }
        case 17: {
          std::cout << "Digite o vértice de início para classificar as arestas usando DFS: ";
          int startDFSClassify;
          std::cin >> startDFSClassify;
          std::cout << "Classificação das arestas a partir do vértice " << startDFSClassify << ": ";
          graph.dfs_directed_classification(startDFSClassify);
          break;
        }
        case 18: {
          std::cout << "Articulações do grafo: ";
          graph.find_articulations();
          break;
        }
        default:
          std::cout << "Opção inválida. Por favor, tente novamente.\n";
          std::cin >> option;
          continue;
      }
      print_menu();
      std::cout << "Digite a opção desejada: ";
      std::cin >> option;
    }
  }
}

int main(int argc, char const* argv[])
{

  run(argc, argv);
  return 0;
}
