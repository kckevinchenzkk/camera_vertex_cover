// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
#include <vector>
#include <list>
#include <typeinfo>
#include <climits>
#include <sstream>
// defined std::cout
#include <iostream>
#include <string>
#include <cstring>


using namespace std;
// Gobal var
class Graph{
    // No. of vertices
    int V;
    // Pointer to an array containing adjacency lists
    vector<list<int>> adj;
    // flag of E is initialized
    bool initialized;
    // flag of E is valid
    bool valid;
    public:
        // mutators
        void set_V(int val){
            // set V as val
            V = val + 1;
            // reset adj and resize to V
            vector<list<int>> adj_new;
            adj = adj_new;
            adj.resize(V);
        }
        void set_init_to_false(){
            initialized = false;
        }
        void set_init_to_true(){
            initialized = true;
        }   
        void set_valid_to_fasle(){
            valid = false;
        }
        void set_valid_to_true(){
            valid = true;
        }
        // record edge using list
        void addEdge(int src, int dest){
            // push dest vertex to src list
            adj[src].push_back(dest);
            // push src vertex to dest list
            adj[dest].push_back(src);
        }
        // accessor
        int get_V(){
            return V;
        }
        // accessor 
        bool get_init(){
            return initialized;
        }
        // accessor
        bool get_valid(){
            return valid;
        }
        // visualize the adj, use for debug purpose
        void print_adj(){
            for (int i=0; i < V; i++){
                std::cout << i << " -> ";
                for (int j : adj[i]) {
                    std::cout << j << " ";
                }
                std::cout << std::endl;
            }
        }
        bool check_empty(){
            for (int i = 0; i < V; ++i) {
                // If any adjacency list is not empty, return false
                if (!adj[i].empty()) {
                    return false;
                }
            } 
            // If all adjacency lists are empty, return true
            return true;
        }
        /*
        // function to implement BFS
        bool BFS(int src, int dest, int v, int pred[], int dist[]){
            // a queue to maintain queue of vertices whose
            // adjacency list is to be scanned as per normal DFS algorithm
            list<int> queue;
            // boolean array visited[] which stores the information whether ith vertex 
            // is reached at least once in the Breadth first search
            bool visited[V];
            // initially all vertices are unvisited so v[i] for all i is false
            // and as no path is yet constructed dist[i] for all i set to infinity
            for (int i = 0; i < V; i++) {
                visited[i] = false;
                dist[i] = INT_MAX;
                pred[i] = -1;
            }
            // now source is first to be visited and distance from source to itself is zero
            visited[src] = true;
            dist[src] = 0;
            queue.push_back(src);
            // BFS algorithm
            while (!queue.empty()){
                // the vertex needs to check its adjacency list
                int u = queue.front();
                queue.pop_front();
                //cout << "u=" << u <<endl;
                for (int j : adj[u]){ // range-based loop to iterate over adj[u], which is a list
                    if (!visited[j]){ // replaced adj[u][i] with v
                        // cout << "visiting"<< j << endl;

                        // if the node is not visited
                        visited[j] = true;
                        // distance add 1 from the predecessor
                        dist[j] = dist[u] + 1;
                        pred[j] = u;  // keep track of the predecessor
                        queue.push_back(j); // adding the vertex to the queue for further BFS
                    }

                    // cout << "dist" << endl;
                    // for (int i = 0 ; i < V;i++){
                    //     cout << dist[i] << endl;
                    // }
                    // cout << endl<<"pred"<<endl;
                    // for (int i = 0 ; i < V;i++){
                    //     cout << pred[i] << endl;
                    // }
                    // cout << endl;

                    // stop when reach the destination
                    if (j == dest){
                        return true;
                    }
                }
            }
            return false;
        }

        // print the shorest path
        void printShortestDistance(int s, int dest, int v){
            int pred[v+1], dist[v+1];
            // as no path is yet constructed dist[i] for all i set to infinity
            for(int i = 0; i < v+1; i++) {
                dist[i] = INT_MAX;
                pred[i] = -1;
            }
            // destination or source vertices is not exist when they are greater than the size of V or smaller than 1
            if (dest > v || s > v || s < 1 || dest < 1){
                cout << "Error: Given source or destination doesn't exist" << endl;
                return;
            }
            // running BFS
            if (BFS(s,dest,v,pred,dist) == false){
                cout << "Error: Given source and destination vertices are not connected" << endl;
                return;
            }
            // initilize a path vector to store the path
            vector<int> path;
            int crawl = dest;
            path.push_back(crawl);
            //cout << crawl << endl;
            // pred stored the predecessor
            // keeping push_back until the end (-1) will get a complete path
            if (crawl < v+1 && crawl > 0){
                while (pred[crawl] != -1){
                    path.push_back(pred[crawl]);
                    crawl = pred[crawl];
                }
                //cout << "Length:" <<dist[dest] << endl;
                // print the path
                for (int i = path.size()-1; i>=0; i--){
                    if (i == 0){
                        cout << path[i];
                    }
                    else{
                        cout << path[i] << "-";
                    }
                }
                cout << endl;
            }
        }
        */
        
