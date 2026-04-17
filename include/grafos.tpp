
#pragma once

#include <cstddef>
#include <vector>
#ifndef GRAPH_TPP
#define GRAPH_TPP

#include "grafos.hpp"

using rep = Representation;

template <typename T>
int Graph<T>::get_vertex_index(T vertex, bool create) {
    auto it = m_vertex_index.find(vertex); //> Procura o vértice no mapa de índices
    if (it != m_vertex_index.end()) { //se o vértice for encontrado, retorna seu índice
        return it->second;
    }

    if (not create) { //> usa na conversão dos tipos, para não criar novos vértices
        return -1;
    }

    if (static_cast<int>(m_vertex_index.size()) >= m_vertices) { //> Verifica se o número máximo de vértices foi atingido
        std::cerr << "Error: maximum number of vertices reached." << std::endl;
        return -1;
    }

    int index = static_cast<int>(m_vertex_index.size()); //> Atribui um novo índice baseado no tamanho atual do mapa
    m_vertex_index[vertex] = index; //> Adiciona o vértice ao mapa com seu índice correspondente
    return index;
}
template <typename T>
T Graph<T>::get_vertex_label(int index) {
    auto it = std::find_if(m_vertex_index.begin(), m_vertex_index.end(), 
                           [index](const auto& pair) { return pair.second == index; });
    if (it != m_vertex_index.end()) return it->first;
    return T();                 //> Retorna um valor padrão se não achar
}

template <typename T>
void Graph<T>::to_list() {
    
    if (graphRep == rep::ADJACENCY_LIST) { return; } //> Se o grafo já estiver usando lista de adjacências, não é necessário converter

    m_list.clear(); //> Limpa a lista de adjacências antes de preenchê-la
    m_list.resize(m_vertices); //> Redimensiona a lista de adjacências para o número de vértices

    if(graphRep == rep::ADJACENCY_MATRIX){
        for (int i = 0; i < m_vertices; ++i) {
            for (int j = 0; j < m_vertices; ++j) {
                if (m_matrix[i][j] != 0) {
                    //> Encontra o rótulo do vértice j correspondente ao índice j
                    auto neighbor = std::find_if(m_vertex_index.begin(), m_vertex_index.end(), [j](const auto& pair) {
                        return pair.second == j;
                    });
                    //> Se o rótulo do vértice j for encontrado, adiciona-o à lista de adjacências do vértice i
                    if (neighbor != m_vertex_index.end()) {
                        m_list[i].push_back(neighbor->first);
                    }
                }
            }
        }
    }
    else{
        for (int e{0}; e < m_edges; ++e) {
            int origin{-1}, destiny{-1};
            for (int i{0}; i < m_vertices; ++i) {
                if (m_inc_matrix[i][e] == -1) {     //> Define origem como o vértice que possui -1
                    origin = i;
                } else if (m_inc_matrix[i][e] == 1) {
                    if(is_targeted){                //> Caso seja direcionado, define o destino como o vértice que possui 1
                        destiny = i;
                    }else{
                        if(origin == -1){       //> Caso não seja direcionado, define o primeiro vértice que possui 1 como origem, e o seguindo como destino
                            origin = i;
                        }
                        else{
                            destiny = i;
                        }
                    }
                }
            }
            if (origin != -1 && destiny != -1) {                                                       //> Caso direcionado
                auto label_dest = std::find_if(m_vertex_index.begin(), m_vertex_index.end(), [destiny](const auto& pair) { return pair.second == destiny; });
                if (label_dest != m_vertex_index.end()) m_list[origin].push_back(label_dest->first);   
                                                                                                       //> Caso ache um par de origem e destino, adiciona à lista
                                                                                                       //> de adjacências

                if (!is_targeted) {                                                                    //> Caso não direcionado
                    auto label_orig = std::find_if(m_vertex_index.begin(), m_vertex_index.end(), [origin](const auto& pair) { return pair.second == origin; });
                    if (label_orig != m_vertex_index.end()) m_list[destiny].push_back(label_orig->first);
                }
            }
        }
    }

    graphRep = rep::ADJACENCY_LIST; //> Define a flag para indicar que o grafo agora está usando lista de adjacências
}

