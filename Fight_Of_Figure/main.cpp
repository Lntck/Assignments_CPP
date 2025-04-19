#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Figure;

typedef struct {
    int x;
    int y;
} Coords;


class State {
protected:
    Figure *figure;
    const string name;
public:
    State(Figure *figure, string name): figure(figure), name(name) {}

    virtual string getName() const {
        return name;
    }

    virtual Coords move(string direction) = 0;
};


class Attacking : public State {
public:
    Attacking(Figure *figure) : State(figure, "ATTACKING") {};

    Coords move(string direction) override {
        Coords coords = {0, 0};
        switch(direction[0]) {
            case 'U':
                coords.x -= 2;
                break;
            case 'D':
                coords.x += 2;
                break;
            case 'L':
                coords.y -= 2;
                break;
            case 'R':
                coords.y += 2;
                break;
            default:
                break;
        }
        return coords;
    };
};


class Normal : public State {
public:
    Normal(Figure *figure) : State(figure, "NORMAL") {};

    Coords move(string direction) override {
        Coords coords = {0, 0};
        switch(direction[0]) {
            case 'U':
                coords.x -= 1;
                break;
            case 'D':
                coords.x += 1;
                break;
            case 'L':
                coords.y -= 1;
                break;
            case 'R':
                coords.y += 1;
                break;
            default:
                break;
        }
        return coords;
    };
};


enum figureType {
    GREEN,
    RED,
    GREENCLONE,
    REDCLONE
};

ostream& operator<<(ostream& os, figureType type) {
    switch (type) {
        case GREEN: os << "GREEN"; break;
        case RED: os << "RED"; break;
        case GREENCLONE: os << "GREENCLONE"; break;
        case REDCLONE: os << "REDCLONE"; break;
        default: os << "UNKNOWN"; break;
    }
    return os;
}


class Figure {
private:
    int x, y;
    State *state;
    figureType type;
    bool isCloned = false;
    bool isAlive = true;
    int coinsCollected = 0;
public:
    Figure(int x, int y, figureType type) : x(x), y(y), state(new Normal(this)), type(type) {};

    ~Figure() { delete state;}

