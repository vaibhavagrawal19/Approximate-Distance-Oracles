#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#define K 4
using namespace std;

string file_path;

struct compare
{
    bool operator()(tuple<int, int, int> &t1, tuple<int, int, int> &t2)
    {
        return std::get<1>(t1) > std::get<1>(t2);
    }
};

void make_delta(vector<vector<tuple<int, int>>> &delta, vector<vector<int>> &vdisjoints, vector<vector<tuple<int, int>>> &v, int n)
{
    for (int index = 0; index < K; index++)
    {
        // tuple -> (vertex, distance, root_ancestor_vertex)
        priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, compare> heap;
        vector<int> distances(n, -1);
        for (int i = K - 1; i >= index; i--)
        {
            for (int j = 0; j < vdisjoints[i].size(); j++)
            {
                heap.push(make_tuple(vdisjoints[i][j], 0, vdisjoints[i][j]));
            }
        }

        int count = 0;

        while (count != n && heap.size() != 0)
        {
            tuple<int, int, int> top = heap.top();
            int node = get<0>(top);
            int distance = get<1>(top);
            int parent = get<2>(top);
            heap.pop();
            if (distances[node] >= 0)
            {
                continue;
            }
            distances[node] = distance;
            delta[index][node] = make_tuple(parent, distance);
            for (int i = 0; i < v[node].size(); i++)
            {
                if (distances[get<0>(v[node][i])] < 0)
                {
                    tuple<int, int, int> next_tuple = make_tuple(get<0>(v[node][i]), distance + get<1>(v[node][i]), parent);
                    heap.push(next_tuple);
                }
            }
            count++;
        }
    }
}

void make_bunches(map<int, int> map[], vector<vector<int>> &vdisjoints, vector<vector<tuple<int, int>>> &v, vector<vector<tuple<int, int>>> &delta, int n)
{
    for (int index = 0; index < K; index++)
    {
        for (int w = 0; w < vdisjoints[index].size(); w++)
        {
            priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, compare> heap;
            vector<int> distances(n, -1);
            int curr_node = vdisjoints[index][w];
            distances[curr_node] = 0;
            map[curr_node][curr_node] = 0;
            for (int j = 0; j < v[curr_node].size(); j++)
            {
                heap.push(make_tuple(get<0>(v[curr_node][j]), get<1>(v[curr_node][j]), get<0>(v[curr_node][j])));
            }
            int count = 1;

            while (count != n && heap.size() != 0)
            {
                tuple<int, int, int> top = heap.top();
                int node = get<0>(top);
                int distance = get<1>(top);
                int parent = get<2>(top);
                heap.pop();
                if (distances[node] >= 0)
                {
                    continue;
                }
                distances[node] = distance;
                if (index < K - 1)
                {
                    if (distance < get<1>(delta[index + 1][node]))
                    {
                        map[node][curr_node] = distance;
                    }
                }
                else
                {
                    map[node][curr_node] = distance;
                }
                for (int i = 0; i < v[node].size(); i++)
                {
                    if (distances[get<0>(v[node][i])] < 0)
                    {
                        tuple<int, int, int> next_tuple = make_tuple(get<0>(v[node][i]), distance + get<1>(v[node][i]), parent);
                        heap.push(next_tuple);
                    }
                }
                count++;
            }
        }
    }
}

void make_disjoints(vector<vector<int>> &vdisjoints, vector<vector<tuple<int, int>>> &v, int n)
{
    vector<int> vertices;

    for (int i = 0; i < n; i++)
    {
        vertices.push_back(i);
    }
    int curr_vert = n;
    double power = -(double)1 / (double)K;
    double prob = 1 - pow(n, power);

    for (int i = 0; i < K - 1; i++)
    {
        int num_alloc = 0;
        int s = 0;
        for (int j = 0; j < curr_vert; j++)
        {
            double rand_val = (double)rand() / (double)RAND_MAX;
            if (rand_val < prob)
            {
                vdisjoints[i].push_back(vertices[j]);
                num_alloc++;
            }
            else
            {
                vertices[s++] = vertices[j];
            }
        }
        curr_vert -= num_alloc;
    }
    for (int i = 0; i < curr_vert; i++)
    {
        vdisjoints[K - 1].push_back(vertices[i]);
    }

    for (int i = 0; i < K; i++)
    {
        // cout << i << "th set\n";
        for (int j = 0; j < vdisjoints[i].size(); j++)
        {
            // cout << vdisjoints[i][j] << " ";
        }
        // cout << endl;
    }
}

int distance_oracle(int u, int v, map<int, int> map[], vector<vector<tuple<int, int>>> &delta)
{
    int w = u;
    int index = 0;

    while (map[v].count(w) == 0)
    {
        index++;
        int c = u;
        u = v;
        v = c;
        w = get<0>(delta[index][u]);
    }

    return map[v][w] + get<1>(delta[index][u]);
}