        vector<int> VertexCover(int k){
            // -- allocate on the heap so that we can reset later if needed
            std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
            vector<vector<Minisat::Lit>> L(V);
            // creating literals
            for (int i = 0; i < V; ++i){
                for (int j = 0; j < k; ++j){
                    Minisat::Lit literal = Minisat::mkLit(solver->newVar());
                    L[i].push_back(literal);
                }
            }
            // adding clauses
            // 1 -> At least one vertex is the ith vertex in the vertex cover:
            // ∀i ∈ [1, k], a clause (x1,i ∨ x2,i ∨ · · · ∨ xn,i)
            for (int i = 0; i < k; ++i){
                Minisat::vec<Minisat::Lit> literals;
                for (int j = 0; j < V; ++j){
                    literals.push(L[j][i]);
                }
                solver->addClause(literals);
                literals.clear();
            }
            // 2 -> No one vertex can appear twice in a vertex cover.
            // ∀m ∈ [1, n], ∀p, q ∈ [1, k] with p < q, a clause (¬xm,p ∨ ¬xm,q)
            for (int m = 0; m < V; ++m){
                for (int p = 0; p < k - 1; ++p){
                    for (int q = p + 1; q < k; ++q){
                        solver->addClause(~L[m][p],~L[m][q]);
                    }
                }
            }
            // 3 No more than one vertex appears in the mth position of the vertex cover.
            // ∀m ∈ [1, k], ∀p, q ∈ [1, n] with p < q, a clause (¬xp,m ∨ ¬xq,m)
            for (int m = 0; m < k; ++m){
                for (int p = 0; p < V - 1; ++p){
                    for (int q = p + 1; q < V; ++q){
                        solver->addClause(~L[p][m],~L[q][m]);
                    }
                }
            }
            // 4 Every edge is incident to at least one vertex in the vertex cover.
            // ∀⟨i, j⟩ ∈ E, a clause (xi,1 ∨ xi,2 ∨ · · · ∨ xi,k ∨ xj,1 ∨ xj,2 ∨ · · · ∨ xj,k)
            vector<vector <int>> edges;
            // Loop through each vertex in the graph
            for (int i = 0; i < int(adj.size()); i++) {
                // Iterate over all vertices adjacent to the current vertex (i)
                for (int neighbor : adj[i]) {
                    // Ensure each edge is only added once
                    if (i < neighbor) {
                        edges.push_back({i, neighbor}); // Add the edge to the list
                    }
                }
            }
            // Iterate over each edge in the 'edges' vector
            for (int i = 0; i < int(edges.size()); ++i) { 
                Minisat::vec<Minisat::Lit> clause; // Create a new clause for the SAT solver

                // Iterate over a range from 0 to k-1
                for (int j = 0; j < k; ++j) { 
                    // Add literals to the clause based on the matrix 'M' and the current edge
                    clause.push(L[edges[i][0] - 1][j]); // Adjust the vertex index and add the literal
                    clause.push(L[edges[i][1] - 1][j]); // Do the same for the other vertex of the edge
                }
                // Add the constructed clause to the solver
                solver->addClause(clause);
                // Clear the clause for the next iteration
                clause.clear();
            }
            bool sat = solver->solve();
            if (sat){
                vector<int> result;
                for (int i = 0; i < V; i++) {
                    for (int j = 0; j < k; ++j) {
                        // Check if the model value of M[i][j] is 0
                        if (Minisat::toInt(solver->modelValue(L[i][j])) == 0) {
                            // If so, add the index 'i' to the result
                            result.push_back(i);
                        }
                    }
                }
                // Return the result containing the indices
                return result;
            }
            return {-1};
        }

