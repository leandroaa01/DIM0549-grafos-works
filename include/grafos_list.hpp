#include <list>
class GraphList {
private:
  std::list<int> *adjacencyList;
  int vertices;
public:
  GraphList(int vertices) {
    this->vertices = vertices;
    adjacencyList = new std::list<int>[vertices];
  }

  void add(int origin, int destiny);
  void remove(int origin, int destiny);
  void print() const;
};
