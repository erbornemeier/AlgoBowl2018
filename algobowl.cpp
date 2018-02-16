#include <cmath>
#include <map>
#include <set>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;
class Node;


class Node {
    public:
    int id;
    bool isLeft;
    set< pair<Node*, int> > edges; //TODO: Implement custom ordering
    Node(int i) {id = i;}
    void addEdge(Node*, int);
};

/*
* void Node::addEdge()
* Adds an edge bidirectionally to both nodes
*/
void Node::addEdge(Node* to, int cost){
    edges.insert(make_pair(to, cost));
    to->edges.insert(make_pair(this, cost));
}

//TODO: implement this into the edge ordering in order to 
//short circuit cost calculations
struct betterOrdering{
  bool operator() (const pair<Node*, int>& lhs, const pair<Node*, int>& rhs) const{
      if (lhs.first->isLeft) return true;
      if (rhs.first->isLeft) return false;
      return true;
    }      
};



class GraphPartition{
    public:
    vector<Node*> nodes;
    pair<int, int> lastSwapped;
    unsigned int N;
    GraphPartition(vector<Node*> n, int size){
        nodes = n;
        N = size;
    }
    GraphPartition* successor();
    void undoSuccessor();
    int cost();
};

/*
* GraphPartition* GraphPartition::successor()
* 
* Returns the next "successor" of the current permutation by swapping two
*  random values on the front and back half of the list
*/
GraphPartition* GraphPartition::successor(){
    //get random indecies on the front and back of the list
    int half = N / 2;
    int v1 = rand() % half;
    int v2 = rand() % half + half;
    nodes[v1]->isLeft = false;
    nodes[v2]->isLeft = true;
    iter_swap(nodes.begin() + v1, nodes.begin() + v2);
    lastSwapped = make_pair(v1,v2);
    return this;
}

void GraphPartition::undoSuccessor(){
    nodes[lastSwapped.first]->isLeft = false;
    nodes[lastSwapped.second]->isLeft = true;
    iter_swap(nodes.begin() + lastSwapped.first, nodes.begin() + lastSwapped.second); 
}

int GraphPartition::cost(){
    int total = 0;
    //check left partition
    for (int i = 0; i < N/2; i++){
        for (pair<Node*, int> e : nodes[i]->edges){
            if (e.first->isLeft) {} //TODO: Implement optimization once sorted properly
            else    total += e.second;
        }    
    }

    return total;    
}


int main(){

    /*****************
    *******SETUP******
    *****************/

    srand(time(NULL));

    //get number of nodes and edges
    int N, E;
    cin >> N >> E;

    //create initial set of nodes
    vector<Node*> nodes;
    for (int i = 1; i <= N; i++){
        Node* newNode = new Node(i);
        newNode->isLeft = (i <= N/2);
        nodes.push_back(newNode);
    }

    //read in and create edges
    for (int i = 0; i < E; i++){
        int from, to, cost;
        cin >> from >> to >> cost;
        nodes[from-1]->addEdge(nodes[to-1], cost);
    }
    
    //store first object in order given /*OPTIMIZABLE?*/
    GraphPartition* current = new GraphPartition(nodes, N); 
    
    /*****************
    ****SIMULATED*****
    ****ANNEALING*****
    *****************/

    int T = 100000;
    int alpha = 0.99;

    long currentCost = current->cost();
    cout << "START COST: " << currentCost << endl;

    for (int i = 1; i <= T; i++){
        float temp = i;
        GraphPartition* next = current->successor();
        long nextCost = next->cost();
        long deltaCost = nextCost - currentCost;
        if (deltaCost <= 0){
            currentCost = nextCost;
            current = next;
        }
        else if (rand() < exp(-deltaCost/temp)){
            currentCost = nextCost;
            current = next;
        }
        else{
            current->undoSuccessor();    
        }
    }

    cout << "END COST: " << currentCost << endl;
    for (int i = 0; i < N/2; i++){
        cout << current->nodes[i]->id << endl;
        for (pair<Node*, int> e : current->nodes[i]->edges)
            if (!e.first->isLeft) cout << e.first->id << "->" << current->nodes[i]->id << " ";
        cout << endl;
    }

    return 0;    
}
