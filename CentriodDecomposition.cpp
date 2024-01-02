// get closest marked vertex to the given vertex
#include <iostream>
#include <vector>
#include <queue>

using ll = long long;
using ull = unsigned ll;

const ll MININT = -1e18;
const ll MAXINT = 1e18;

class vertex_ref {
public:
    ll node_number;
    ll distance;
    vertex_ref() {}
    vertex_ref(const ll& _node_number,const ll& _distance) : node_number(_node_number), distance(_distance) {}
    bool operator<(const vertex_ref& other) const{
        return distance > other.distance;
    }
} put_in;

class vertex {
public:
    bool is_marked = false;
    std::priority_queue<vertex_ref> closest_mark_cen;
    std::vector<vertex_ref> cen_parent;
};

class edge {
public:
    ll to;
    ll sub_size = 0;
    ll inv;
};

std::vector<vertex> v;
std::vector<std::vector<edge>> f;
std::vector<bool> checked;
std::vector<bool> marked;

ll recur1(ll u,ll p,ll index) {
    f[u][index].sub_size = 1;
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            f[u][index].sub_size += recur1(f[u][q].to,u,f[u][q].inv);
        }
    }
    f[p][f[u][index].inv].sub_size = v.size() - f[u][index].sub_size;
    return f[u][index].sub_size;
}

void build_size() {
    for(int q = 0;q < f[0].size();q++) {
        recur1(f[0][q].to,0,f[0][q].inv);
    }
}

void recur3(ll u,ll p,ll index,const ll& sub_N,ll cen, ll cen_distance) {
    f[p][f[u][index].inv].sub_size = sub_N - f[u][index].sub_size;
    if(checked[u]) return;
    v[u].cen_parent.push_back(vertex_ref(cen,cen_distance));
    for(int q = 0;q < f[u].size();q++) {
        if(f[u][q].to != p) {
            recur3(f[u][q].to,u,f[u][q].inv,sub_N,cen,cen_distance + 1);
        }
    }
}

void recur2(ll u,ll cen_parent, ll distance) {
    if(checked[u]) return;
    ll get_max_size = MININT;
    ll get_max_sub_tree = 0;
    ll get_total_size = 1;
    for(int q = 0;q < f[u].size();q++) {
        if(f[f[u][q].to][f[u][q].inv].sub_size > get_max_size) {
            get_max_size = f[f[u][q].to][f[u][q].inv].sub_size;
            get_max_sub_tree = f[u][q].to;
        }
        get_total_size += f[f[u][q].to][f[u][q].inv].sub_size;
    }
    if(get_max_size * 2 > get_total_size) {
        recur2(get_max_sub_tree,cen_parent,distance + 1);
    }
    else {
        checked[u] = true;
        for(int q = 0;q < f[u].size();q++) {
            recur3(f[u][q].to,u,f[u][q].inv, f[f[u][q].to][f[u][q].inv].sub_size,u, 1);
        }
        for(int q = 0;q < f[u].size();q++) {
            recur2(f[u][q].to,u,1);
        }
    }
}

void build_centroid() {
    recur2(0,-1,0);
}

void mark_node(ll u) {
    marked[u] = true;
    v[u].closest_mark_cen.push(vertex_ref(u,0));
    for(int q = 0;q < v[u].cen_parent.size();q++) {
        ll p_cen = v[u].cen_parent[q].node_number;
        v[p_cen].closest_mark_cen.push(vertex_ref(u,v[u].cen_parent[q].distance));
    }
}

void unmark_node(ll u) {
    marked[u] = false;
}

ll get_min_mark(ll u) {
    while(v[u].closest_mark_cen.size() > 0 && !marked[v[u].closest_mark_cen.top().node_number]) {
        v[u].closest_mark_cen.pop();
    }
    if(v[u].closest_mark_cen.size())
        return v[u].closest_mark_cen.top().distance;
    return MAXINT;
}

ll get_closest_distance(ll u) {
    ll ans = get_min_mark(u);
    if(marked[u]) return 0;
    for(int q = 0;q < v[u].cen_parent.size();q++) {
        ans = std::min(ans,v[u].cen_parent[q].distance + get_min_mark(v[u].cen_parent[q].node_number));
    }
    return ans;
}

ll get_closest_distance_brute_force(ll u) {
    class nn{
        public:
        ll vertex_number;
        ll distance;
        ll parent;
    };
    std::queue<nn> qu;
    qu.push({u,0,-1});
    while(qu.size()) {
        if(marked[qu.front().vertex_number]) {
            return qu.front().distance;
        }
        else {
            ll v = qu.front().vertex_number;
            for(int q = 0;q < f[v].size();q++) {
                if(f[v][q].to != qu.front().parent)
                    qu.push({f[v][q].to,qu.front().distance + 1,v});
            }
            qu.pop();
        }
    }
    return MAXINT;
}

ll get_rand_value() {
    #define UR (ull)std::rand()
    return UR << 49 | UR << 34 | UR << 19 | UR << 4 | UR & (15);
}

bool run_test() {
    f.clear();
    v.clear();
    checked.clear();
    marked.clear();

    ll vertex_number = (ull)get_rand_value() % (10000) + 30000;
    for(int q = 0;q < vertex_number;q++) {
        f.push_back(std::vector<edge>());
        v.push_back(vertex());
        checked.push_back(false);
        marked.push_back(false);
    }

    for(int q = 1;q < vertex_number;q++) {
        ll parent = (ull)get_rand_value() % q;
        f[parent].push_back(edge());
        f[q].push_back(edge());
        f[parent].back().to = q;
        f[q].back().to = parent;
        f[parent].back().inv = f[q].size() - 1;
        f[q].back().inv = f[parent].size() - 1; 
    }

    build_size();
    build_centroid();

    for(int q = 0;q < 1000;q++) {
        ll mode = std::rand() % 3;
        if(mode == 0) {
            ll u = (ull)get_rand_value() % vertex_number;
            mark_node(u);
        }
        else if(mode == 1) {
            ll u = (ull)get_rand_value() % vertex_number;
            unmark_node(u);
        }
        else {
            ll u = (ull)get_rand_value() % vertex_number;
            ll get_ans = get_closest_distance_brute_force(u);
            ll get_close = get_closest_distance(u);
            if(get_ans != get_close) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    ll _  = 0;
    std::cin >> _;
    for(int q = 1;q <=_;q++) {
        std::cout << "testcase " << q << " : ";
        if(run_test()) {
            std::cout << "passed\n";
        }
        else {
            std::cout << "failed\n";
        }
    }
}