template <typename T>
void Graph<T>::to_matrix() {
    if (graphRep == rep::ADJACENCY_MATRIX) { return;  } //> Se o grafo já estiver usando matriz de adjacências, não é necessário converter
    if (graphRep != rep::ADJACENCY_LIST) {this->to_list();} //> Para garantir eficiência, utiliza a forma de lista como um pivô
    m_matrix.clear(); //> Limpa a matriz de adjacências antes de preenchê-la
    m_matrix.resize(m_vertices, std::vector<int>(m_vertices, 0)); //> Redimensiona a matriz de adjacências para o número de vértices, inicializando com zeros

    if(graphRep == rep::ADJACENCY_LIST){
        //> Converte de lista de adjacências para matriz de adjacências
        for (int i = 0; i < m_vertices; ++i) {
            for (const auto& neighbor : m_list[i]) {
                auto neighbor_index = get_vertex_index(neighbor, false); //> Obtém o índice do vértice vizinho sem criar um novo vértice
                if (neighbor_index >= 0) {
                    m_matrix[i][neighbor_index] = 1;
                }
            }
        }
    }
    
    graphRep = rep::ADJACENCY_MATRIX; //> Define a flag para indicar que o grafo agora está usando matriz de adjacências
}
template <typename T>
void Graph<T>::to_incMat(){
    if(graphRep == rep::INCIDENCY_MATRIX){ return; }
    if(graphRep != rep::ADJACENCY_LIST)  {this->to_list();}
    
    m_inc_matrix.clear();
    m_inc_matrix.resize(m_vertices, std::vector<int>(m_edges, 0));

    int edge{0}; //> Variável que guarda o indice de cada aresta da matriz dentro do loop

    std::vector<std::vector<bool>> visited(m_vertices, std::vector<bool>(m_vertices, false)); //> Matriz de booleanos que verifica se cada aresta foi vista.

    for(int i{0}; i < m_vertices; ++i){
        for(const auto& neighbor : m_list[i]){
            int index = get_vertex_index(neighbor, false);
            if(!is_targeted && visited[index][i])continue;  //> Caso não seja direcionado, evitar contar a mesma aresta 2 vezes.
            if(edge < m_edges){             
                m_inc_matrix[i][edge] = 1;
                m_inc_matrix[index][edge] = is_targeted ? -1 : 1; //> Caso seja direcionado, o vértice de saída fica como -1
                visited[i][index] = true;
                ++edge;
            }

        }
    }
    graphRep = rep::INCIDENCY_MATRIX;
}

template <typename T>
void Graph<T>::add(T origin, T destiny) {
    int origin_index = get_vertex_index(origin); //> Obtém o índice do vértice de origem, criando-o se necessário
    int destiny_index = get_vertex_index(destiny); //> Obtém o índice do vértice de destino, criando-o se necessário

     if (origin_index < 0 or destiny_index < 0) { 
        std::cerr << "Error: vertex index out of bounds." << std::endl;
        return;
    }

    if (origin_index < 0  or destiny_index < 0) {
        return;
    }
    bool edge_exists = false;
    if (graphRep == rep::ADJACENCY_LIST) {
        edge_exists = std::find(m_list[origin_index].begin(), m_list[origin_index].end(), destiny) != m_list[origin_index].end();
    } else {
        if (m_matrix[origin_index][destiny_index] == 1) edge_exists = true;
    }

    if (not edge_exists) {
        m_edges++; //> Incrementa o contador de arestas do grafo apenas se a aresta não existir
    }

     if (graphRep == rep::ADJACENCY_LIST) {
        m_list[origin_index].push_back(destiny); //> Adiciona o vértice de destino à lista de adjacências do vértice de origem
        if( not is_targeted){
             m_list[destiny_index].push_back(origin); //> Adiciona o vértice de origem à lista de adjacências do vértice de destino (grafo não direcionado)
        }
    } else if(graphRep == rep::ADJACENCY_MATRIX) {
        m_matrix[origin_index][destiny_index] = 1; //> Marca a presença da aresta na matriz de adjacências
            if( not is_targeted){
                m_matrix[destiny_index][origin_index] = 1; //> Marca a presença da aresta na matriz de adjacências (grafo não direcionado)
            }
    }else {
        for(int i{0}; i < m_vertices; ++i){
            m_inc_matrix[i].push_back(0);   //> Cria uma nova coluna na matriz de incidências
        }
        m_inc_matrix[origin_index][m_edges - 1] = 1; //> Adiciona o vertice de origem da aresta
        m_inc_matrix[destiny_index][m_edges - 1]  = is_targeted ? -1 : 1; //> Adiciona o vertice de destino da aresta
    }
}
template <typename T>
void Graph<T>::add(std::pair<T, T> edge){
    add(edge.first, edge.second);
}

