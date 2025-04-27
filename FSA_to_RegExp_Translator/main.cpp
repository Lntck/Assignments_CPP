// Rushan Shafeev
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;


struct Transition{
    string from, symbol, to;

    bool operator==(const Transition& other) const {
        return from == other.from 
            && symbol == other.symbol 
            && to == other.to;
    }
};


bool is_word(string word) {
    for (char c : word) {
        if (!isalpha(c) && !isdigit(c) && c != '_') { return false;}
    }
    return true;
}


template<typename T>
bool in_vector(T& value, vector<T>& vec) {
    for (T& element : vec) {
        if (element == value) {
            return true;
        }
    }
    return false;
}


int main() {
    string type, initial, line;
    vector<string> states, alphabet, accepting;
    vector<Transition> transitions;

    // Type
    getline(cin, line);
    if (line != "type=[deterministic]" && line != "type=[non-deterministic]") {
        cout << "Input is malformed.\n";
        return 0;
    }

    type= "type=[deterministic]" == line ? "deterministic" : "non-deterministic";

    // States
    getline(cin, line);
    if (line.substr(0, 8) != "states=[" || line.substr(line.length()-1, 1) != "]" || !line.substr(8, line.length()-9).length()) {
        cout << "Input is malformed.\n";
        return 0;
    }
    stringstream st(line.substr(8, line.length()-9));
    string state;
    while (getline(st, state, ',')) {
        if (state.empty() || !is_word(state)) {
            cout << "Input is malformed.\n";
            return 0;
        }
        states.push_back(state);
    }

    // remove duplicates
    vector<string> unique_states;
    for (string s : states) {
        if (find(unique_states.begin(), unique_states.end(), s) == unique_states.end()) {
            unique_states.push_back(s);
        }
    }
    states = unique_states;
    unique_states.clear();

    // Alphabet
    getline(cin, line);
    if (line.substr(0, 10) != "alphabet=[" || line.substr(line.length()-1, 1) != "]" || !line.substr(10, line.length()-11).length()) {
        cout << "Input is malformed.\n";
        return 0;
    }

    stringstream alp_ss(line.substr(10, line.length()-11));
    string alph;
    while (getline(alp_ss, alph, ',')) {
        if (alph.empty() || !is_word(alph)) {
            cout << "Input is malformed.\n";
            return 0;
        }
        if (alph == "eps" && type == "deterministic") {
            cout << "FSA is non-deterministic.\n";
            return 0;
        }
        alphabet.push_back(alph);
    }

    // Initital
    getline(cin, line);
    initial = line.substr(9, line.length()-10);
    if (line.substr(0, 9) != "initial=[" || line.substr(line.length()-1, 1) != "]" || !is_word(initial)) {
        cout << "Input is malformed.\n";
        return 0;
    }
    if (initial.empty()) {
        cout << "Initial state is not defined.\n";
        return 0;
    }
    if (!in_vector(initial, states)) {
        cout << "A state '" << initial << "' is not in the set of states.\n";
        return 0;
    }

    // Accepting
    getline(cin, line);
    if (line.substr(0, 11) != "accepting=[" || line.substr(line.length()-1, 1) != "]") {
        cout << "Input is malformed.\n";
        return 0;
    }

    stringstream acc_ss(line.substr(11, line.length()-12));
    while (getline(acc_ss, state, ',')) {
        if (!is_word(state)) {
            cout << "Input is malformed.\n";
            return 0;
        }
        if (state.empty()) {
            cout << "Set of accepting states is empty.\n";
            return 0;
        }
        if (!in_vector(state, states)) {
            cout << "A state '" << state << "' is not in the set of states.\n";
            return 0;
        }
        accepting.push_back(state);
    }

    if (accepting.empty()) {
        cout << "Set of accepting states is empty.\n";
        return 0;
    }

    // remove duplicates
    vector<string> unique_accepting;
    for (string s : accepting) {
        if (find(unique_accepting.begin(), unique_accepting.end(), s) == unique_accepting.end()) {
            unique_accepting.push_back(s);
        }
    }
    accepting = unique_accepting;
    unique_accepting.clear();

    sort(accepting.begin(), accepting.end());

    // Transitions
    getline(cin, line);
    if (line.substr(0, 13) != "transitions=[" || line.substr(line.length()-1, 1) != "]" || line.substr(0, 14) == "transitions=[," || line.substr(line.length()-2, 2) == ",]") {
        cout << "Input is malformed.\n";
        return 0;
    }

    stringstream transition_ss(line.substr(13, line.length()-14));
    string transition;
    while (getline(transition_ss, transition, ',')) {
        stringstream trans_ss(transition);
        string state1, alpha, state2, op;
        int i=0;
        if (transition.empty()) {
            cout << "Input is malformed.\n";
            return 0;
        }
        while (getline(trans_ss, op, '>')) {
            if (op.empty()) {
                cout << "Input is malformed.\n";
                return 0;
            }
            if (i==0) { // state1
                if (!is_word(op)) {
                    cout << "Input is malformed.\n";
                    return 0;
                }
                if (!in_vector(op, states)) {
                    cout << "A state '" << op << "' is not in the set of states.\n";
                    return 0;
                }
                state1 = op;
            } else if (i==1) { // alpha
                if (!is_word(op)) {
                    cout << "Input is malformed.\n";
                    return 0;
                }
                if (!in_vector(op, alphabet)) {
                    cout << "A transition symbol '" << op << "' is not in the alphabet.\n";
                    return 0;
                }
                alpha = op;
            } else if (i==2) { // state2
                if (!is_word(op)) {
                    cout << "Input is malformed.\n";
                    return 0;
                }
                if (!in_vector(op, states)) {
                    cout << "A state '" << op << "' is not in the set of states.\n";
                    return 0;
                }
                state2 = op;
            } else {
                cout << "Input is malformed.\n";
                return 0;
            }
            i++;
        }
        Transition tr;
        tr.from = state1;
        tr.symbol = alpha;
        tr.to = state2;
        
        if (in_vector(tr, transitions)) {
            cout << "Input is malformed.\n";
            return 0;
        }

        transitions.push_back(tr);
    }

    // check if fsa is real deterministic
    if (type == "deterministic") {
        for (int i=0; i<transitions.size(); i++) {
            for (int j=i+1; j<transitions.size(); j++) {
                if (transitions[i].from == transitions[j].from && transitions[i].symbol == transitions[j].symbol) {
                    cout << "FSA is non-deterministic.\n";
                    return 0;
                }
            }
        }
    }

    // check for disjoint states
    queue<string> que;
    unordered_set<string> visited;
    visited.insert(initial);
    que.push(initial);
    while (!que.empty()) {
        string curr = que.front();
        que.pop();
        for (Transition t : transitions) {
            if (curr == t.from && visited.find(t.to) == visited.end()) {
                visited.insert(t.to);
                que.push(t.to);
            }
        }
    }
    if (visited.size() != states.size()) {
        cout << "Some states are disjoint.\n";
        return 0;
    }

    // Kleene's Algorithm
    int n = states.size();
    map<string, int> index_state;
    vector<vector<string>> R(n, vector<string>(n));
    for (int i=0; i<n; i++) { index_state[states[i]] = i;}

    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            string expr = "";
            for (Transition t : transitions) {
                if (index_state[t.from] == i && index_state[t.to] == j) {
                    if (!expr.empty()) { expr += "|";}
                    expr += t.symbol;
                }
            }
            if (i == j) {
                if (!expr.empty()) { expr += "|";}
                expr += "eps";
            }
            if (expr.empty()) { expr = "{}";}
            R[i][j] = expr;
        }
    }

    for (int k=0; k<n; k++) {
        vector<vector<string>> new_R(n, vector<string>(n));
        for (int i=0; i<n; i++) {
            for (int j=0; j<n; j++) {
                new_R[i][j] = "(" + R[i][k] + ")(" + R[k][k] + ")*(" + R[k][j] + ")|(" + R[i][j] + ")";
            }
        }
        R = new_R;
    }

    string result = "";
    int count = 0;
    for (int i=0; i<n; i++) {
        if (in_vector(states[i], accepting)) {
            if (!result.empty()) {
                if (!count) {
                    result = "(" + result + ")|(";
                } else {
                    result += ")|(";
                }
                count++;
            }
            result += R[0][i];
        }
    }
    if (result == "") { 
        result = "({})";
    } else {
        if (count) {
            result = result + ")";
        } else {
            result = "(" + result + ")";
        }
    }
    

    cout << result << "\n";
    return 0;
}