int dijkistra(int u, int v, int n, vector<vector<tuple<int, int>>> &graph)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> vis(n, 0);
    vector<int> dist(n, INT32_MAX);
    pq.push({0, u});
    dist[u] = 0;
    while (!pq.empty())
    {
        auto curr = pq.top().second;
        pq.pop();

        if (curr == v)
        {
            return dist[curr];
        }
        // cout << "now relaxing " << curr << "\n";
        if (vis[curr])
        {
            continue;
        }
        vis[curr] = 1;
        for (auto it = graph[curr].begin(); it != graph[curr].end(); it++)
        {
            if (!vis[get<0>(*it)])
            {
                // cout << curr << " is discovering " << get<0>(*it) << "\n";
                if (dist[get<0>(*it)] > dist[curr] + get<1>(*it))
                {
                    dist[get<0>(*it)] = dist[curr] + get<1>(*it);
                    pq.push({dist[get<0>(*it)], get<0>(*it)});
                }

                if (get<0>(*it) == v)
                {
                    return dist[get<0>(*it)];
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    file_path = (string)argv[1];

    string GraphLine;
    ifstream Graph(file_path);

    int n = 0, m = 0;
    getline(Graph, GraphLine);
    getline(Graph, GraphLine);
    vector<string> tokens;
    stringstream check1(GraphLine);
    string intermediate;

    // Tokenizing w.r.t. space ' '
    while (getline(check1, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }

    stringstream obj1, obj2, obj3;

    obj1 << tokens[0];
    obj1 >> n;

    obj2 << tokens[1];
    obj2 >> m;

    cout << n << " " << m << "\n";
    vector<vector<tuple<int, int>>> v(n);

    for (int i = 0; i < m; i++)
    {
        getline(Graph, GraphLine);

        // Vector of string to save tokens
        vector<string> tokens;
        int a, b, c;

        // stringstream class check1
        stringstream check1(GraphLine);

        string intermediate;

        // Tokenizing w.r.t. space ' '
        while (getline(check1, intermediate, ' '))
        {
            tokens.push_back(intermediate);
        }

        stringstream obj1, obj2, obj3;

        obj1 << tokens[0];
        obj1 >> a;

        obj2 << tokens[1];
        obj2 >> b;

        a--;
        b--;

        c = rand() % 5;

        // // Printing the token vector
        // for (int i = 0; i < tokens.size(); i++)
        //     cout << tokens[i] << '\n';
        // cout << a << " " << b << " " << c << "\n";
        // cout << tokens[0] << " " << tokens[1] << " " << tokens[2] << "\n";

        n = std::max(n, a);
        n = std::max(n, b);

        v[a].push_back(make_tuple(b, c));
        v[b].push_back(make_tuple(a, c));
    }

    clock_t oracle1, oracle2;

    oracle1 = clock();
    vector<vector<int>> vdisjoints(K);
    make_disjoints(vdisjoints, v, n);
    vector<vector<tuple<int, int>>> delta(K, vector<tuple<int, int>>(n));
    make_delta(delta, vdisjoints, v, n);
    map<int, int> bunches[n];
    make_bunches(bunches, vdisjoints, v, delta, n);
    oracle2 = clock();

    cout << fixed << (double)(oracle2 - oracle1) / (double)(CLOCKS_PER_SEC) << setprecision(5) << endl;
    clock_t dij1, dij2;

    dij1 = clock();
    int **short_path = (int **)malloc(sizeof(int *) * n);
    for(int i=0;i<n;i++){
        short_path[i] = (int *)malloc(sizeof(int) * n);
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            short_path[i][j] = INT32_MAX/2;
        }
    }

    for(int i=0;i<n;i++){
        for(int k=0;k<v[i].size();k++){
            int node = get<0>(v[i][k]);
            short_path[i][node] = get<1>(v[i][k]);
        }
    }

    for(int k=0;k<n;k++){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(short_path[i][j] > short_path[i][k] + short_path[k][j]){
                    short_path[i][j] = short_path[i][k] + short_path[k][j];
                }
            }
        }
    }

    dij2 = clock();

    cout << fixed << (double)(dij2 - dij1) / (double)(CLOCKS_PER_SEC) << setprecision(5) << "\n";

    // dij1 = clock();
    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = i + 1; j < n; j++)
    //     {
    //         dijkistra(i, j, n, v);
    //         // cout << "computed b/w " << i << " " << j << "\n";
    //         // cout << "the distance between " << i << " and " << j << " is: " << dijkistra(i, j, n, v) << "\n";
    //     }
    // }
    // dij2 = clock();

    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < n; j++)
    //     {
    //         int dist = distance_oracle(i, j, bunches, delta);
    //         int actual = dijkistra(i, j, n, v);
    //         if (dist > actual * (2 * K - 1))
    //         {
    //             cout << dist << " " << actual << endl;
    //             cout << "Error\n";
    //             return 0;
    //         }
    //     }
    // }

    cout << "Success\n";
}