template <typename T>
void Graph<T>::remove(T origin, T destiny) {
    int origin_index = get_vertex_index(origin, false); //> Obtém o índice do vértice de origem sem criar um novo vértice
    int destiny_index = get_vertex_index(destiny, false); //> Obtém o índice do vértice de destino sem criar um novo vértice

    if (origin_index < 0 or destiny_index < 0) {
        std::cerr << "Error: vertex not found." << std::endl;
        return;
    }

    if (graphRep == rep::ADJACENCY_LIST) {
        m_list[origin_index].remove(destiny); //> Remove o vértice de destino da lista de adjacências do vértice de origem
        if(!is_targeted){
            m_list[destiny_index].remove(origin);
        }
    } else if(graphRep == rep::ADJACENCY_MATRIX){
        m_matrix[origin_index][destiny_index] = 0;
        if(!is_targeted){
            m_matrix[destiny_index][origin_index] = 0;
        }
    }
    else{
        for(int e{0}; e < m_edges; ++e){
            if(m_inc_matrix[origin_index][e] == 1 && m_inc_matrix[destiny_index][e] == (is_targeted ? -1 : 1)){
                for(int i{0}; i < m_vertices; i++){
                    m_inc_matrix[i][e] = m_inc_matrix[i].back();
                    m_inc_matrix.pop_back();
                }
                break;
            }
        }
    }

    m_edges--; //> Decrementa o contador de arestas do grafo
}

template <typename T>
void Graph<T>::print() const {
    if (graphRep == rep::ADJACENCY_LIST) {
        for (int i = 0; i < m_vertices; ++i) {
            //> Encontra o rótulo do vértice correspondente ao índice i
            auto label = std::find_if(m_vertex_index.begin(), m_vertex_index.end(), [i](const auto& pair) {
                return pair.second == i;
            });

            if (label != m_vertex_index.end()) {
                std::cout << "A{" << label->first << "}: { ";
            } else {
                std::cout << "[ ? ]: ";
            }
            for (const auto& neighbor : m_list[i]) {
                std::cout << neighbor << ", ";
            }
            std::cout << "}";

            std::cout << '\n';
        }
        return;
    } else if (graphRep == rep::INCIDENCY_MATRIX) {
        for (int e = 0; e < m_edges; ++e) {
            std::cout << "E" << e << " ";
        }
        std::cout << "\n";
        for (int i = 0; i < m_vertices; ++i) {
            auto valor = std::find_if(m_vertex_index.begin(), m_vertex_index.end(), [i](const auto& pair) {
                return pair.second == i;
            });
            if (valor != m_vertex_index.end()) std::cout << valor->first << "| ";
            else std::cout << "?| ";

            for (int e = 0; e < m_edges; ++e) {
                if (m_inc_matrix[i][e] >= 0) std::cout << " "; //> Apenas para alinhar negativos com positivos.
                std::cout << m_inc_matrix[i][e] << " ";
            }
            std::cout << "\n";
        }
    }
    std::cout << "   ";
    for (int i = 0; i < m_vertices; ++i) {
        auto valor = std::find_if(m_vertex_index.begin(), m_vertex_index.end(), [i](const auto& pair) {
            return pair.second == i;
        });

        if (valor != m_vertex_index.end()) {
            std::cout << valor->first << " ";
        } else {
            std::cout << "? ";
        }
    }
    std::cout << "\n";
    for (int i = 0; i < m_vertices; ++i) {
        auto valor = std::find_if(m_vertex_index.begin(), m_vertex_index.end(), [i](const auto& pair) {
            return pair.second == i;
        });

        if (valor != m_vertex_index.end()) {
            std::cout << valor->first << "| ";
        } else {
            std::cout << "?| ";
        }

        for (int j = 0; j < m_vertices; ++j) {
            std::cout << m_matrix[i][j] << " ";
        }
        std::cout << "| \n";
    }
}

