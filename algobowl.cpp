#include <cmath>
#include <map>
#include <set>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

class Node {
    public:
    int index;
    //TODO create comparison operator to sort by first value in descending order
    //so that we can short-circuit edge detection.
    set< pair<Node*, int> > edges; 
    Node(int i) {index = i;}
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


class GraphPartition{
    public:
    vector<Node*> nodes;
    unsigned int N;
    GraphPartition(vector<Node*> n, int size){
        nodes = n;
        N = size;
    }
    GraphPartition* successor();
    int cost();
};

/*
* GraphPartition* GraphPartition::successor()
* 
* Returns the next "successor" of the current permutation by swapping two
*  random values on the front and back half of the list
*/
GraphPartition* GraphPartition::successor(){
    //deepcopy new path
    vector<Node*> newPath = nodes;
    //get random indecies on the front and back of the list
    size_t half = N / 2;
    size_t v1 = rand() % half;
    size_t v2 = rand() % half + half;
    //swap values
    iter_swap(newPath.begin() + v1, newPath.begin() + v2);
    return new GraphPartition(newPath, N);
}

int GraphPartition::cost(){
    int total = 0;
    //check left partition
    for (int i = 0; i < N/2; i++){
        for (pair<Node*, int> e : nodes[i]->edges){
            //since edges are order descending, break when we reach
            //the left partition
            if (e.first->index <= N/2){} //TODO
            else {total += e.second;}
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

    for (int i = 0; i < N/2; i++){
        for (pair<Node*, int> e : nodes[i]->edges){
            if (e.first->index > N/2) cout << "*";
            cout << e.first->index << "->" << nodes[i]->index << "  ";
        }
    }

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
        if (deltaCost < 0){
            currentCost = nextCost;
            current = next;
        }
        else if (rand() < 1/temp/*exp(-deltaCost/temp)*/){
            currentCost = nextCost;
            current = next;
        }
    } 
    cout << "END COST: " << currentCost << endl;
    for (Node* n : current->nodes)
        cout << n->index << " ";
    return 0;    
}
