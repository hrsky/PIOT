#include <map>

using namespace std;

class Dict {
  private:
    Dict() {
      ps = 0;
      is = 0;
    }
    Dict(const Dict&);
    Dict& operator=(const Dict&);

    map<string, int> predicate;
    map<string, int> individual;
    int ps, is;
  public:
    static Dict& getInstance() {
      static Dict instance;
      return instance;
    }
    int addPre(string& s) {
      map<string, int>::iterator it = predicate.find(s);
      if(it != predicate.end()) {
        return it->second;
      }
      predicate.insert(pair<string, int>(s, predicate.size() + 1));
      return predicate.size();
    };
    int addInd(string& s) {
      map<string, int>::iterator it = individual.find(s);
      if(it != individual.end()) {
        return it->second;
      }
      individual.insert(pair<string, int>(s, individual.size() + 1));
      return individual.size();
    }
};


