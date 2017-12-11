// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <vector>
#include <sstream>
#include <list>
#include <climits>

using namespace std;

int N=0;
int A[100][100];

class Edge{
public:
    int begin;
    int end;
    int w=1;
}Edge1;


struct Node
{
    int u; 
    int w; 
    Node(int a, int b) :u(a), w(b){}
};

struct Record
{
    int pre; 
    int cost; 
};

int n, m, s; 
vector<list<Node>> Adj; 
vector<Record> Path; 

int flag=0;
int V_star;
int V_end;
int count_line=0;
bool edge_confirm = false;
bool vertex_confirm = false;
vector<Edge> edge_list;



void Dijkstra()
{
    vector<bool> isUsed(n, false); 
    list<int> Assi; 
    Path.assign(n, Record());

    for (int i = 0; i < n; i++)
    {
        Path[i].pre = i;
        Path[i].cost = INT_MAX;
    }
    isUsed[s] = true;
    for (auto it = Adj[s].begin(); it != Adj[s].end(); it++)
    {
        Path[it->u].pre = s;
        Path[it->u].cost = it->w;
        Assi.push_back(it->u);
    }
    while (!Assi.empty())
    {
        list<int>::iterator It;
        int minCost = INT_MAX;
        
        for (auto it = Assi.begin(); it != Assi.end(); it++)
        {
            if (minCost > Path[*it].cost)
            {
                minCost = Path[*it].cost;
                It = it;
            }
        }
        int u = *It;
        Assi.erase(It);
        isUsed[u] = true;
        for (auto it = Adj[u].begin(); it != Adj[u].end(); it++)
        {
            if (isUsed[it->u]) continue;
            if (Path[it->u].cost == INT_MAX) Assi.push_back(it->u);
            if (Path[it->u].cost > minCost + it->w)
            {
                Path[it->u].cost = minCost + it->w;
                Path[it->u].pre = u;
            }
        }
    }
}

void Traverse(int k)
{
    if (Path[k].pre == k) { cout << k; return; }
    Traverse(Path[k].pre);
    cout << "-" << k;
}

void Print()
{
    for (int i = 0; i < n; i++)
    {
        if (i == V_end && i == s){
            cout << s << endl;
            break;
        }
        if (i == V_end && Path[i].cost == INT_MAX){
            cerr << "Error: No path\n";
            continue; }
        if (i == V_end) {
            Traverse(i);
            cout << "\n";
        }
        //cout << "Minimal Cost: " << Path[i].cost << endl << endl;
    }
}

void execute_Dij(){
    // execute Dijkstra algorithm

    n = N; // n: number of vertexs
    m = edge_list.size(); // m: number of edges
    s = V_star; // s: begin vertex
    // std::cout << n << m << s;
    Adj.assign(n, std::list<Node>());
    int u, v, w; // u: begin point of edge, v: end point of edge, w: weight(in undirected graph, w is 1)
    w = 1;
    for (int i = 0; i < edge_list.size(); i++)
    {
        u = edge_list[i].begin;
        v = edge_list[i].end;
        Adj[u].push_back(Node(v, w));
    }
    Dijkstra();
    Print();

    // clean global vlaues for next time calculation
    edge_list.clear();
//    num_vertexs = 0;
//    edges = "";
//    v_begin = 0;
//    v_end = 0;
}

bool gain_edges(string edge) {
    flag=1;
    int comma_position = 0;
    string a , b ;
    edge = edge.substr(1);
    edge = edge.substr(0,edge.length()-1);
    //cout<<"Edge: "<<edge<<endl;
    for (int i = 0; i < edge.length(); i++) {
        if (edge[i] == ',') {
            comma_position = i;
            a = edge.substr(0, comma_position);
            b = edge.substr(comma_position + 1, edge.length() - 1);
//            cout<<"a is "<<a<<endl;
//            cout<<"b is "<<b<<endl;
            int aa = stoi(a);
            int bb = stoi(b);
            if ((aa<0)||(aa>=N)||(bb<0)||(bb>=N)){
                cerr << "Error:Invild input.\n";
                return false;
            } else{
                A[aa][bb] = 1;
                A[bb][aa] = 1;
                count_line=count_line+1;
            }
        }
    }
    return true;
}

bool sep_edge_com(string in_edge){

    string m = in_edge.substr(1, in_edge.length());
    int count1 = 0;
    for (int i = 0; i < m.length(); i++) {
        if ((m[i] == ',') && (m[i - 1] == '>')) {
            string edges1;
            edges1 = m.substr(count1, i - count1);
            bool b = gain_edges(edges1);
            if(!b){
                return false;
            }
            count1 = i + 1;
            //cout << edges1 << endl;
        }
        if ((m[i] == '}') && (m[i - 1] == '>')) {
            string edges2;
            edges2 = m.substr(count1, i - count1);
            bool b = gain_edges(edges2);
            if(!b){
                return false;
            }
            //cout << edges2 << endl;
        }
    }
    edge_confirm = true;
    return true;
}

int main(int argc, char** argv) {

    char in_V = 'V';
    char in_E = 'E';
    char in_s = 's';

    while (!cin.eof()) {
        string line;
        getline(cin, line);
        //cout << line << endl;
        istringstream input(line);

        // while there are characters in the input line
        while (!input.eof()) {
            char command;
            // parse an integer
            input >> command;
            //cout << "command is:" << command << endl;
            if (input.fail()) {
                //cerr << "Error:input failed.\n";
                break;
            } else if (command == in_V) {
                unsigned num;
                input >> num;
                if (num > 0) {
                    if(vertex_confirm && !edge_confirm){
                        cerr<<"Error:already has V.\n";
                        break;
                    }
                    N = num;
                    vertex_confirm = true;
                    edge_confirm = false;
                }else{
                    cerr<<"Error:Vertex number should be positive.\n";
                    break;
                }
            } else if (command == in_E) {
                if(N>0) {
                    if(edge_confirm){
                        cerr<<"Error:already has valid E.\n";
                        break;
                    }
                    string in_edge;
                    input >> in_edge;
                    sep_edge_com(in_edge);
                }else{
                    cerr<<"Error:Please enter vertex number first.\n";
                    break;
                }
            } else if (command == in_s) {
                if((N>0)&&(flag==1)){
                    int star_end;
                    input >> star_end;
                   
                    if(star_end>=0 && star_end<N){
                        V_star=star_end;
                        
                    }
                    else{
                        cerr<<"Error:Incorrect start vertex number.\n";
                        break;
                    }
                    input >> star_end;
                    if(star_end>=0 && star_end<N) {
                        V_end = star_end;
                        //  check
                        //cout<<"start end"<<V_star<<V_end<<endl;
                        //cout<<"Node number"<<N<<endl;
                        for(int i=0;i<N;i++){
                            for(int j=0;j<N;j++){
                                if(A[i][j] == 1){
                                    Edge1.begin = i;
                                    Edge1.end = j;
                                    edge_list.push_back(Edge1);
                                }
                            }
                        }
                        execute_Dij();
                    }
                    else{
                        cerr<<"Error:Incorrect end vertex number.\n";
                        break;
                    }
                }
                else{
                    cerr<<"Error:Please enter vertex number and edges first.\n";
                    break;
                }
            } else {
                cerr << "Error:Invaild command.\n";
                break;
            }
        }
    }

}
