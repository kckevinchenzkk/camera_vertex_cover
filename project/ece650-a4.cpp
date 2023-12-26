// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <climits>
#include <minisat/core/Solver.h>
#include <algorithm>
#include <pthread.h>
#include <chrono>
#include <sys/types.h>
#include <unistd.h>
//#include "helper.h"
using namespace std;




class Graph{
private:
// n indicate vertices 
    int n;
// use adjacency list to store edges
    list<int> *adjlist;
    int vertex_set = 0;
public: 
// add vertices to graph
    void addVertex(int n);
// add edges to graph
    void addEdge(int u, int v);
// print the shortest path from u to v
    void printVC();
// use breadth first search to find the shortest path
    vector<int> VertexCover(int k);
    void printVC_1();
    void printVC_2();
    vector<int> mostIncidentVertexCover();
    vector<int> findMinLengthVertexCoverByApprox2();
};

inline void Graph::addVertex(int num){
    // create the adjacency list 
        adjlist = new list<int>[num+1];
        n = num;
        vertex_set = 1;
}

inline void Graph::addEdge(int u, int v){
    // add v to the end of list u
    adjlist[u].push_back(v);
    // add u to the end of list v
    adjlist[v].push_back(u);
}


inline vector<int> Graph::VertexCover(int k){
    Minisat::Solver solver;
    vector <vector<Minisat::Lit>> M(n);
    //creating nxk literals
    for (int i = 0; i < n; ++i) 
	{
        for (int j = 0; j < k; ++j) 
		{
            Minisat::Lit literal = Minisat::mkLit(solver.newVar());
            M[i].push_back(literal);
        }
    }

    for (int i = 0; i < k; ++i) 
	{
        Minisat::vec<Minisat::Lit> clause;
        for (int j = 0; j < n; ++j) 
		{
            clause.push(M[j][i]);
        }
        solver.addClause(clause);
        clause.clear();
    }


    for (int i = 0; i < n; ++i) 
	{
        for (int j = 0; j < (k - 1); ++j) 
		{
            for (int q = j + 1; q < k; ++q) 
			{
                solver.addClause(~M[i][j], ~M[i][q]); 
            }
        }
    }

    for (int i = 0; i < k; ++i) 
	{
        for (int j = 0; j < (n - 1); ++j) 
		{
            for (int q = j + 1; q < n; ++q) 
			{
                solver.addClause(~M[j][i], ~M[q][i]);
            }
        }
    }
    vector<vector<int>> edges;
    for (int i = 0; i < n; ++i){
        for (list<int>::iterator j = adjlist[i].begin(); j != adjlist[i].end(); ++j){
            if (i < *j) { // To avoid duplicate edges in an undirected graph
                edges.push_back({i, *j});
                // cout << i << *j << " ";
            }
        }
    }
    // for (int i = 0; i<edges.size(); i++){
    //     for (int j = 0; j<edges[i].size(); j++){
    //         cout << edges[i][j] << " ";
    //     }
    // }
    for (long unsigned int i = 0; i < edges.size(); ++i){ 
        Minisat::vec<Minisat::Lit> clause;
        for (int j = 0; j < k; ++j){ 
            {
                clause.push(M[edges[i][0]-1][j]);
                // cout << Minisat::toInt(M[edges[i][1]][j]) << endl;
                clause.push(M[edges[i][1]-1][j]);
            }
        }
        solver.addClause(clause);
        clause.clear();
    }

    bool sat = solver.solve();

    if (sat) 
	{
        vector<int> result;
        for (int i = 0; i < n; i++) 
		{
            for (int j = 0; j < k; ++j) 
			{
                if (Minisat::toInt(solver.modelValue(M[i][j])) == 0) 
				{
                    result.push_back(i);
                }
            }
        }
        return result;
    }
    return {n+1};
}



inline vector<int> Graph::mostIncidentVertexCover() {
    vector<int> result;
    list<int> *tempAdjList = new list<int>[n+1];
    for (int i = 0; i < n+1; ++i) {
        tempAdjList[i] = adjlist[i];
    }
    while (true) {
        int maxDegree = 0;
        int maxVertex = -1;
        //Find the vertex with the maximum degree
        for (int i = 0; i < n+1; ++i) {
            if (tempAdjList[i].size() > maxDegree) {
                maxDegree = tempAdjList[i].size();
                maxVertex = i;
            }
        }
        if (maxDegree == 0) {
            break;
        }
        // Add the vertex to the result and remove its edges
        result.push_back(maxVertex);
        
        for (int neighbor : tempAdjList[maxVertex]) {
            tempAdjList[neighbor].remove(maxVertex);
        }
        tempAdjList[maxVertex].clear();
    }

    delete[] tempAdjList;
    return result;
}

