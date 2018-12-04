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

    enum {
        TY_NONE,
        TY_DOT,
        TY_HOR,
        TY_VER,
        TY_MUSUME,
    };

    int getType(char c) const {
        switch(c){
            case '.': return TY_DOT;
            case '<': return TY_HOR;
            case '^': return TY_VER;
            case '1': return TY_MUSUME;
        }
        return TY_NONE;
    }

    State move(const State& s, int type, int x, int y, int d) const {
        State res = s;
        const int tx = x + dx[d];
        const int ty = y + dy[d];
        const int id = y * W + x;
        const int tid = ty * W + tx;
        switch(type){
        case TY_DOT:
            res[tid] = '.';
            res[id] = EMPTY;
            break;
        case TY_HOR: //only left
            res[id] = res[id+1] = EMPTY;
            res[tid] = '<';
            res[tid+1] = '>';
            break;
        case TY_VER: // only top
            res[id] = res[id+W] = EMPTY;
            res[tid] = '^';
            res[tid+W] = 'v';
            break;
        case TY_MUSUME: // only left-top
            res[id] = res[id+1] = res[id+W] = res[id+1+W] = EMPTY;
            res[tid] = res[tid+1] = res[tid+W] = res[tid+1+W] = MUSUME;
            break;
        }

        return res;
    }

    bool canMove(const State& s, int type, int x, int y, int d) const {
        const int tx = x + dx[d];
        const int ty = y + dy[d];
        switch(type){
        case TY_DOT:
            return isin(tx, ty) && s[ty*W+tx] == EMPTY;
        case TY_HOR: //only left
            if(d == 0)
                return isin(tx, ty) && s[ty*W+tx] == EMPTY;
            else if(d == 2)
                return isin(tx+1, ty) && s[ty*W+tx+1] == EMPTY;
            else
                return isin(tx, ty) && isin(tx+1, ty) && s[ty*W+tx] == EMPTY && s[ty*W+tx+1] == EMPTY;
        case TY_VER: // only top
            if(d == 1)
                return isin(tx, ty) && s[ty*W+tx] == EMPTY;
            else if(d == 3)
                return isin(tx, ty+1) && s[ty*W+tx+W] == EMPTY;
            else
                return isin(tx, ty) && isin(tx, ty+1) && s[ty*W+tx] == EMPTY && s[ty*W+tx+W] == EMPTY;
        case TY_MUSUME: // only left-top
            return isin(tx, ty) && isin(tx+1, ty) && isin(tx, ty+1) && isin(tx+1, ty+1)
                && (s[ty*W+tx] == EMPTY || s[ty*W+tx] == MUSUME)
                && (s[ty*W+tx+1] == EMPTY || s[ty*W+tx+1] == MUSUME)
                && (s[ty*W+tx+W] == EMPTY || s[ty*W+tx+W] == MUSUME)
                && (s[ty*W+tx+1+W] == EMPTY || s[ty*W+tx+1+W] == MUSUME);
        }

        return false;
    }

    States moves(const State& s) const {
        States res;

        for(int y=0;y<H;++y){
            for(int x=0;x<W;++x){
                int type = getType(s[y*W+x]);
                if(type == TY_NONE) continue;
                if(type == TY_MUSUME && !(x+1<W && y+1 < H && s[y*W+x+1] == MUSUME && s[y*W+x+W] == MUSUME)) continue;

                for(int d=0;d<DIR;++d){
                    if(canMove(s, type, x, y, d)){ // cannot move piece
                        res.emplace_back(move(s, type, x, y, d));
                    }
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