#include <cmath>
#include <map>
#include <set>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

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


int main(int argc, char** argv){

    /*****************
    *******SETUP******
    *****************/


    ifstream fin;
    if (argc != 2){
        cerr << "Wrong format" << endl << "Exectute by: " << argv[0] 
             << " yourInput.txt" << endl;    
        return -1;
    }
    else {
        fin.open(argv[1]);
       if (fin.fail()){
            cerr << "File could not be opened"  << endl; 
        }
    }



    srand(time(NULL));

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
    
    //store first object in order given /*OPTIMIZABLE?*/
    GraphPartition* current = new GraphPartition(nodes, N); 
    
    /*****************
    ****SIMULATED*****
    ****ANNEALING*****
    *****************/

    double T = 1000000;
    double t = T;
    double alpha = 0.9999;

    clock_t start = clock();

    long currentCost = current->cost();
    cout << "START COST: " << currentCost << endl;
    float min = pow(10,-6);
    while (t > min){       
        GraphPartition* next = current->successor();
        long nextCost = next->cost();
        long deltaCost = nextCost - currentCost;
        if (deltaCost < 0){
            currentCost = nextCost;
            current = next;
            cout << currentCost <<  endl;
        }
        else if ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) <
                  (exp(-pow(deltaCost,2)/t))) {
            currentCost = nextCost;
            current = next;
            cout << currentCost << "**" << endl;
        }
        else{
            current->undoSuccessor();    
	    //cout << "No move" << endl;
        }
	t *= alpha;
	//cout << "temp: " << t << endl;
        //cout << "Delta: " << exp(-deltaCost/t) << endl;
	//cout << "Some other random: " << (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) << endl;
    }

    cout << "END COST: " << currentCost << endl;
    cout << (clock() - start)/(float)(CLOCKS_PER_SEC) << endl;
    return 0;    
}
