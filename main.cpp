#include "include/core/grafos.hpp"
#include "include/parser/CreateParser.tpp"
#include "include/parser/GraphParser.hpp"
#include "include/parser/TxtParser.hpp"
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

using raw_str = const char*;
using str = std::string;

inline raw_str MENU_OPCOES = R"(  
Selecione a opção que deseja utilizar:
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
    19 - Executar o algoritmo de Kuskal para encontrar a árvore geradora mínima do grafo
    0 - Sair
)";

void print_menu() { std::cout << MENU_OPCOES << "\n"; }

template <typename T>
void interactive_menu(Graph<T>& graph)
{
  constexpr bool is_char = std::is_same_v<T, char>;
  auto example_pair = []() -> const char* { return is_char ? "A B" : "1 2"; };

  auto read_vertex = [](const std::string& prompt) -> T {
    T v{};
    std::cout << prompt;
    std::cin >> v;
    std::cout << '\n';
    return v;
  };

  auto read_pair = [](const std::string& prompt) -> std::pair<T, T> {
    T a{}, b{};
    std::cout << prompt;
    std::cin >> a >> b;
    return { a, b };
  };

  std::map<int, std::function<void()>> actions;
  actions[1] = [&] {
    graph.to_matrix();
    std::cout << "Grafo em forma de " << graph.getRepresentation() << "\n";
  };
  actions[2] = [&] {
    graph.to_list();
    std::cout << "Grafo em forma de " << graph.getRepresentation() << "\n";
  };
  actions[3] = [&] {
    graph.to_incMat();
    std::cout << "Grafo em forma de " << graph.getRepresentation() << "\n";
  };
  actions[4] = [&] { std::cout << "Tamanho do grafo: " << graph.total_vertices() << "\n"; };
  actions[5] = [&] { std::cout << "Quantidade de arestas: " << graph.total_edges() << "\n"; };
  actions[6] = [&] { std::cout << "O grafo é conexo? " << (graph.is_conexo() ? "Sim" : "Não") << "\n"; };
  actions[7] = [&] { std::cout << "O grafo é bipartido? " << (graph.is_bipartite() ? "Sim" : "Não") << "\n"; };
  actions[8] = [&] {
    auto [v1, v2] = read_pair(std::string("Digite os dois vértices para verificar se são adjacentes (ex: ") + example_pair() + "): ");
    std::cout << "Os vértices " << v1 << " e " << v2 << " são adjacentes? " << (graph.is_adjacent(v1, v2) ? "Sim" : "Não")
              << "\n";
  };
  actions[9] = [&] {
    T vertex = read_vertex("Digite o vértice para verificar seu grau: ");
    int degree = graph.degree(vertex);
    std::cout << "O grau do vértice " << vertex << " é: " << degree << "\n";
  };
  actions[10] = [&] {
    auto [origin, destiny] = read_pair(std::string("Digite os vértices de origem e destino para adicionar uma aresta (ex: ") + example_pair() + "): ");
    graph.add(origin, destiny);
  };
  actions[11] = [&] {
    auto [origin, destiny] = read_pair(std::string("Digite os vértices de origem e destino para remover uma aresta (ex: ") + example_pair() + "): ");
    graph.remove(origin, destiny);
  };
  actions[12] = [&] {
    T vertex = read_vertex("Digite o vértice a ser adicionado: ");
    graph.add(vertex);
  };
  actions[13] = [&] {
    T vertex = read_vertex("Digite o vértice a ser removido: ");
    graph.remove(vertex);
  };
  actions[14] = [&] {
    std::cout << "Imprimindo o grafo em forma de " << graph.getRepresentation() << "\n";
    graph.print();
  };
  actions[15] = [&] {
    T start = read_vertex("Digite o vértice de início para a busca em profundidade (DFS): ");
    std::cout << "Resultado da busca em profundidade (DFS) a partir do vértice " << start << ": \n";
    graph.dfs(start);
  };
  actions[16] = [&] {
    T start = read_vertex("Digite o vértice de início para a busca em largura (BFS): ");
    std::cout << "Resultado da busca em largura (BFS) a partir do vértice " << start << ": \n";
    graph.bfs(start);
  };
  actions[17] = [&] {
    T start = read_vertex("Digite o vértice de início para classificar as arestas usando DFS: ");
    std::cout << "Classificação das arestas a partir do vértice " << start << ": \n";
    graph.dfs_directed_classification(start);
  };
  actions[18] = [&] {
    std::cout << "Articulações do grafo: \n";
    graph.find_articulations();
  };
  actions[19] = [&]{
    std::cout <<"Algoritmo de kuskal para caminho mínimo: \n";
    auto k=  graph.kuskal();
  };
  
  while (true) {
    print_menu();
    std::cout << "Digite a opção desejada: ";

    int option = 0;
    std::cin >> option;
    if (option == 0) {
      return;
    }

    auto it = actions.find(option);
    if (it == actions.end()) {
      std::cout << "Opção inválida. Por favor, tente novamente.\n";
      continue;
    }

    it->second();
  }
}

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

  filePath = argv[1];

  if (argc > 2) {
    if (!verify_args(argc, argv, gt, go, filePath)) {
      return;
    }
  }
  try {
    if (gt == GraphType::CHAR) {
      std::unique_ptr<psr::Parser<char>> parser = CreateParser::create<char>(psr::ParserType::TXT);
      Graph<char> graph = parser->parse(filePath, go);
      interactive_menu(graph);
    } else {
      std::unique_ptr<psr::Parser<int>> parser = CreateParser::create<int>(psr::ParserType::TXT);
      Graph<int> graph = parser->parse(filePath, go);
      interactive_menu(graph);
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return;
  }
}



int main(int argc, char const* argv[])
{
  run(argc, argv);
  return 0;
}