template <typename T>
int Graph<T>::degree(T vertex) {
    to_list(); //> Converte para lista de adjacências para facilitar o cálculo do grau do vértice
    int vertex_index = get_vertex_index(vertex, false); //> Obtém o índice do vértice sem criar um novo vértice
    if (vertex_index < 0) {
        std::cerr << "Error: vertex not found." << std::endl;
        return -1;  
    }
    auto degree = m_list[vertex_index].size(); //> Retorna o número de vizinhos do vértice, que corresponde ao seu grau
    to_matrix(); //> Converte de volta para matriz de adjacências para manter a consistência da representação do grafo
    return degree;
}

template <typename T>
bool Graph<T>::is_adjacent(T vertex1, T vertex2) {
    to_matrix(); //> Converte para matriz de adjacências para facilitar a verificação de adjacência entre os vértices
    int vertex1_index = get_vertex_index(vertex1, false); //> Obtém o índice do primeiro vértice sem criar um novo vértice
    int vertex2_index = get_vertex_index(vertex2, false); //> Obtém o índice do segundo vértice sem criar um novo vértice

    if (vertex1_index < 0 or vertex2_index < 0) {
        std::cerr << "Error: vertex not found." << std::endl;
        return false;  
    }

    return m_matrix[vertex1_index][vertex2_index] != 0; //> Verifica se há uma aresta entre os dois vértices na matriz de adjacências
}

template <typename T>
std::vector<T> Graph<T>::bfs(T start_vertex){
    std::vector<bool> visited(m_vertices, false); //> Vetor que guarda se cada vértice foi visidado
    std::vector<T> visit_order{};                          //> Vetor que guarda a ordem em que cada vértice foi visitado
    std::queue<int> q;                                     //> Fila (FIFO) de apoio para gerenciar o caminho da busca

    int start_index = get_vertex_index(start_vertex, false);
    if(start_index == -1)return visit_order;    //> start_vertex não esteja no grafo, retorna um vetor vazio.

    this->to_list();    //> Otimização: realizar a busca em uma lista de adjacência é mais rápido que as matrizes de adjacência e incidência

    visited[start_index] = true;
    q.push(start_index);    //> Começa colocando o vértice "raiz" na fila

    while(!q.empty()){    
        int v = q.front();                                 //> Chama o topo da fila de "v"
        q.pop();
        visit_order.push_back(get_vertex_label(v));  //> Adiciona o vértice v na ordem de encontro

        for(auto& neightbor : m_list[v]){                   //> Visita os vizinhos do vértice v
            int w = get_vertex_index(neightbor, false);
            if(w != -1 && !visited[w]){                     //> Caso o vizinho não tenha sido visitado, visita e coloca na fila
                visited[w] = true;              
                q.push(w);
            }
        }
    }
    return visit_order;                                     //> Retorna o vetor com a ordem de visitas
}
template <typename T>
void Graph<T>::dfs_rec(int index, std::vector<T>& visit_order, std::vector<bool>& visited){
    visited[index] = true;
    visit_order.push_back(get_vertex_label(index));

    for(auto& neightbor : m_list[index]){
        int w = get_vertex_index(neightbor, false);
        if(w != -1 && !visited[w]){                         //> Se o vizinho não for visitado
            dfs_rec(w, visit_order, visited);   //> Chamada recursiva para o vizinho
        }
    }
}
template <typename T>
std::vector<T> Graph<T>::dfs(T start_vertex){
    std::vector<T> visit_order;
    int start_index = get_vertex_index(start_vertex);
    if(start_index == -1)return visit_order;

    this->to_list();

    std::vector<bool> visited(m_vertices, false);
    
    dfs_rec(start_index, visit_order, visited);

    return visit_order;
}