    Figure* clone() {
        if (isCloned || type == GREENCLONE || type == REDCLONE) {
            return nullptr;
        }
        return new Figure(y, x, type == GREEN ? GREENCLONE : REDCLONE);
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    figureType getType() {
        return type;
    }

    bool getStatus() {
        return isAlive;
    }

    void Died() {
        this->isAlive = false;
    }

    int getCoins() {
        return coinsCollected;
    }

    void addCoins(int v) {
        this->coinsCollected += v;
    }

    void setX(int x) {
        this->x = x;
    }

    void setY(int y) {
        this->y = y;
    }

    void ChangeStyle() {
        string style = this->state->getName();
        delete this->state;
        if (style == "NORMAL") {
            this->state = new Attacking(this);
        } else {
            this->state = new Normal(this);
        }
        cout << type << " CHANGED STYLE TO " << this->state->getName() << "\n";
    }

    Coords move(string direction) {
        return state->move(direction);
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


typedef struct {
    Figure* figure = nullptr;
    Coin* coin = nullptr;
} Cell;


class Board {
private:
    int size;
    vector<vector<Cell>> board;
    int greenTeamScore = 0;
    int redTeamScore = 0;
public:
    Board(int size): size(size) {
        board.resize(size, vector<Cell>(size));
    }

    bool isValid(int x, int y) {
        return x >= 0 && x < size && y >= 0 && y < size;
    }

    bool placeFigure(int x, int y, Figure* figure) {
        if (!isValid(x, y)) { return false;}
        this->board[x][y].figure = figure;
        return true;
    }

    bool moveFigure(int x, int y, Figure* figure) {
        if (!isValid(x, y)) { return false;}
        if (this->board[x][y].figure != nullptr) {
            this->board[x][y].figure->Died();
            delete this->board[x][y].figure;
            cout << figure->getType() << " MOVED TO " << x+1 << " " << y+1 << " AND KILLED " << this->board[x][y].figure->getType() << "\n";
        } else if (this->board[x][y].coin != nullptr) {
            figure->addCoins(this->board[x][y].coin->getValue());
            if (figure->getType() == RED || figure->getType() == REDCLONE) {
                redTeamScore += figure->getCoins();
            } else { greenTeamScore += figure->getCoins();}
            cout << figure->getType() << " MOVED TO " << x+1 << " " << y+1 << " AND COLLECTED " << this->board[x][y].coin->getValue() << "\n";
            delete this->board[x][y].coin;
        } else {
            cout << figure->getType() << " MOVED TO " << x+1 << " " << y+1 << "\n";
        }
        this->board[x][y].figure = figure;
        figure->setX(x);
        figure->setY(y);
        return true;
    }

    void placeCoin(Coin* coin) {
        if (!isValid(coin->getX(), coin->getY())) { return;}
        this->board[coin->getX()][coin->getY()].coin = coin;
    }

    bool isEmpty(int x, int y) {
        if (!isValid(x, y)) { return false;}
        return this->board[x][y].coin == nullptr && this->board[x][y].figure == nullptr;
    }

    int getRedScore() {
        return redTeamScore;
    }

    int getGreenScore() {
        return greenTeamScore;
    }
};


class Game {
private:
    Board* board;
    vector<Figure*> figures;
    vector<Coin*> coins;
public:
    Game(int size, int X_g, int Y_g, int X_r, int Y_r, vector<Coin*> coins) {
        this->board = new Board(size);
        this->coins = coins;
        figures.push_back(new Figure(X_g, Y_g, GREEN));
        figures.push_back(new Figure(X_r, Y_r, RED));
        for(Coin* coin : coins) { board->placeCoin(coin);}
        for(Figure* figure : figures) { board->placeFigure(figure->getX(), figure->getY(), figure);}
    }

    Figure* findFigure(figureType type) {
        for (Figure* figure : figures) {
            if (figure->getStatus() && figure->getType() == type) {
                return figure;
            }
        }
        return nullptr;
    }

    void move(figureType type, string direction) {
        Figure* figure = findFigure(type);
        if (figure != nullptr) {
            Coords newCoords = figure->move(direction);
            board->moveFigure(newCoords.x + figure->getX(), newCoords.y + figure->getY(), figure);
        } else {
            cout << "INVALID ACTION\n";
        }
    }

    void copy(figureType type) {
        Figure* figure = findFigure(type);
        if (figure != nullptr && type != GREENCLONE && type != REDCLONE && figure->getX() != figure->getY() && board->isEmpty(figure->getY(), figure->getX())) {
            Figure* clone = figure->clone();
            if (clone == nullptr) { 
                cout << "INVALID ACTION\n";
            } else {
                figures.push_back(clone);
                board->placeFigure(clone->getX(), clone->getY(), clone);
                cout << type << " CLONED TO " << clone->getX()+1 << " " << clone->getY()+1 << "\n";
            }
        } else {
            cout << "INVALID ACTION\n";
        }
    }

    void changeStyle(figureType type) {
        Figure* figure = findFigure(type);
        if (figure != nullptr) {
            figure->ChangeStyle();
        } else {
            cout << "INVALID ACTION\n";
        }
    }

    void finish() {
        int redTeamScore = board->getRedScore();
        int greenTeamScore = board->getGreenScore();

        if (redTeamScore > greenTeamScore) {
            cout << "RED TEAM WINS. SCORE " << greenTeamScore << " " << redTeamScore;
        } else if (redTeamScore < greenTeamScore) {
            cout << "GREEN TEAM WINS. SCORE " << greenTeamScore << " " << redTeamScore;
        } else {
            cout << "TIE. SCORE " << greenTeamScore << " " << redTeamScore;
        }
    }
};


figureType stringToFigureType(const string& figure) {
    if (figure == "GREEN") return GREEN;
    if (figure == "RED") return RED;
    if (figure == "GREENCLONE") return GREENCLONE;
    if (figure == "REDCLONE") return REDCLONE;
    return (figureType) 404;
}


int main() {
    int N, X_g, Y_g, X_r, Y_r, M, P;
    cin >> N >> X_g >> Y_g >> X_r >> Y_r >> M;

    vector<Coin*> coins;
    int x, y, v;
    for (int i = 0; i < M; i++) {
        cin >> x >> y >> v;
        coins.push_back(new Coin(x-1, y-1, v));
    }

    Game game(N, X_g-1, Y_g-1, X_r-1, Y_r-1, coins);

    cin >> P;
    for (int i = 0; i < P; i++) {
        string figure, action;
        cin >> figure >> action;

        if (action == "COPY") {
            game.copy(stringToFigureType(figure));
        } else if (action == "STYLE") {
            game.changeStyle(stringToFigureType(figure));
        } else {
            game.move(stringToFigureType(figure), action);
        }
    }
    game.finish();
    return 0;
}