inline vector<int> Graph::findMinLengthVertexCoverByApprox2()
{
    vector<int> result;
    list<int> *tempAdjList = new list<int>[n+1];
    for (int i = 0; i < n+1; ++i) {
        tempAdjList[i] = adjlist[i];
    }
    for (int i = 0; i < n+1; ++i) {
        // maxDegree = 0;
        if (!tempAdjList[i].empty()) {
            int firstElement = tempAdjList[i].front();
            result.push_back(i);
            result.push_back(firstElement);
            for (int neighbor : tempAdjList[i]) {
                tempAdjList[neighbor].remove(i);
            }
            tempAdjList[i].clear();
            for (int neighbor : tempAdjList[firstElement]) {
                tempAdjList[neighbor].remove(firstElement);
            }
            tempAdjList[firstElement].clear();
        }
    }
    delete[] tempAdjList;
    return result;
}

inline void Graph::printVC(){
    vector<int> result;
    vector<int> pre_result;
    // for (int i = 0; i < n; ++i){
    int i = floor(n/2);
    int pre_i = 0;
    result = VertexCover(i);
    for (int r:result){
            pre_result.push_back(r);
    }
    if (result.size() >= 0&&result[0] == n+1){
        pre_i = i;
        i = i + 1;
    }
    else if(result.size() >= 0&&result[0] != n+1){
        pre_i = i;
        i = i - 1;
    }
    // else if(pre_i==i-1&&result.size() >= 0&&result[0] != n+1){
    //     sort(result.begin(), result.end());
    //     cout << "CNF-SAT-VC: ";
    //     for (int j : result) {
    //         cout << j+1 << ' ';
    //     }
    //     break;
    // }
    while (i>=0&&i<n+1){
        result = VertexCover(i);
        // for (int r:result){
        //     pre_result.push_back(r);
        // }
        if (pre_i != i+1&&result.size() >= 0&&result[0] == n+1){
            pre_i = i;
            i = i + 1;
        }
        else if(pre_i == i+1&&result.size() >= 0&&result[0] == n+1){
            sort(pre_result.begin(), pre_result.end());
            cout << "CNF-SAT-VC: ";
            for (int j : pre_result) {
                cout << j+1 << ' ';
            }
            cout << endl;
            break;
        }
        // if (result.size() >= 0&&result[0] == n+1){
        //     pre_i = i;
        //     i = i + 1;
        // }
        else if(pre_i!=i-1&&result.size() >= 0&&result[0] != n+1){
            pre_i = i;
            i = i - 1;
        }
        else if(pre_i==i-1&&result.size() >= 0&&result[0] != n+1){
            sort(result.begin(), result.end());
            cout << "CNF-SAT-VC: ";
            for (int j : result) {
                cout << j+1 << ' ';
            }
            cout << endl;
            break;
        }
        pre_result.clear();
        for (int r:result){
            pre_result.push_back(r);
        }

        // if (result.size() >= 0&&result[0] != n+1){
        //     sort(result.begin(), result.end());
        //     cout << "CNF-SAT-VC: ";
        //     for (int j : result) {
        //         cout << j+1 << ' ';
        //     }
        //     break;
        // }
    }
    // }
    
}

inline void Graph::printVC_1(){
    vector<int> result = mostIncidentVertexCover();
    if (result.size() >= 0&&result[0] != n+1){
        sort(result.begin(), result.end());
        cout << "APPROX-VC-1: ";
        for (int j : result) {
            cout << j << ' ';
        }
        cout << endl;
    }
}    

inline void Graph::printVC_2(){
    vector<int> result = findMinLengthVertexCoverByApprox2();
    if (result.size() >= 0&&result[0] != n+1){
        sort(result.begin(), result.end());
        cout << "APPROX-VC-2: ";
        for (int j : result) {
            cout << j << ' ';
        }
        cout << endl;
    }
} 

