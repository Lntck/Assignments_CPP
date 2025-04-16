#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Figure;

class State {
protected:
    Figure *figure;
    const string name = "State";
public:
    State(Figure *figure) {
        this->figure = figure;
    }

    string getName() {
        return name;
    }

    virtual void move() = 0;
};


class Attacking : public State {
private:
    const string name = "ATTACKING";
public:
    Attacking(Figure *figure) : State(figure) {};

    void move() override {

    };
};


class Normal : public State {
private:
    const string name = "NORMAL";
public:
    Normal(Figure *figure) : State(figure) {};

    void move() override {

    };
};


enum figureType {
    GREEN,
    RED,
    GREENCLONE,
    REDCLONE
};


class Figure {
private:
    State *state;
    figureType type;
public:
    Figure(State *state, figureType type) : state(new Normal(this)), type(type) {};

    ~Figure() { delete state;}

    void ChangeStyle() {
        if (this->state->getName() == "Normal") {
            this->state = new Attacking(this);
        } else {
            this->state = new Normal(this);
        }
        cout << type << " CHANGED STYLE TO " << this->state->getName();
    }
};


class Coin {
private:
    const int x, y, value;
public:
    Coin(int x, int y, int value) : x(x), y(y), value(value) {}

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    int getValue() {
        return value;
    }
};


class Board {

};


class Game {
private:
public:

};



int main() {
    int N, X_g, Y_g, X_r, Y_r, M, P;
    cin >> N >> X_g >> Y_g >> X_r >> Y_r >> M;

    vector<Coin*> coins;
    int x, y, v;
    for (int i = 0; i < M; i++) {
        cin >> x >> y >> v;
        coins.push_back(new Coin(x, y, v));
    }

    cin >> P;
    for (int i = 0; i < P; i++) {
        string figure, action;


    }

    return 0;
}