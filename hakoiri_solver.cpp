#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;


const char EMPTY = '0';
const char MUSUME = '1';
const int DIR = 4;
const int dx[] = {-1,0,1,0};
const int dy[] = {0,-1,0,1};

using State = string;
using States = vector<State>;

struct Solver{
    int H, W;
    State init, goal;
    vector<int> goals;
    map<State,int> memo;
    map<State,State> prev;

    Solver(int H, int W, State s): H(H), W(W), init(s){
        for(int i=0;i<W*H;++i){
            if(init[i] == EMPTY){
                goals.push_back(i);
            }
        }
    }

    bool isin(int x, int y) const{
        return 0 <= x && x < W && 0 <= y && y < H;
    }

    State move(const State& s, char c, int d) const {
        State res(H*W, EMPTY);
        for(int y=0;y<H;++y){
            for(int x=0;x<W;++x){
                if(s[y*W+x] == c){
                    res[(y + dy[d]) * W + (x + dx[d])] = c;
                }
            }
        }
        for(int y=0;y<H;++y){
            for(int x=0;x<W;++x){
                if(s[y*W+x] != c && s[y*W+x] != EMPTY){
                    res[y*W+x] = s[y*W+x];
                }
            }
        }

        return res;
    }

    States moves(const State& s) const {
        States res;
        map<char,int> candidates;

        for(int y=0;y<H;++y){
            for(int x=0;x<W;++x){
                char c = s[y*W+x];
                if(!candidates.count(c)){
                    candidates[c] = 0b1111;
                }

                for(int d=0;d<DIR;++d){
                    int tx = x + dx[d];
                    int ty = y + dy[d];
                    if(!isin(tx, ty) || (s[ty*W+tx] != EMPTY && s[ty*W+tx] != c)){ // cannot move piece
                        candidates[c] &= ~(1 << d);
                    }
                }
            }
        }

        for(auto& candidate: candidates){
            for(int d=0;d<DIR;++d){
                if(candidate.second>>d&1){
                    res.emplace_back(move(s, candidate.first, d));
                }
            }
        }

        return res;
    }
    
    bool isend(const State& s){
        bool end = true;
        for(auto goal : goals){
            if(s[goal] != MUSUME){
                return false;
            }
        }
        return true;
    }

    bool solve(){
        memo.clear();
        prev.clear();
        memo[init] = 0;
        queue<State> q;
        q.push(init);
        while(!q.empty()){
            State s = q.front();
            q.pop();

            if(isend(s)){
                goal = s;
                return true;
            }

            int step = memo[s];
            auto states = moves(s);
            for(auto& state: states){
                if(!memo.count(state)){
                    memo[state] = step + 1;
                    prev[state] = s;
                    q.push(state);
                }
            }
        }

        return false;
    }
    
    void prints(const State& s) const{
        for(int y=0;y<H;++y){
            cout << s.substr(y*W, W) << endl;
        }
    }

    States getTrace() const {
        States res = {goal};
        while(prev.count(res.back())){
            res.emplace_back(prev.at(res.back()));
        }

        reverse(begin(res), end(res));
        return res;
    }
};

Solver input(){
    int h, w;
    cin >> h >> w;
    string s;
    for(int y=0;y<h;++y){
        string tmp;
        cin >> tmp;
        s += tmp;
    }
    return Solver(h, w, State(s));
}

int main(){
    Solver sol = input();
    if(!sol.solve()){
        cout << "Solution does not exist." << endl;
        return 0;
    }

    cout << "Found solution." << endl;
    for(auto& s: sol.getTrace()){
        sol.prints(s);
        cout << endl;
    }

    return 0;
}