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
void Graph<T>::to_list() {
    
    if (use_list) { return; } //> Se o grafo já estiver usando lista de adjacências, não é necessário converter

    m_list.clear(); //> Limpa a lista de adjacências antes de preenchê-la
    m_list.resize(m_vertices); //> Redimensiona a lista de adjacências para o número de vértices

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

    use_list = true; //> Define a flag para indicar que o grafo agora está usando lista de adjacências
}

template <typename T>
void Graph<T>::to_matrix() {
    if (!use_list) {return;} //> Se o grafo já estiver usando matriz de adjacências, não é necessário converter

    m_matrix.clear(); //> Limpa a matriz de adjacências antes de preenchê-la
    m_matrix.resize(m_vertices, std::vector<int>(m_vertices, 0)); //> Redimensiona a matriz de adjacências para o número de vértices, inicializando com zeros

    for (int i = 0; i < m_vertices; ++i) {
        for (const auto& neighbor : m_list[i]) {
            auto neighbor_index = get_vertex_index(neighbor, false); //> Obtém o índice do vértice vizinho sem criar um novo vértice
            if (neighbor_index >= 0) {
                m_matrix[i][neighbor_index] = 1;
            }
        }
    }

    use_list = false; //> Define a flag para indicar que o grafo agora está usando matriz de adjacências
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

    m_edges++; //> Incrementa o contador de arestas do grafo

     if (use_list) {
        m_list[origin_index].push_back(destiny); //> Adiciona o vértice de destino à lista de adjacências do vértice de origem
        if( not is_targeted){
             m_list[destiny_index].push_back(origin); //> Adiciona o vértice de origem à lista de adjacências do vértice de destino (grafo não direcionado)
        }
    } else {
        m_matrix[origin_index][destiny_index] = 1; //> Marca a presença da aresta na matriz de adjacências
            if( not is_targeted){
                m_matrix[destiny_index][origin_index] = 1; //> Marca a presença da aresta na matriz de adjacências (grafo não direcionado)
            }
    }
}

template <typename T>
void Graph<T>::remove(T origin, T destiny) {
    int origin_index = get_vertex_index(origin, false); //> Obtém o índice do vértice de origem sem criar um novo vértice
    int destiny_index = get_vertex_index(destiny, false); //> Obtém o índice do vértice de destino sem criar um novo vértice

    if (origin_index < 0 or destiny_index < 0) {
        std::cerr << "Error: vertex not found." << std::endl;
        return;
    }

    if (use_list) {
        m_list[origin_index].remove(destiny); //> Remove o vértice de destino da lista de adjacências do vértice de origem
    } else {
        m_matrix[origin_index][destiny_index] = 0;
    }

    m_edges--; //> Decrementa o contador de arestas do grafo
}

template <typename T>
void Graph<T>::print() const {
    if (use_list) {
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