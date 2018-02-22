#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class Node {
    public:
        int id;
        bool isLeft;
        vector< pair<Node*, int> > edges; 
        Node(int i) {id = i;}
        void addEdge(Node*, int);
};

/*
* void Node::addEdge()
* Adds an edge bidirectionally to both nodes
*/
void Node::addEdge(Node* to, int cost){
    edges.push_back(make_pair(to, cost));
    to->edges.push_back(make_pair(this, cost));
}

class GraphPartition{
    public:
    vector<Node*> nodes;
    pair<int, int> lastSwapped;
    unsigned int N;
    GraphPartition(vector<Node*> n, int size){
        nodes = n;
        N = size;
    }
    int successor(int);
    void undoSuccessor();
    int cost();
};

/*
* GraphPartition* GraphPartition::successor()
* 
* Returns the next "successor" of the current permutation by swapping two
*  random values on the front and back half of the list
*/
int GraphPartition::successor(int currentCost){
    //get random indecies on the front and back of the list
    int half = N / 2;
    int v1 = rand() % half;
    int v2 = rand() % half + half;
    nodes[v1]->isLeft = false;
    nodes[v2]->isLeft = true;
    iter_swap(nodes.begin() + v1, nodes.begin() + v2);
    lastSwapped = make_pair(v1,v2);
    for (pair<Node*, int> edge: nodes[v2]->edges){
        if (edge.first == nodes[v1])
            continue;
        else if (!edge.first->isLeft)
            currentCost -= edge.second;
        else
            currentCost += edge.second;
    }
    for (pair<Node*, int> edge: nodes[v1]->edges){
        if (edge.first == nodes[v2])
            continue;
        else if (edge.first->isLeft)
            currentCost -= edge.second;
        else
            currentCost += edge.second;
    }

    return currentCost;
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
            if (!e.first->isLeft) 
                total += e.second;
        }    
    }
    return total;    
}


int main(int argc, char** argv){

    /*****************
    *******SETUP******
    *****************/

    ifstream fin;
    ifstream test_fin;
    long test_cost;

    if (argc != 3){
        cerr << "Wrong format" << endl << "Exectute by: " << argv[0] 
             << " yourInput.txt yourInputToCheck.txt" << endl;;    
        return -1;
    }
    else {
       fin.open(argv[1]);
       test_fin.open(argv[2]);
       if (fin.fail()){
            cerr << "Input File could not be opened"  << endl; 
       }

       if (test_fin.fail()){
            cerr << "Input Test File could not be opened"  << endl; 
        }
        else {
            test_fin >> test_cost;
        }
    }

    cout << "TEST COST FOR " << argv[2] << ": " << test_cost << endl;

    //get number of nodes and edges
    int N, E;
    fin >> N >> E;

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
        fin >> from >> to >> cost;
        nodes[from-1]->addEdge(nodes[to-1], cost);
    }
    
    vector<Node*> testNodes;
    for (int i = 0; i < N; i++){
        int newID;
        test_fin >> newID;
        Node* toAdd = nodes[newID - 1];
        toAdd->isLeft = (i < N/2);
        testNodes.push_back(toAdd);
    }

    //store first object in order given /*OPTIMIZABLE?*/
    GraphPartition* current = new GraphPartition(testNodes, N); 
    
    /*****************
    ******TEST********
    *****INPUT********
    *****************/

    long currentCost = current->cost();

    if (currentCost == test_cost) cout << "IT ALL CHECKS OUT!" << endl;
    else                          cout << "WRONG!: test cost -> " << test_cost
                                       << "\t check cost: " << currentCost << endl;

    fin.close();
    test_fin.close();

    return 0;    
}