        void print_VertexCover(){
            vector<int> result;
            for(int i = 0; i < V; i++){
                result = VertexCover(i);
                if (result.size() >= 0 && result[0]!=-1){
                    for (int j : result){
                        cout << j+1 << " ";
                    }
                    cout << endl;
                    break;
                }
            }
        }
};


Graph g;
int size;
void parse_input (string input){
    istringstream input_flow(input);
    char command;
    char buff;
    int src = -1;
    int dest = -1;
    input_flow >> command;
    //cout << command << endl;
    //cout << typeid(command).name() << endl;
    // peacefully exit with EOF
    if (input_flow.eof()){
        exit(0);
    }
    // switch between three different commands
    switch (command){
        case 'V':{
            input_flow >> size;
            // set total number of vertices
            g.set_V(size);
            // set the flag to false so that E can be define
            g.set_init_to_false();
            g.set_valid_to_fasle();
            break;
        }
        case 'E':{
            // if E is not define
            if(!g.get_init()){
                // set the flag to true which means the E has been defined
                g.set_init_to_true();
                g.set_V(size);
                g.set_valid_to_true();
                // skipping the space character
                input_flow >> buff;
                // reading until the end
                while (!(buff=='}')){
                    string input_src;
                    string input_dest;
                    input_flow >> buff;
                    // if the char < that means the next input are source until reach comma
                    if(buff == '<'){
                        input_flow >> buff;
                        while(!(buff==',')){
                            input_src += buff;
                            input_flow >> buff;
                        }
                        // convert string to integer
                        if (!(input_src.empty())){
                            src = stoi(input_src);
                        }
                        // throw error if a vertex doesn't exist
                        if (src > size){
                            cout << "Error: try to specify an edge for a vertex doesn't exist" << endl;
                            g.set_valid_to_fasle();
                            break;
                        }
                    }
                    // after comma are destination until >
                    if(buff == ','){
                        input_flow >> buff;
                        while(!(buff=='>')){
                            input_dest += buff;
                            input_flow >> buff;
                        }
                        // convert string to integer
                        if (!(input_dest.empty())){
                            dest = stoi(input_dest);
                        }
                        // throw error if a vertex doesn't exist
                        if (dest > size){
                            cout << "Error: try to specify an edge for a vertex doesn't exist" << endl;
                            g.set_valid_to_fasle();
                            break;
                        }
                    
                    }
                    // withdrawn the middle comma
                    input_flow >> buff;
                    // add the edges to the graph only when there is an edge
                    if(!(src == -1)){
                        g.addEdge(src, dest);
                    }
                    src = -1;
                    dest = -1;
                    
                }
                //debugging
                //g.print_adj();
                
                if (size == 0 || g.check_empty()){
                    //cout << "Error: V is empty of E is invalid" << endl;
                    cout << endl;
                    break;
                }
                // else if(){
                //     g.print_adj();
                //     cout << "1" << endl;
                //     break;
                // }
                g.print_VertexCover();
            }
            break;
        }
        /*
        case 's':{
            // if E is not defined and valid then break
            if (!g.get_init() || !g.get_valid()){
                cout << "Error: E is not defined or E is invalid" << endl;
                break;
            }
            string buff;
            // reading the s command input src and dest
            while (input_flow >> buff){
                if (!(buff == "s")){
                    if (src == -1){
                        src = stoi(buff);
                    }
                    else if(dest == -1){
                        dest = stoi(buff);
                    }
                }
            }
            // if the src and dest are the same then only output the vertex
            if (src == dest){
                cout << src << endl;
                break;
            }
            // print the shortest path
            g.printShortestDistance(src, dest, size);
            break;
        }
        */
        default:{
            cerr << "Error: Unknown Command" << endl;
            break;
        }
    }
}



int main(int argc, char **argv) {

    string input_data;
    // read from stdin until EOF
    while (!cin.eof()) {
        string line;
        while(true){
            try{
                if (cin.eof()){
                    break;
                }
                getline(cin, line);
                //cout << line << endl;
                parse_input(line);
            }
            catch(...){
                cerr << "Error: Incorrect Execution." << endl;
            }
        }
    }
    return 0;
}
