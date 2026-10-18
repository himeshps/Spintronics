#include<bits/stdc++.h>
using namespace std;

#define endl "\n"

const int N = 3e5 + 5;
vector<vector<int>> g;
int tin[N] , tout[N]; 
int timer = 0; 

void dfs(int u , int p){
    tin[u] = timer++; 
    for(auto v : g[u]){
        if(v != p) dfs(v , u); 
    }
    tout[u] = timer; 
}

struct Node{
    long long val; 
};

class segtree{
private : 
    vector<Node>t;
    vector<long long> v; // FIX 1a: Changed to long long to prevent truncation
    int n; 

    Node merge(const Node& a , const Node& b){
        return {a.val + b.val}; 
    }

    void build(int idx , int l , int r){
        if(l == r){
            t[idx].val = v[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(2*idx + 1 , l , mid);
        build(2*idx + 2 , mid + 1 , r); 
        t[idx] = merge(t[2*idx + 1] , t[2*idx + 2]); 
    }
    Node query(int idx , int lq , int rq , int l , int r){
        if(lq > r || rq < l) return {0};
        if(lq <= l && rq >= r){
            return t[idx]; 
        }
        int mid = (l + r) >> 1; 
        return merge(query(2*idx + 1 , lq , rq , l , mid) , query(2*idx + 2 , lq , rq , mid + 1, r)) ;
    }
    void update(int idx , int l , int r , int pos , long long val){ // FIX 1b: val must be long long
        if(l == r){
            t[idx] = {val}; 
            v[pos] = val; 
            return; 
        }
        int mid = (l + r) >> 1; 
        if(pos <= mid){
            update(2*idx + 1 , l , mid , pos , val); 
        }
        else {
            update(2*idx + 2 , mid + 1 , r , pos , val) ;
        }
        t[idx] = merge(t[2*idx + 1] , t[2*idx + 2]); 
    }

public : 
    segtree(vector<long long>& inp){ // FIX 1c: Input vector type must match
        n = inp.size(); 
        v = inp; 
        t.resize(4*n); 
        build(0 , 0 ,n - 1); 
    }
    long long query(int l , int r){ // FIX 1d: Return long long instead of int to prevent overflow
        return query(0 , l , r , 0 , n - 1).val; 
    }
    void update(int index , long long value){ // FIX 1e: Value must be long long
        update(0 , 0 , n - 1 , index , value); 
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr); 
    
    int n , q; 
    if (!(cin >> n >> q)) return 0;
    
    g.resize(n + 1); 
    vector<long long> vec(n + 1); // 1-based indexing for easier tracking of node values
    for(int i = 1; i <= n ; i++){
        cin >> vec[i]; 
    } 
    for(int i = 0; i < n - 1 ; ++i){
        int from , to; cin >> from >> to;
        g[from].push_back(to);
        g[to].push_back(from); 
    }
    
    // Run DFS to calculate the tree traversal order
    dfs(1 , -1) ;

    // FIX 2: Arrange values by traversal entry time order (flattened array mapping)
    vector<long long> flat_vec(n);
    for(int i = 1; i <= n; i++){
        flat_vec[tin[i]] = vec[i];
    }
    
    // Now instantiate the segment tree using the correctly formatted flattened array
    segtree st(flat_vec); 
    
    for(int i = 0 ; i < q ; ++i){
        int type; cin >> type; 
        if(type == 1){
            int s; long long x; cin >> s >> x;
            st.update(tin[s], x); // FIX 3: Point update must target the entry timestamp index
        }
        else{
            int s; cin >> s; 
            int l = tin[s] , r = tout[s]; 
            cout << st.query(l , r - 1) << endl; 
        }
    }
    return 0;
}
