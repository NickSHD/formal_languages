#include <iostream>
#include <string>
#include <set>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

const int eps = 0;
const int alph_size = 4;

struct nodeNFA {
    vector <nodeNFA*> next[alph_size];
};

class reverseNFA {
public:

    nodeNFA* start;
    nodeNFA* finish;

    reverseNFA() : start(nullptr), finish(nullptr) {}
    reverseNFA(char sym);

    friend reverseNFA operator+ (reverseNFA& first, reverseNFA& second);
    friend reverseNFA operator* (reverseNFA& first, reverseNFA& second);
    reverseNFA klini();


    int find_max_suff(string u);
};

reverseNFA::reverseNFA(char sym) {
    start = new nodeNFA;
    finish = new nodeNFA;

    if (sym == '1') sym = eps;
    else sym = sym - 'a' + 1;

    start->next[sym].push_back(finish);
}

reverseNFA operator+ (reverseNFA& first, reverseNFA& second)  {
    reverseNFA result;
    result.start = new nodeNFA;
    result.finish = new nodeNFA;

    result.start->next[eps].push_back(first.start);
    result.start->next[eps].push_back(second.start);

    first.finish->next[eps].push_back(result.finish);
    second.finish->next[eps].push_back(result.finish);\

    return result;
}

reverseNFA operator* (reverseNFA& first, reverseNFA& second) {
    reverseNFA result;
    result.start = second.start;
    result.finish = first.finish;

    second.finish->next[eps].push_back(first.start);

    return result;
}

reverseNFA reverseNFA::klini() {
    reverseNFA result;
    result.start = new nodeNFA;
    result.finish = result.start;

    result.start->next[eps].push_back(start);
    finish->next[eps].push_back(result.start);

    return result;
}

int reverseNFA::find_max_suff(string u) {
    reverse(u.begin(), u.end());

    set<nodeNFA*> layer;
    queue<nodeNFA*> eps_trans;

    layer.insert(start);
    eps_trans.push(start);

    while (!eps_trans.empty()) {
        nodeNFA* temp = eps_trans.front();
        eps_trans.pop();

        for (int i = 0; i < temp->next[eps].size(); i++) {
            if (layer.find(temp->next[eps][i]) == layer.end()) {
                layer.insert(temp->next[eps][i]);
                eps_trans.push(temp->next[eps][i]);
            }
        }
    }

    int ans = 0;

    for (int i = 0; i < u.size(); i++) {
        set<nodeNFA*> last_layer = layer;
        layer.clear();

        for (auto const &it: last_layer) {
            for (int j = 0; j < it->next[u[i] - 'a' + 1].size(); j++) {
                layer.insert(it->next[u[i] - 'a' + 1][j]);
                eps_trans.push(it->next[u[i] - 'a' + 1][j]);
            }
        }

        while (!eps_trans.empty()) {
            nodeNFA* temp = eps_trans.front();
            eps_trans.pop();

            for (int j = 0; j < temp->next[eps].size(); j++) {
                if (layer.find(temp->next[eps][j]) == layer.end()) {
                    layer.insert(temp->next[eps][j]);
                    eps_trans.push(temp->next[eps][j]);
                }
            }
        }

        if (layer.size() == 0) return ans;
        ans++;
    }

    return ans;
}

int main() {
    vector <reverseNFA> stack_reg;
    string a, u;

    cin >> a >> u;

    for (int i = 0; i < a.size(); i++) {
        if (a[i] == '.') {
            if (stack_reg.empty()) {
                cout << "error";
                return 0;
            }
            reverseNFA second = stack_reg.back();
            stack_reg.pop_back();

            if (stack_reg.empty()) {
                cout << "error";
                return 0;
            }
            reverseNFA first = stack_reg.back();
            stack_reg.pop_back();

            stack_reg.push_back(first * second);
        }
        else if (a[i] == '+') {
            if (stack_reg.empty()) {
                cout << "error";
                return 0;
            }
            reverseNFA second = stack_reg.back();
            stack_reg.pop_back();

            if (stack_reg.empty()) {
                cout << "error";
                return 0;
            }
            reverseNFA first = stack_reg.back();
            stack_reg.pop_back();

            stack_reg.push_back(first + second);
        }
        else if (a[i] == '*') {
            if (stack_reg.empty()) {
                cout << "error";
                return 0;
            }
            reverseNFA temp = stack_reg.back();
            stack_reg.pop_back();

            stack_reg.push_back(temp.klini());
        }
        else {
            if (('a' <= a[i] && a[i] <= 'c') || a[i] == '1')
                stack_reg.push_back(reverseNFA(a[i]));
        }
    }

    if (stack_reg.size() != 1) {
        cout << "error";
        return 0;
    }


    cout << stack_reg.back().find_max_suff(u);

}