template <typename T>
bool Graph<T>::is_conexo() {
    if (graphRep == rep::ADJACENCY_LIST) { to_matrix(); } 
    auto g = m_matrix;
    int vertex = 0; //> Inicia a busca a partir do primeiro vértice (índice 0)
    std::vector<int> visited(m_vertices, 0); //> Vetor para marcar os vértices visitados durante a busca
    std::queue<int> q; //> Fila para a busca em largura (BFS)
    q.push(vertex); //> Adiciona o vértice inicial à fila
    visited[vertex] = 1; //> Marca o vértice inicial como visitado
        while (!q.empty()) {
            int current = q.front(); //> Obtém o vértice atual da frente da fila
            q.pop(); //> Remove o vértice atual da fila
    
            for (int i = 0; i < m_vertices; ++i) {
                if (g[current][i] != 0 && !visited[i]) { //> Verifica se há uma aresta entre o vértice atual e o vértice i, e se o vértice i ainda não foi visitado
                    visited[i] = 1; //> Marca o vértice i como visitado
                    q.push(i); //> Adiciona o vértice i à fila para continuar a busca
                }
            }
        }

        //> Verifica se todos os vértices foram visitados, o que indica que o grafo é conexo
        for (int i = 0; i < m_vertices; ++i) {
            if (!visited[i]) {
                return false; //> Se algum vértice não foi visitado, o grafo não é conexo
            }       
}
    return true; //> Se todos os vértices foram visitados, o grafo é conexo
}
template <typename T>
void Graph<T>::find_articulations(){
    if(is_targeted)return;

    this->to_list();

    int dfs_time{0};
    std::vector<int> dfn(m_vertices, 0);
    std::vector<int> parent(m_vertices, -1);
    std::vector<bool> is_articulation(m_vertices, false);
    std::vector<std::pair<int, int>> stack_edges;

    auto lowpt = [&](auto&& self, int u){
              dfn[u] = ++dfs_time;  
              int lowest_vertex{u};
              int children{0};

              for(auto& neighbor : m_list[u]){
                int v = get_vertex_index(neighbor, false);
                if(v == -1 || v == parent[u])continue;

                if(dfn[v] < dfn[u] && dfn[v]){
                    stack_edges.push_back({u, v});
                    if(dfn[v] < dfn[lowest_vertex]){
                        lowest_vertex = v;
                    }
                }
                else if(dfn[v] == 0){
                    children++;
                    parent[v] = u;
                    stack_edges.push_back({u, v});
                    
                    int child_lowest_vertex = self(self, v);

                    if(dfn[child_lowest_vertex] < dfn[lowest_vertex]){
                        lowest_vertex = child_lowest_vertex;
                    }
                    if((parent[u] == -1 && children > 1) ||(parent[u] != -1 && dfn[child_lowest_vertex] >= dfn[u]) ){
                        is_articulation[u] = true;

                        while (true) {
                            auto edge = stack_edges.back();
                            stack_edges.pop_back();
                            std::cout << "(" << get_vertex_label(edge.first) << ", " << get_vertex_label(edge.second) << ") ";
                            if (edge.first == u && edge.second == v) break;
                        }
                    }
                }
            }
        return lowest_vertex;
        };

    for(int i{0}; i < m_vertices; ++i){
        if(!dfn[i]){
            lowpt(i);

            if(!stack_edges.empty()){
                std::cout << "Bloco Biconexo: ";
                while (!stack_edges.empty()) {
                    auto edge = stack_edges.back();
                    stack_edges.pop_back();
                    std::cout << "(" << get_vertex_label(edge.first) << ", " << get_vertex_label(edge.second) << ") ";
                }
                std::cout << '\n';
            }
        }
    }
    std::cout << "Pontos de Articulacao: ";
    bool has_art = false;
    for (int i = 0; i < m_vertices; ++i) {
        if (is_articulation[i]) {
            std::cout << get_vertex_label(i) << " ";
            has_art = true;
        }
    }
    if (!has_art) std::cout << "Nenhum";
    std::cout << '\n';
}



