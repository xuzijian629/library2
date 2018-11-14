#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

// 頂点クエリに答える用
struct LinkCutTree {
    static const int L = 0, R = 1;
    struct node {
        node *pp, *cp[2]; // *lp, *rp
        bool rev;
        int dcost, dmin;
        int id;
        node() {
            id = -1;
            pp = cp[L] = cp[R] = nullptr;
            rev = false;
            dcost = dmin = 0;
        }
        
        bool is_root() {
            return !pp || pp->cp[L] != this && pp->cp[R] != this;
        }

        void propagate() {
            if (rev) {
                swap(cp[L], cp[R]);
                if (cp[L]) {
                    cp[L]->rev ^= true;
                }
                if (cp[R]) {
                    cp[R]->rev ^= true;
                }
                rev = false;
            }
        }

        // rot_right if d == 1, rot_left if d == 0
        void rot(int d) {
            node *q = pp, *r = q->pp;
            int e = 1 - d;
            if (q->cp[e] = cp[d]) {
                cp[d]->pp = q;
            }
            cp[d] = q; q->pp = this;

            int qd = q->dmin;
            q->dmin = q->dcost;
            if (q->cp[e]) {
                q->dmin = min(q->dmin, dmin + q->cp[e]->dmin);
            }
            if (q->cp[d]) {
                q->dmin = min(q->dmin, q->cp[d]->dmin);
            }
            q->dcost -= q->dmin;
            if (q->cp[e]) {
                q->cp[e]->dmin += dmin - q->dmin;
            }
            if (q->cp[d]) {
                q->cp[d]->dmin -= q->dmin;
            }
            if (cp[e]) {
                cp[e]->dmin += dmin;
            }
            dcost += dmin;
            dmin = qd;
            
            if (pp=r) {
                if (r->cp[L] == q) {
                    r->cp[L] = this;
                }
                if (r->cp[R] == q) {
                    r->cp[R] = this;
                }
            }
        }
        
        void splay() {
            propagate();
            while (!is_root()) {
                node *q = pp;
                if (q->is_root()) {
                    q->propagate();
                    propagate();
                    if (q->cp[L] == this) {
                        rot(R);
                    } else {
                        rot(L);
                    }
                } else{
                    node *r = q->pp;
                    r->propagate();
                    q->propagate();
                    propagate();
                    for (int i = 0; i < 2; i++) { 
                        if (r->cp[i] == q) {
                            if (q->cp[i] == this) {
                                q->rot(1-i);
                                rot(1-i);
                            } else {
                                rot(i);
                                rot(1-i);
                            }
                            break;
                        }
                    }
                }
            }
        }
    };

    vector<node> nodes;
    LinkCutTree(int n) : nodes(n) {
        for (int i = 0; i < n; i++) {
            nodes[i].id = i;
        }
    }
    node *expose(node *x) {
        node *rp = NULL;
        for (node *p = x; p != NULL; p = p->pp) {
            int ndmin;
            p->splay();

            if (p->cp[R]) {
                p->cp[R]->dmin += p->dmin;
            }
            ndmin = p->dmin + p->dcost;
            if (rp) {
                ndmin = min(ndmin, rp->dmin);
            }
            if (p->cp[L]) {
                ndmin = min(ndmin, p->dmin + p->cp[L]->dmin);
            }
            if (rp) {
                rp->dmin = rp->dmin - ndmin;
            }
            if (p->cp[L]) {
                p->cp[L]->dmin += p->dmin - ndmin;
            }
            p->dcost += p->dmin - ndmin;
            p->dmin = ndmin;

            p->cp[R] = rp;
            rp = p;
        }
        x->splay();
        return x;
    }
    int findroot(int n) {
        node *v = expose(&nodes[n]);
        while (v->cp[L]) {
            v->propagate();
            v = v->cp[L];
        }
        v->splay();
        return v->id;
    }
    // vから根に向かうパスの中で最小の重みとその重みをもつ最も根に近い頂点の組を返す
    pair<int, int> findcost(int n) {
        node *v = expose(&nodes[n]);
        while (1) {
            v->propagate();
            if (v->cp[L] && v->cp[L]->dmin == 0) {
                v = v->cp[L];
            } else if ((!v->cp[L] || v->cp[L]->dmin > 0) && v->dcost > 0) {
                v = v->cp[R];
            } else {
                break;
            }
        }
        v->splay();
        return make_pair(v->dmin, v->id);
    }
    // vから根に向かうパスの中にある頂点の重みにxを加える
    void addcost(int n, int x) {
        node *v = expose(&nodes[n]);
        v->dmin += x;
    }
    void cut(int v) {
        node *c = &nodes[v];
        expose(c);
        node *p = c -> cp[L];
        c->cp[L] = nullptr;
        p->pp = nullptr;

        p->dmin += c->dmin;
        c->dmin += c->dcost;
        c->dcost = 0;
    }
    void link(int v, int w) {
        node *c = &nodes[v], *p = &nodes[w];
        expose(c);
        expose(p);
        c->pp = p;
        p->cp[R] = c;

        p->dcost += p->dmin - min(p->dmin, c->dmin);
        if (p->cp[L]) {
            p->cp[L]->dmin += p->dmin - min(p->dmin, c->dmin);
        }
        p->dmin = min(p->dmin, c->dmin);
        c->dmin = c->dmin - p->dmin;
    }
    void evert(int v) {
        node *r = expose(&nodes[v]);
        r->rev ^= true;
    }
};

int main() {
    cin.tie(0);
    cout.tie(0);
    ios::sync_with_stdio(0);
    int n;
    cin >> n;
    LinkCutTree lct(n);
    while (1) {
        char cmd;
        cin >> cmd;
        if (cmd == 'C') {
            int u, v;
            cin >> u >> v;
            u--;
            v--;
            lct.evert(u);
            lct.link(u, v);
        } else if (cmd == 'D') {
            int u, v;
            cin >> u >> v;
            u--;
            v--;
            lct.evert(u);
            lct.cut(v);
        } else if (cmd == 'T') {
            int u, v;
            cin >> u >> v;
            u--;
            v--;
            cout << (lct.findroot(u) == lct.findroot(v) ? "YES" : "NO") << endl; 
        } else {
            return 0;
        }
    }
}