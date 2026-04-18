
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
void Graph<T>::add(T vertex) {
    if (get_vertex_index(vertex, false) != -1) return; //> Caso o vértice já exista, não adiciona nada

    m_vertices++; //> Aumenta em 1 a quantidade de vértices

    for (auto& row : m_matrix) { //> Adiciona 1 linha a mais na matriz de adjacência
        row.push_back(0); 
    }
    m_matrix.push_back(std::vector<int>(m_vertices, 0)); //> Adiciona 1 coluna a mais na matriz de adjacência

    m_list.push_back(std::list<T>()); //> Adiciona a lista do vértice "vertex" na lista de adjacência

    if (!m_inc_matrix.empty()) {        //> Caso a matriz de incidência já tenha sido criada:
        int current_edges = m_inc_matrix[0].size();     //> Adiciona uma linha a mais para representar o novo vértice 
        m_inc_matrix.push_back(std::vector<int>(current_edges, 0)); 
    }

    m_vertex_index[vertex] = m_vertices - 1; //> O íncide do novo vértice é o último
}

template <typename T>
void Graph<T>::add(T origin, T destiny) {
    int origin_index = get_vertex_index(origin); //> Obtém o índice do vértice de origem, criando-o se necessário
    int destiny_index = get_vertex_index(destiny); //> Obtém o índice do vértice de destino, criando-o se necessário

     if (origin_index < 0 or destiny_index < 0) {   //> Caso não ache algum dos vértices, não faz nada
        std::cerr << "Error: vertex index out of bounds." << std::endl;
        return;
    }
    bool edge_exists = false;       //> Flag que diz se a aresta existe no grafo
    if (graphRep == rep::ADJACENCY_LIST) {  //> Caso seja representado por lista de adjacência, verifica se ele está na
        edge_exists = std::find(m_list[origin_index].begin(), m_list[origin_index].end(), destiny) != m_list[origin_index].end();
    } else if(graphRep == rep::ADJACENCY_MATRIX) { //> Caso seja uma matriz de adjecência, verifica se a posiçao [o][d] possui valor igual a 1
        if (m_matrix[origin_index][destiny_index] == 1) edge_exists = true;
    }
    else{
        for(int j{0}; j < m_edges; ++j){ //> Verifica se os dois vértices de origem e destino possuem uma conexão E_j
            if(m_inc_matrix[origin_index][j] != 0 && m_inc_matrix[destiny_index][j] != 0){
                edge_exists = true;
                break;
            }
        }
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
void Graph<T>::remove(T vertex){
    int vertex_index = get_vertex_index(vertex, false); 
    if(vertex_index < 0){       //> Caso não encontre o vértice, retorna
        std::cerr << "Erro: Vértice não encontrado." << std::endl;
        return;
    }

    T last_vertex = get_vertex_label(m_vertices - 1); //> Último vértice do grafo

    int edges_removed = 0; //> Contador de arestas removidas
    
    if (graphRep == rep::ADJACENCY_LIST && !m_list.empty()) { //> Caso seja uma lista de adjacência não vazia:
        for(auto& adj_list : m_list){
            edges_removed += std::count(adj_list.begin(), adj_list.end(), vertex); //> Soma a quantidade de arestas removidas
        }
        if (is_targeted) { //> caso seja direcionada:
            edges_removed += m_list[vertex_index].size(); //> Adiciona o resto das arestas faltantes
            if(std::find(m_list[vertex_index].begin(), m_list[vertex_index].end(), vertex) != m_list[vertex_index].end()){
                edges_removed--;    //> Caso não encontre a lista de adjacência do vértice, decrementa (não contar arestas de origem e destino iguais)
            }
        }
        
    } 
    else if (graphRep == rep::ADJACENCY_MATRIX && !m_matrix.empty()) { //> Caso seja uma matriz de adjacência não vazia:
        for (int i = 0; i < m_vertices; ++i) { 
            if (m_matrix[i][vertex_index] != 0) edges_removed++; //> Adiciona a quantidade de 1 (representa uma ligação entre dois vértices) na quantidade de 
                                                                 //> arestas removidas
        }
        if (is_targeted) {                                       //> Caso seja direcionado:
            for (int j = 0; j < m_vertices; ++j) {
                if (m_matrix[vertex_index][j] != 0) edges_removed++; //> Adiciona o resto das arestas faltantes
            }
            if(m_matrix[vertex_index][vertex_index] != 0)edges_removed--; //> Caso encontre uma aresta que começa e termina em vertex, decrementa.
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
            adj_list.remove(vertex);
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
                }
                for(int i{0}; i < m_vertices; ++i){         //> Remove a última coluna da matriz
                    m_inc_matrix[i].pop_back();
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
            for (const auto& neighbor : m_list[i]) {
                std::cout << neighbor << ", ";
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
    visited[index] = true;                          //> Marca o vértice como visitado
    visit_order.push_back(get_vertex_label(index)); //> Adiciona o vértice no vector de ordem

    for(auto& neightbor : m_list[index]){
        int w = get_vertex_index(neightbor, false);
        if(w != -1 && !visited[w]){                         //> Se o vizinho não for visitado
            dfs_rec(w, visit_order, visited);   //> Chamada recursiva para o vizinho
        }
    }
}
template <typename T>
std::vector<T> Graph<T>::dfs(T start_vertex){   
    std::vector<T> visit_order;                             //> Vetor que guarda a ordem em que cada vértice foi visitado
    int start_index = get_vertex_index(start_vertex);
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
    if(is_targeted) return; //> Não faz sentido encontrar articulações em Dígrafos neste contexto

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

        for(auto& neighbor : m_list[u]){
            int v = get_vertex_index(neighbor, false);  //> Obtém o índice do vizinho v
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

                    std::cout << "Bloco Biconexo: ";
                    //> Desempilha as arestas até encontrar a aresta (u, v) que causou a articulação
                    while (true) {
                        auto edge = stack_edges.back();
                        stack_edges.pop_back();
                        std::cout << "(" << get_vertex_label(edge.first) << ", " << get_vertex_label(edge.second) << ") ";
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
    if (!is_targeted) return; //> Apenas classificar cada um em Grafos não direcionados. 

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

        for (const auto& neighbor : m_list[u]) {
            int v = get_vertex_index(neighbor, false);
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
#endif