template <typename T>
int Graph<T>::get_newest_vertex(int current) {
    for (int i{0}; i < m_vertices; ++i) {
        if (m_matrix[current][i] != 0) {
            return i; //> Retorna o índice do primeiro vértice adjacente encontrado, garantindo a consistência na atribuição de cores durante a verificação de bipartição
        }
    }
    return -1; //> Retorna -1 se não houver vértices adjacentes, o que pode indicar um vértice isolado
}

template <typename T>
bool Graph<T>::is_bipartite() {
    enum class Color { RED, BLUE, NONE };

    if (graphRep == rep::ADJACENCY_LIST) { to_matrix(); }

    auto g = m_matrix;
    std::vector<Color> colors(m_vertices, Color::NONE);
    std::stack<int> s;

    for (int start = 0; start < m_vertices; ++start) {
        if (colors[start] != Color::NONE) { continue; }

        colors[start] = Color::RED;
        s.push(start);

        while (not s.empty()) {
            int current = s.top();
            s.pop();

            for (int i = 0; i < m_vertices; ++i) {
                if (g[current][i] == 0) { continue; }

                if (colors[i] == Color::NONE) {
                    colors[i] = (colors[current] == Color::RED) ? Color::BLUE : Color::RED;
                    s.push(i);
                } else if (colors[i] == colors[current]) {
                    return false;
                }
            }
        }
    }

    return true;
}


template <typename T>
void Graph<T>::dfs_directed_classification(T start_vertex) {
    if (!is_targeted) {
        std::cout << "Aviso: A classificacao de arestas em Arvore, Retorno, Avanco e Cruzamento "
                  << "e uma propriedade tipicamente aplicada a Digrafos.\n";
    }

    int start_index = get_vertex_index(start_vertex, false);
    if (start_index == -1) {
        std::cerr << "Erro: Vertice inicial nao encontrado.\n";
        return;
    }

    this->to_list(); 

    enum class Color { WHITE, GRAY, BLACK };
    std::vector<Color> color(m_vertices, Color::WHITE);
    std::vector<int> discovery_time(m_vertices, 0); //> Profundidade de entrada (d)
    std::vector<int> finish_time(m_vertices, 0);    //> Profundidade de saída (f)
    std::vector<int> parent(m_vertices, -1);
    
    int time = 0; //> Relógio global da DFS

    //> Função recursiva auxiliar (lambda) para visitar os vértices
    auto dfs_visit = [&](auto&& self, int u) -> void {
        time++;
        discovery_time[u] = time;
        color[u] = Color::GRAY; //> Vértice descoberto, mas ainda visitando vizinhos

        for (const auto& neighbor : m_list[u]) {
            int v = get_vertex_index(neighbor, false);
            if (v == -1) continue;

            std::cout << "Aresta (" << get_vertex_label(u) << " -> " << get_vertex_label(v) << "): ";

            if (color[v] == Color::WHITE) {
                std::cout << "Arvore\n";
                parent[v] = u;
                self(self, v); 
            } 
            else if (color[v] == Color::GRAY) {
                std::cout << "Retorno\n";
            } 
            else if (color[v] == Color::BLACK) {
                if (discovery_time[u] < discovery_time[v]) {
                    std::cout << "Avanco\n";
                } else {
                    std::cout << "Cruzamento\n";
                }
            }
        }

        color[u] = Color::BLACK; //> Todos os vizinhos visitados, vértice finalizado
        time++;
        finish_time[u] = time;
    };

    std::cout << "Classificacao de Arestas:\n";
    
    //> Dispara a DFS a partir do vértice inicial solicitado
    dfs_visit(dfs_visit, start_index);

    std::cout << "\n--- Profundidade de Entrada (d) e Saida (f) ---\n";
    for (int i = 0; i < m_vertices; i++) {
        if (color[i] != Color::WHITE) { //> Imprime apenas os que foram alcançados
            std::cout << "Vertice [" << get_vertex_label(i) 
                      << "]: Entrada = " << discovery_time[i] 
                      << ", Saida = " << finish_time[i] << "\n";
        }
    }
}
#endif