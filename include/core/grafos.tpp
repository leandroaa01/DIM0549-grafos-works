
#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
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
            T origin_label = get_vertex_label(i);
            for (int j = 0; j < m_vertices; ++j) {
                if (m_matrix[i][j] == 0) { continue; }

                auto neighbor = std::find_if(m_vertex_index.begin(), m_vertex_index.end(), [j](const auto& pair) {
                    return pair.second == j;
                });
                if (neighbor == m_vertex_index.end()) { continue; }

                int edge_cost = m_weighted ? m_matrix[i][j] : 0;
                m_list[i].push_back(Edge<T>{origin_label, neighbor->first, edge_cost});
            }
        }
    }
    else{
        for (int e{0}; e < m_edges; ++e) {
            int origin{-1}, destiny{-1};
            for (int i{0}; i < m_vertices; ++i) {
                if (is_targeted) {
                    if (m_inc_matrix[i][e] == 1) origin = i;
                    else if (m_inc_matrix[i][e] == -1) destiny = i;
                } else {
                    if (m_inc_matrix[i][e] == 1) {
                        if (origin == -1) origin = i;
                        else destiny = i;
                    }
                }
            }

            if (origin != -1 && destiny != -1) {
                T origin_label = get_vertex_label(origin);
                T destiny_label = get_vertex_label(destiny);
                int edge_cost = (m_weighted && e < static_cast<int>(m_edge_costs.size())) ? m_edge_costs[e] : 0;

                m_list[origin].push_back(Edge<T>{origin_label, destiny_label, edge_cost});

                if (!is_targeted) {
                    m_list[destiny].push_back(Edge<T>{destiny_label, origin_label, edge_cost});
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
    m_matrix.resize(m_vertices, std::vector<int>(m_vertices, 0)); //> 0 = sem aresta

    if(graphRep == rep::ADJACENCY_LIST){
        //> Converte de lista de adjacências para matriz de adjacências
        for (int i = 0; i < m_vertices; ++i) {
            for (const auto& edge : m_list[i]) {
                auto neighbor_index = get_vertex_index(edge.destiny, false); //> Obtém o índice do vértice vizinho sem criar um novo vértice
                if (neighbor_index < 0) { continue; }

                int value = m_weighted ? edge.cost : 1;
                if (m_weighted && value < 1) {
                    std::cerr << "Error: edge cost must be >= 1 in weighted graphs." << std::endl;
                    continue;
                }

                m_matrix[i][neighbor_index] = value;
            }
        }
    }
    
    graphRep = rep::ADJACENCY_MATRIX; //> Define a flag para indicar que o grafo agora está usando matriz de adjacências
}
template <typename T>
void Graph<T>::to_incMat(){
    if(graphRep == rep::INCIDENCY_MATRIX){ return; }
    if(graphRep != rep::ADJACENCY_LIST)  {this->to_list();}
    
    m_inc_matrix.assign(m_vertices, std::vector<int>{});
    m_edge_costs.clear();

    std::vector<std::vector<bool>> visited(m_vertices, std::vector<bool>(m_vertices, false));

    for(int i{0}; i < m_vertices; ++i){
        for(const auto& edge : m_list[i]){
            int index = get_vertex_index(edge.destiny, false);
            if(index < 0) continue;
            if(!is_targeted && visited[index][i]) continue;  //> Evita contar a mesma aresta 2 vezes (não-direcionado)

            for(int r{0}; r < m_vertices; ++r){
                m_inc_matrix[r].push_back(0);
            }
            int col = static_cast<int>(m_edge_costs.size());
            m_inc_matrix[i][col] = 1;
            m_inc_matrix[index][col] = is_targeted ? -1 : 1; //> Dígrafo: destino = -1
            m_edge_costs.push_back(m_weighted ? edge.cost : 0);

            visited[i][index] = true;
        }
    }

    m_edges = static_cast<int>(m_edge_costs.size());
    graphRep = rep::INCIDENCY_MATRIX;
}
template <typename T>
void Graph<T>::add(T vertex) {
    if (get_vertex_index(vertex, false) != -1) return; //> Caso o vértice já exista, não adiciona nada

    m_vertices++; //> Aumenta em 1 a quantidade de vértices

    for (auto& row : m_matrix) { //> Adiciona 1 linha a mais na matriz de adjacência
        row.push_back(0); 
    }
    m_matrix.push_back(std::vector<int>(m_vertices, 0)); //> Adiciona 1 coluna a mais na matriz de adjacência

    m_list.push_back(std::list<Edge<T>>()); //> Adiciona a lista do vértice "vertex" na lista de adjacência

    if (!m_inc_matrix.empty()) {        //> Caso a matriz de incidência já tenha sido criada:
        int current_edges = m_inc_matrix[0].size();     //> Adiciona uma linha a mais para representar o novo vértice 
        m_inc_matrix.push_back(std::vector<int>(current_edges, 0)); 
    }

    m_vertex_index[vertex] = m_vertices - 1; //> O íncide do novo vértice é o último
}

template <typename T>
void Graph<T>::add(T origin, T destiny) {
    add(origin, destiny, 0);
}

template <typename T>
void Graph<T>::add(T origin, T destiny, int cost) {
    if ( not m_weighted)  { cost = 0; } //> Se o grafo não for ponderado, ignora o custo recebido (cost=0)

    int origin_index = get_vertex_index(origin);
    int destiny_index = get_vertex_index(destiny);

    if (origin_index < 0 or destiny_index < 0) {
        std::cerr << "Error: vertex index out of bounds." << std::endl;
        return;
    }

    bool edge_exists = false;

    if (graphRep == rep::ADJACENCY_LIST) {
        auto it = std::find_if(m_list[origin_index].begin(), m_list[origin_index].end(), [&](const Edge<T>& e){
            return e.destiny == destiny;
        });
        edge_exists = (it != m_list[origin_index].end());

        if (edge_exists) {
            if (m_weighted) it->cost = cost;
            if (!is_targeted && origin != destiny) {
                auto it2 = std::find_if(m_list[destiny_index].begin(), m_list[destiny_index].end(), [&](const Edge<T>& e){
                    return e.destiny == origin;
                });
                if (it2 != m_list[destiny_index].end() && m_weighted) it2->cost = cost;
            }
            return;
        }
    } else if(graphRep == rep::ADJACENCY_MATRIX) {
        edge_exists = (m_matrix[origin_index][destiny_index] != 0);
        if (edge_exists) {
            m_matrix[origin_index][destiny_index] = m_weighted ? cost : 1;
            if (!is_targeted) {
                m_matrix[destiny_index][origin_index] = m_weighted ? cost : 1;
            }
            return;
        }
    }
    else {
        for(int j{0}; j < m_edges; ++j){
            if (is_targeted) {
                if(m_inc_matrix[origin_index][j] == 1 && m_inc_matrix[destiny_index][j] == -1){
                    edge_exists = true;
                    if (m_weighted && j < static_cast<int>(m_edge_costs.size())) m_edge_costs[j] = cost;
                    break;
                }
            } else {
                if(m_inc_matrix[origin_index][j] == 1 && m_inc_matrix[destiny_index][j] == 1){
                    edge_exists = true;
                    if (m_weighted && j < static_cast<int>(m_edge_costs.size())) m_edge_costs[j] = cost;
                    break;
                }
            }
        }
        if (edge_exists) { return; }
    }

    m_edges++;

    if (graphRep == rep::ADJACENCY_LIST) {
        m_list[origin_index].push_back(Edge<T>{origin, destiny, cost});
        if (!is_targeted && origin != destiny) {
            m_list[destiny_index].push_back(Edge<T>{destiny, origin, cost});
        }
    } else if(graphRep == rep::ADJACENCY_MATRIX) {
        m_matrix[origin_index][destiny_index] = m_weighted ? cost : 1;
        if (!is_targeted) {
            m_matrix[destiny_index][origin_index] = m_weighted ? cost : 1;
        }
    } else {
        if (m_inc_matrix.empty()) {
            m_inc_matrix.assign(m_vertices, std::vector<int>{});
        }
        for(int i{0}; i < m_vertices; ++i){
            m_inc_matrix[i].push_back(0);
        }
        m_inc_matrix[origin_index][m_edges - 1] = 1;
        m_inc_matrix[destiny_index][m_edges - 1]  = is_targeted ? -1 : 1;
        if (static_cast<int>(m_edge_costs.size()) < m_edges) m_edge_costs.push_back(m_weighted ? cost : 0);
    }
}


template <typename T>
void Graph<T>::remove(T vertex){
    int vertex_index = get_vertex_index(vertex, false); 
    if(vertex_index < 0){       //> Caso não encontre o vértice, retorna
        std::cerr << "Erro: Vértice não encontrado." << std::endl;
        return;
    }

    T last_vertex = get_vertex_label(m_vertices - 1); //> Último vértice do grafo

    int edges_removed = 0; //> Contador de arestas removidas
    
    if (graphRep == rep::ADJACENCY_LIST && !m_list.empty()) {
        if (!is_targeted) {
            edges_removed = static_cast<int>(m_list[vertex_index].size());
        } else {
            for(auto& adj_list : m_list){
                edges_removed += static_cast<int>(std::count_if(adj_list.begin(), adj_list.end(), [&](const Edge<T>& e){
                    return e.destiny == vertex;
                }));
            }
            edges_removed += static_cast<int>(m_list[vertex_index].size());

            bool has_self_loop = std::any_of(m_list[vertex_index].begin(), m_list[vertex_index].end(), [&](const Edge<T>& e){
                return e.destiny == vertex;
            });
            if (has_self_loop) edges_removed--;
        }
    }
    else if (graphRep == rep::ADJACENCY_MATRIX && !m_matrix.empty()) { //> Caso seja uma matriz de adjacência não vazia:
        if (!is_targeted) {
            for (int j = 0; j < m_vertices; ++j) {
                if (m_matrix[vertex_index][j] != 0) edges_removed++;
            }
        } else {
            for (int i = 0; i < m_vertices; ++i) {
                if (m_matrix[i][vertex_index] != 0) edges_removed++;
            }
            for (int j = 0; j < m_vertices; ++j) {
                if (m_matrix[vertex_index][j] != 0) edges_removed++;
            }
            if(m_matrix[vertex_index][vertex_index] != 0) edges_removed--;
        }
    }
    else if (graphRep == rep::INCIDENCY_MATRIX && !m_inc_matrix.empty()) {
        for (int e{0}; e < m_edges; ++e) {
            if (m_inc_matrix[vertex_index][e] != 0) edges_removed++;
        }
    }
    
    if (graphRep != rep::INCIDENCY_MATRIX) {    //> Reduz o total de arestas (Se for matriz de incidência, o loop final já faz isso)
        m_edges -= edges_removed;              
    }

    if(vertex_index != m_vertices - 1){ //> Caso o índice do vertex não seja o último, realiza um swap em lista e matriz de adjacência, trocando o
                                        //> vértice original (com suas arestas) com o último vértice (e suas arestas).
        m_vertex_index[last_vertex] = vertex_index;  
        
        if (!m_matrix.empty()) {    //> Troca em uma matriz de adjecência
            std::swap(m_matrix[vertex_index], m_matrix[m_vertices - 1]);
            for(int i{0}; i < m_vertices; ++i){
                std::swap(m_matrix[i][vertex_index], m_matrix[i][m_vertices - 1]);
            }
        }
        
        if (!m_list.empty()) { //> Troca em uma lista de adjacência
            std::swap(m_list[vertex_index], m_list[m_vertices - 1]);
        }
    }

    if (!m_matrix.empty()) { //> Após a troca, remove a última linha e coluna da matriz de adjacência (elas que estão representando o vértice removido agora)
        m_matrix.pop_back();
        for(auto& r : m_matrix){
            r.pop_back();
        }
    }

    if (!m_list.empty()) { //> Após a troca, remove a última lista e todas as aparições do vértice removido das outras listas
        m_list.pop_back();
        for(auto& adj_list : m_list){
            adj_list.remove_if([&](const Edge<T>& e){ return e.destiny == vertex; });
        }
    }
    if(!m_inc_matrix.empty()){  //> Caso a matriz de incidência exista (já tiver sido criada)
        int num_cols = m_inc_matrix[vertex_index].size(); //> Variável que guarda a quantidade de colunas da matriz

        for(int col{num_cols-1}; col >= 0; --col){
            if(m_inc_matrix[vertex_index][col] != 0){       //> Caso tenha uma conexão envolvendo o vértice que será removido:
                int last_col = m_inc_matrix[0].size() - 1;  //> Variável que guarda a posição da última coluna da matriz
                if(col != last_col){                        //> Caso a conexão encontrada não seja a última na matriz:
                    for(int i{0}; i < m_vertices; ++i){
                        std::swap(m_inc_matrix[i][col], m_inc_matrix[i][last_col]); //> Troca a coluna da conexão pela última coluna da matriz
                    }
                    if (!m_edge_costs.empty() && col < static_cast<int>(m_edge_costs.size()) && last_col < static_cast<int>(m_edge_costs.size())) {
                        std::swap(m_edge_costs[col], m_edge_costs[last_col]);
                    }
                }
                for(int i{0}; i < m_vertices; ++i){         //> Remove a última coluna da matriz
                    m_inc_matrix[i].pop_back();
                }
                if (!m_edge_costs.empty()) {
                    m_edge_costs.pop_back();
                }
                if (graphRep == rep::INCIDENCY_MATRIX) m_edges--; //> Caso seja uma matriz de incidência, decrementa a quantidade de arestas
            }
        }
        if(vertex_index != m_vertices - 1){ //> Caso o vértice não seja a última linha, troca a posição das duas linhas
            std::swap(m_inc_matrix[vertex_index], m_inc_matrix[m_vertices - 1]);
        }
        m_inc_matrix.pop_back();    //> Remove a última linha da matriz (garantido que seja a linha do vértice)
    }

    m_vertex_index.erase(vertex); //> Remove o vértice do mapa dos índices do grafo
    m_vertices--;                 //> Decrementa a quantidade de vértices no grafo
}


template <typename T>
void Graph<T>::remove(T origin, T destiny) {
    int origin_index = get_vertex_index(origin, false); //> Obtém o índice do vértice de origem sem criar um novo vértice
    int destiny_index = get_vertex_index(destiny, false); //> Obtém o índice do vértice de destino sem criar um novo vértice

    if (origin_index < 0 or destiny_index < 0) {
        std::cerr << "Error: vertex not found." << std::endl;
        return;
    }

    bool removed = false;

    if (graphRep == rep::ADJACENCY_LIST) {
        auto before = m_list[origin_index].size();
        m_list[origin_index].remove_if([&](const Edge<T>& e){ return e.destiny == destiny; });
        removed = (m_list[origin_index].size() != before);
        if(!is_targeted && origin != destiny){
            m_list[destiny_index].remove_if([&](const Edge<T>& e){ return e.destiny == origin; });
        }
    } else if(graphRep == rep::ADJACENCY_MATRIX){
        removed = (m_matrix[origin_index][destiny_index] != 0);
        m_matrix[origin_index][destiny_index] = 0;
        if(!is_targeted){
            m_matrix[destiny_index][origin_index] = 0;
        }
    }
    else{
        for(int e{0}; e < m_edges; ++e){
            bool match = false;
            if (is_targeted) {
                match = (m_inc_matrix[origin_index][e] == 1 && m_inc_matrix[destiny_index][e] == -1);
            } else {
                match = (m_inc_matrix[origin_index][e] == 1 && m_inc_matrix[destiny_index][e] == 1);
            }
            if(match){
                int last = m_edges - 1;
                if (e != last) {
                    for(int i{0}; i < m_vertices; ++i){
                        std::swap(m_inc_matrix[i][e], m_inc_matrix[i][last]);
                    }
                    if (e < static_cast<int>(m_edge_costs.size()) && last < static_cast<int>(m_edge_costs.size())) {
                        std::swap(m_edge_costs[e], m_edge_costs[last]);
                    }
                }
                for(int i{0}; i < m_vertices; ++i){
                    m_inc_matrix[i].pop_back();
                }
                if (!m_edge_costs.empty()) m_edge_costs.pop_back();
                removed = true;
                break;
            }
        }
    }

    if (!removed) {
        std::cerr << "Error: edge not found." << std::endl;
        return;
    }

    m_edges--; //> Decrementa o contador de arestas do grafo
}

template <typename T>
void Graph<T>::print() const {
    if (graphRep == rep::ADJACENCY_LIST) {  //> Caso seja uma lista de Adjacência
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
            for (const auto& edge : m_list[i]) {
                if (m_weighted) {
                    std::cout << edge.destiny << "(" << edge.cost << ")" << ", ";
                } else {
                    std::cout << edge.destiny << ", ";
                }
            }
            std::cout << "}";

            std::cout << '\n';
        }
        return;
    } else if (graphRep == rep::INCIDENCY_MATRIX) { //> Caso seja uma matriz de Incidência
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
    else if(graphRep == rep::ADJACENCY_MATRIX){ //> Caso seja uma matriz de adjacência
    std::cout << "   ";
    for (int i = 0; i < m_vertices; ++i) {
        auto valor = std::find_if(m_vertex_index.begin(), m_vertex_index.end(), [i](const auto& pair) {
            return pair.second == i;
        });

        if (valor != m_vertex_index.end()) {
            std::cout << valor->first << " ";
        } else {
            std::cout << "? "; //> Caso não encontre o valor.
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
            std::cout << "?| "; //> Caso não encontre o valor.
        }

        for (int j = 0; j < m_vertices; ++j) {
            std::cout << m_matrix[i][j] << " ";
        }
        std::cout << "| \n";
    }
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
        std::cout << "Visitando o vértice: " << get_vertex_label(v) << '\n';

        for(auto& edge : m_list[v]){                   //> Visita os vizinhos do vértice v
            int w = get_vertex_index(edge.destiny, false);
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
    visited[index] = true;                          //> Marca o vértice como visitado
    visit_order.push_back(get_vertex_label(index)); //> Adiciona o vértice no vector de ordem

    std::cout << "Visitando o vértice: " << get_vertex_label(index) << '\n';

    for(auto& edge : m_list[index]){
        int w = get_vertex_index(edge.destiny, false);
        if(w != -1 && !visited[w]){                         //> Se o vizinho não for visitado
            dfs_rec(w, visit_order, visited);   //> Chamada recursiva para o vizinho
        }
    }
}
template <typename T>
std::vector<T> Graph<T>::dfs(T start_vertex){   
    std::vector<T> visit_order;                             //> Vetor que guarda a ordem em que cada vértice foi visitado
    int start_index = get_vertex_index(start_vertex, false);
    if(start_index == -1)return visit_order;                //> Caso o vértice não seja encontrado, retorna um vetor vazio

    this->to_list();    //> Otimização: realizar a busca em uma lista de adjacência é mais rápido que as matrizes de adjacência e incidência  

    std::vector<bool> visited(m_vertices, false); //> Vetor que guarda se cada vértice foi visitado
    
    dfs_rec(start_index, visit_order, visited); //> Chamada da função recursiva começando do vértice passado.

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
    if(is_targeted) {
        std::cerr << "Erro: Grafos direcionados não possuem articulação.\n";
        return; //> Não faz sentido encontrar articulações em Dígrafos neste contexto
    }
    this->to_list(); //> Otimização: realizar a busca em uma lista de adjacência é mais rápido que nas matrizes de adjacência e incidência

    int dfs_time{0};                                      //> Contador da ordem de acesso dos vértices
    std::vector<int> dfn(m_vertices, 0);                  //> Vetor que guarda a ordem de descoberta de cada vértice
    std::vector<int> parent(m_vertices, -1);              //> Vetor que guarda os pais de cada vértice na árvore da DFS
    std::vector<bool> is_articulation(m_vertices, false); //> Vetor booleano que marca se um vértice é uma articulação
    std::vector<std::pair<int, int>> stack_edges;         //> Pilha que guarda as arestas visitadas para formar os blocos biconexos

    auto lowpt = [&](auto&& self, int u) -> int {   //> Função Lambda que executa a DFS e calcula o lowpt
        dfn[u] = ++dfs_time;                //> Atribui o tempo de descoberta ao vértice u
        int lowest_vertex{u};               //> O menor vértice alcançável a partir de u (inicialmente é ele mesmo)
        int children{0};                    //> Conta quantos filhos u tem na árvore da DFS

        for(auto& edge : m_list[u]){
            int v = get_vertex_index(edge.destiny, false);  //> Obtém o índice do vizinho v
            if(v == -1 || v == parent[u]) continue;     //> Ignora se for inválido ou se for a aresta que volta diretamente para o pai

            //> CASO 1: v já foi visitado e tem um tempo menor que u (Encontramos uma Aresta de Retorno)
            if(dfn[v] < dfn[u] && dfn[v] != 0){      
                stack_edges.push_back({u, v});          //> Adiciona a aresta na pilha
                if(dfn[v] < dfn[lowest_vertex]){        //> Atualiza o lowest_vertex se o dfn de v for menor que o atual
                    lowest_vertex = v;
                }
            }
            //> CASO 2: v ainda não foi visitado (Encontramos uma Aresta de Árvore)
            else if(dfn[v] == 0){ 
                children++;                    //> v se torna um filho oficial de u na árvore
                parent[v] = u;                 //> u é definido como pai de v
                stack_edges.push_back({u, v}); //> Adiciona a aresta na pilha
                
                int child_lowest_vertex = self(self, v); //> Chamada recursiva da DFS para explorar o filho v

                //> Atualiza o lowest_vertex de u com base no retorno do filho v
                if(dfn[child_lowest_vertex] < dfn[lowest_vertex]){
                    lowest_vertex = child_lowest_vertex;
                }

                //> VERIFICAÇÃO DE ARTICULAÇÃO:
                //> 1. u é raiz e tem mais de 1 filho
                //> 2. u não é raiz e o filho v não consegue alcançar um ancestral de u (dfn[child_lowest_vertex] >= dfn[u])
                if((parent[u] == -1 && children > 1) || (parent[u] != -1 && dfn[child_lowest_vertex] >= dfn[u])){
                    is_articulation[u] = true; //> Marca u como ponto de articulação.

                    std::cout << "Bloco Biconexo: \n";
                    //> Desempilha as arestas até encontrar a aresta (u, v) que causou a articulação
                    while (true) {
                        auto edge = stack_edges.back();
                        stack_edges.pop_back();
                        std::cout << "(" << get_vertex_label(edge.first) << ", " << get_vertex_label(edge.second) << ")\n";
                        if (edge.first == u && edge.second == v) break; //> Para ao remover a raiz deste bloco
                    }
                    std::cout << '\n';
                }
            }
        }
        return lowest_vertex; //> Retorna o menor vértice que a subárvore de u consegue alcançar
    };

    //> Garante que todos os componentes conexos sejam visitados
    for(int i{0}; i < m_vertices; ++i){
        if(!dfn[i]){ //> Se o vértice ainda não foi descoberto, inicia uma nova DFS a partir dele
            lowpt(lowpt, i);

            //> Ao finalizar a DFS de uma componente, pode sobrar um bloco biconexo na raiz (caso ela não seja articulação)
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
    
    //> Print dos pontos de articulação encontrados
    std::cout << "Pontos de Articulacao: "; 
    bool has_art = false;                   
    for (int i = 0; i < m_vertices; ++i) {
        if (is_articulation[i]) {
            std::cout << get_vertex_label(i) << " ";
            has_art = true; 
        }
    }
    if (!has_art) std::cout << "Nenhum"; //> Caso o grafo não possua nenhuma articulação
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
    enum class Color { RED, BLUE, NONE }; //> Enum que classifica a cor de cada vértice

    if (graphRep != rep::ADJACENCY_MATRIX) { to_matrix(); } //> A implementação é baseada em matriz de adjacência, então
                                                            //> garante que a matriz seja representada de acordo

    auto g = m_matrix;            //> Matriz temporária que representa o grafo
    std::vector<Color> colors(m_vertices, Color::NONE);     //> Vetor que guarda a cor de cada vértice do grafo
    std::stack<int> s;                                      //> Pilha (LIFO) para gerenciar a busca dos vértices

    for (int start = 0; start < m_vertices; ++start) {
        if (colors[start] != Color::NONE) { continue; } //> Se possuir cor, ignora

        colors[start] = Color::RED; //> Define a cor inicial como vermelho
        s.push(start);            //> Adiciona o vértice na pilha

        while (not s.empty()) {
            int current = s.top();  //> Retira o vértice do topo da pilha
            s.pop();

            for (int i = 0; i < m_vertices; ++i) {
                if (g[current][i] == 0) { continue; }   //> Caso o vértice não possua aresta com o vértice na posição i, ignora

                if (colors[i] == Color::NONE) {         //> Caso o vértice de posição 'i' não tenha cor, vai colorir com base em 's'
                    colors[i] = (colors[current] == Color::RED) ? Color::BLUE : Color::RED;
                    s.push(i);                      
                } else if (colors[i] == colors[current]) {
                    return false;                       //> Caso os dois vértices tenham a mesma cor, ele não é bipartido
                }
            }
        }
    }

    return true; //> Se passou por todos os vértices sem dar erro, é bipartido
}


template <typename T>
void Graph<T>::dfs_directed_classification(T start_vertex) {
    if (!is_targeted){ //> Apenas classificar cada um em Grafos não direcionados.
        std::cerr << "Erro: Não tem como classificar em um grafo não direcionado\n";
        return;  
    } 

    int start_index = get_vertex_index(start_vertex, false);
    if (start_index == -1) {                                        //> Caso o vértice não seja encontrado, acaba a função
        std::cerr << "Erro: Vertice inicial nao encontrado.\n";
        return;
    }

    this->to_list(); //> Otimização: realizar a busca em uma lista de adjacência é mais rápido que as matrizes de adjacência e incidência 

    enum class Color { WHITE, GRAY, BLACK };
    std::vector<Color> color(m_vertices, Color::WHITE);
    std::vector<int> discovery_time(m_vertices, 0); //> Profundidade de entrada (d)
    std::vector<int> finish_time(m_vertices, 0);    //> Profundidade de saída (f)
    std::vector<int> parent(m_vertices, -1);        //> Vetor que guarda os pais de cada vértice
    
    int time = 0; //> Relógio global da DFS

    //> Função recursiva auxiliar (lambda) para visitar os vértices
    auto dfs_visit = [&](auto&& self, int u) -> void {
        time++;
        discovery_time[u] = time;
        color[u] = Color::GRAY; //> Vértice descoberto, mas ainda visitando vizinhos

        for (const auto& edge : m_list[u]) {
            int v = get_vertex_index(edge.destiny, false);
            if (v == -1) continue;

            std::cout << "Aresta (" << get_vertex_label(u) << " -> " << get_vertex_label(v) << "): ";

            if (color[v] == Color::WHITE) { //> Caso seja uma aresta de Árvore:
                std::cout << "Arvore\n";
                parent[v] = u;
                self(self, v); 
            } 
            else if (color[v] == Color::GRAY) { //> Caso seja uma aresta de Retorno:
                std::cout << "Retorno\n";
            } 
            else if (color[v] == Color::BLACK) {    
                if (discovery_time[u] < discovery_time[v]) { //> Caso seja uma aresta de avanço
                    std::cout << "Avanco\n";
                } else {                                     //> Caso seja uma areta de cruzamento
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
template <typename T>
std::string Graph<T>::getRepresentation(){
    switch(graphRep){   
        case Representation::ADJACENCY_MATRIX:
            return "Matriz de Adjacência"; //> Retorna a representação em formato de string
        break;
        case Representation::ADJACENCY_LIST:
            return "Lista de Adjacência";  //> Retorna a representação em formato de string
        break;
        case Representation::INCIDENCY_MATRIX:
            return "Matriz de Incidência"; //> Retorna a representação em formato de string
        break;
        default:
            std::cerr << "Erro: Representação inválida!" << std::endl; //> Caso a representação esteja inválida 
                                                                       //> (impossível, apenas está aqui para facilitar alterações futuras) 
            return "";
    }
}
template<typename T>
bool Graph<T>::has_cycle(){
    //> Usa matriz de adjacência como base
    if (graphRep != rep::ADJACENCY_MATRIX) { to_matrix(); }

    if (is_targeted) {
        enum class Color { WHITE, GRAY, BLACK }; //> Enum para classificar os vértices durante a DFS: WHITE (não visitado), GRAY (visitando), BLACK (finalizado)
        std::vector<Color> color(m_vertices, Color::WHITE);

        //função recursiva auxiliar (lambda) para realizar a DFS e detectar ciclos
        std::function<bool(int)> dfs = [&](int u) -> bool {
            color[u] = Color::GRAY; //> Marca o vértice como visitando
            for (int v = 0; v < m_vertices; ++v) {
                if (m_matrix[u][v] == 0) continue; //> Ignora se não houver aresta entre u e v
                if (color[v] == Color::GRAY) return true;      //> aresta de retorno
                if (color[v] == Color::WHITE && dfs(v)) return true; //> aresta de árvore 
            }
            color[u] = Color::BLACK; //> Marca o vértice como finalizado
            return false; //> Não encontrou ciclo a partir deste vértice
        };

        //> Inicia a DFS para cada vértice não visitado
        for (int i = 0; i < m_vertices; ++i) {
            if (color[i] == Color::WHITE && dfs(i)) return true;
        }
        return false; //> Se passou por todos os vértices sem encontrar ciclo, retorna falso
    }

    //> Não direcionado: BFS/DFS com controle de pai
    std::vector<int> visited(m_vertices, 0); //> Vetor para marcar os vértices visitados durante a busca
    std::vector<int> parent(m_vertices, -1);//> Vetor para guardar o pai de cada vértice na busca
    std::queue<int> q;

    for (int start = 0; start < m_vertices; ++start) {
        if (visited[start]) continue; //> Se já foi visitado, ignora

        visited[start] = 1; //> Marca o vértice inicial como visitado
        parent[start] = -1; //> O vértice inicial não tem pai
        q.push(start); //> Começa a busca a partir do vértice inicial

        while (not q.empty()) { //> Enquanto houver vértices na fila
            int u = q.front(); //> Obtém o vértice atual da frente da fila
            q.pop();//> Remove o vértice atual da fila

            for (int v = 0; v < m_vertices; ++v) { //> Para cada vértice v, verifica se é adjacente a u
                if (m_matrix[u][v] == 0) continue; //> Ignora se não houver aresta entre u e v

                if (not visited[v]) { //> Se v ainda não foi visitado, marca como visitado, define u como pai de v e adiciona v à fila
                    visited[v] = 1;
                    parent[v] = u;
                    q.push(v);
                } else if (parent[u] != v) { //> Se v já foi visitado e não é o pai de u, então encontramos um ciclo
                    return true;
                }
            }
        }
    }

    return false;
}
template <typename T>
Matriz<int> Graph<T>::kuskal(){
    if (not m_weighted) { //> O algoritmo de Kruskal é aplicável apenas a grafos ponderados, então verifica se o grafo é ponderado
        std::cerr << "Erro: O algoritmo de Kruskal só é aplicável a grafos ponderados.\n";
        return Matriz<int>{};
    }

    if (is_targeted) { //> O algoritmo de Kruskal é aplicável apenas a grafos não direcionados, então verifica se o grafo é direcionado
        std::cerr << "Erro: O algoritmo de Kruskal (MST) só é aplicável a grafos não direcionados.\n";
        return Matriz<int>{};
    }

    //> Garante matriz de adjacência disponível
    if (graphRep != rep::ADJACENCY_MATRIX) { to_matrix(); }
    //
    std::vector<std::tuple<int, int, int>> edges; //> (origem, destino, custo)
    //> preenche o vetor de arestas a partir da matriz de adjacência, garantindo que cada aresta seja considerada apenas uma vez (i, j) e (j, i) são a mesma aresta em um grafo não direcionado
    for (int i = 0; i < m_vertices; ++i) {
        for (int j = i + 1; j < m_vertices; ++j) {
            if (m_matrix[i][j] != 0) {
                edges.emplace_back(i, j, m_matrix[i][j]);
            }
        }
    }
    //> Ordena as arestas pelo custo em ordem crescente
    std::sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
        return std::get<2>(a) < std::get<2>(b);
    });
    //> Cria um novo grafo para armazenar a árvore geradora mínima (MST)
    Graph<T> mst(m_vertices, false, true);
    mst.m_vertex_index = m_vertex_index; //> Garante que os rótulos dos vértices sejam os mesmos no MST

    int i {0}; //> Contador de arestas adicionadas ao MST
    for (const auto& [u, v, cost] : edges) {
        mst.m_matrix[u][v] = cost; 
        mst.m_matrix[v][u] = cost; 

        if (mst.has_cycle()) { //> Verifica se a adição da nova aresta criou um ciclo no MST
            mst.m_matrix[u][v] = 0;
            mst.m_matrix[v][u] = 0;
            continue;
        }
        //> Se não criou ciclo, mantém a aresta no MST e incrementa o contador
        i++;
        if (i == m_vertices - 1) break; //> O MST de um grafo com n vértices tem exatamente n-1 arestas, então podemos parar quando atingirmos esse número
    }

    mst.print(); //> Imprime a matriz de adjacência do MST resultante
    return mst.m_matrix;
}

#endif