void *CNF_proc(void *graph_ptr) {
    auto start = chrono::high_resolution_clock::now();
    Graph *graph = static_cast<Graph*>(graph_ptr);
    graph->printVC();

    clockid_t cid;
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    // cout << "Execution time: " << elapsed.count() << " seconds" << endl;
    return nullptr;
}
void *VC1_proc(void *graph_ptr) {
    auto start = chrono::high_resolution_clock::now();
    Graph *graph = static_cast<Graph*>(graph_ptr);
    graph->printVC_1();

    clockid_t cid;
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    // cout << "Execution time: " << elapsed.count() << " seconds" << endl;

    return nullptr;
}
void *VC2_proc(void *graph_ptr) {
    auto start = chrono::high_resolution_clock::now();
    Graph *graph = static_cast<Graph*>(graph_ptr);
    graph->printVC_2();

    clockid_t cid;
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    // cout << "Execution time: " << elapsed.count() << " seconds" << endl;

    return nullptr;
}


Graph new_graph;
int main(int argc, char** argv) {
    string line;
    // create two flags to check if vertices and edges are defined
    int vertex_set = 0;
    int edge_set = 0;
    int vertex;
    pthread_t t_CNF;
    pthread_t t_CV1;
    pthread_t t_CV2;
    int retcode;
    clockid_t cid;

    
    // read a line of input until EOL and store in a string
    while (getline(cin, line)) {
        if (!line.empty()){
            // create an input stream based on the line
            // we will use the input stream to parse the line
            istringstream input(line);
            // create a graph object
            // Graph new_graph;
            char opt;
            // parse the command
            input >> opt;
            if (input.fail()){
                cerr << "Error: error parsing the command\n";
            }
            else{
                if(opt=='V'){
                    // if command is 'V', then read the number of vertices
                    input >> vertex;
                    if (vertex == 1)
                        cerr << "Error: V must be followed by an integer greater than one\n";
                    else{
                        vertex_set = 1;
                        // add vetices to the graph
                        new_graph.addVertex(vertex);
                        edge_set = 0;
                    }
                }
                else if(opt=='E'){
                    char f_bracket;
                    input >> f_bracket;
                    if (!vertex_set)
                        cerr << "Error: vertices not set\n";
                    else if (edge_set)
                        cerr << "Error: edges already set\n";
                    else if (input.fail())
                        cerr << "Error: error parsing edges\n";
                    else{
                        // if command is 'E', then start read edges
                        edge_set = 1;
                        while (true){
                            char s_bracket;
                            int u;
                            input >> s_bracket;
                            if(s_bracket == '}'){
                                cout << endl;
                                break;
                            }
                            input >> u;
                            char comma;
                            input >> comma;
                            int v;
                            input >> v;
                            // check if the edge is valid
                            if (u>vertex||v>vertex){
                                cerr << "Error: edges not valid\n";
                                vertex_set = 0;
                                break;
                            }
                            // add edge to the graph
                            new_graph.addEdge(u,v);
                            
                            input >> s_bracket;
                            input >> comma;
                            // read until the last edge
                            if(comma == '}'){
                                // method 1
                                //new_graph.printVC();
                                retcode = pthread_create(&t_CNF, NULL, CNF_proc, &new_graph);
                                if (retcode) {
                                    cerr << "Error in pthread_create: " << retcode << endl;
                                    return 1;
                                }
                                retcode = pthread_join(t_CNF, NULL);
                                if (retcode) {
                                    cerr << "Error in pthread_join: " << retcode << endl;
                                }
                                retcode = pthread_create(&t_CV1, NULL, VC1_proc, &new_graph);
                                //int retcode = pthread_create(&t, NULL, CNF_proc, &new_graph);
                                if (retcode) {
                                    cerr << "Error in pthread_create: " << retcode << endl;
                                    return 1;
                                }
                                retcode = pthread_join(t_CV1, NULL);
                                if (retcode) {
                                    cerr << "Error in pthread_join: " << retcode << endl;
                                }
                                retcode = pthread_create(&t_CV2, NULL, VC2_proc, &new_graph);
                                if (retcode) {
                                    cerr << "Error in pthread_create: " << retcode << endl;
                                    return 1;
                                }
                                retcode = pthread_join(t_CV2, NULL);
                                if (retcode) {
                                    cerr << "Error in pthread_join: " << retcode << endl;
                                }

                                // MUTLITHREAD!!!!!!!
                               
                                
                               
                                // !!!!!!!!!!!

                                // killing thread
                                // if (retcode = pthread_kill(t, 0)) {
                                //     printf("main thread: kill() 1 returned error: %s\n", strerror(retcode));
                                //     fflush(stdout);
                                // } else {
                                //     printf("main thread: child running well.\n");
                                //     fflush(stdout);
                                // }

                                break;
                            }
                        }
                    }
                }
            }
        }
        // new_graph.deleteGraph();
        if (cin.fail()&&!cin.eof())
            cerr << "Error: error parsing the command\n";
    }   
}

