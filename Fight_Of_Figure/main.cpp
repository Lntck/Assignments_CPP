#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Structure to hold board coordinates
typedef struct {
    int x;
    int y;
} Coords;

class Figure;

// Abstract base class for figure states (State pattern)
class State {
protected:
    Figure *figure;
    const string name;
public:
    // Initializes state with figure and name
    State(Figure *figure, string name): figure(figure), name(name) {}

    // Returns the state's name
    virtual string getName() const {
        return name;
    }

    // Pure virtual method for movement calculation
    virtual Coords move(string direction) = 0;
};

// State for aggressive movement (two steps)
class Attacking : public State {
public:
    // Initializes Attacking state
    Attacking(Figure *figure) : State(figure, "ATTACKING") {};

    // Calculates movement for attacking mode
    Coords move(string direction) override {
        Coords coords = {0, 0};
        // Maps direction to two-step coordinate changes
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

// State for regular movement (one step)
class Normal : public State {
public:
    // Initializes Normal state
    Normal(Figure *figure) : State(figure, "NORMAL") {};

    // Calculates movement for normal mode
    Coords move(string direction) override {
        Coords coords = {0, 0};
        // Maps direction to one-step coordinate changes
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

// Enumeration for figure types
enum figureType {
    GREEN,
    RED,
    GREENCLONE,
    REDCLONE
};

// Outputs string representation of figure type
ostream& operator<<(ostream& os, figureType type) {
    // Converts figure type to string
    switch (type) {
        case GREEN: os << "GREEN"; break;
        case RED: os << "RED"; break;
        case GREENCLONE: os << "GREENCLONE"; break;
        case REDCLONE: os << "REDCLONE"; break;
        default: os << "UNKNOWN"; break;
    }
    return os;
}

// Represents a figure on the board
class Figure {
private:
    int x, y;
    State *state;
    figureType type;
    bool isCloned = false;
    bool isAlive = true;
    int coinsCollected = 0;
public:
    // Initializes figure with position, type, and default state
    Figure(int x, int y, figureType type) : x(x), y(y), state(new Normal(this)), type(type) {};

    // Cleans up state
    ~Figure() { delete state;}

    // Clones figure (Prototype pattern)
    Figure* clone() {
        // Prevents cloning if already cloned or is a clone
        if (isCloned || type == GREENCLONE || type == REDCLONE) {
            return nullptr;
        }
        isCloned = true;
        return new Figure(y, x, type == GREEN ? GREENCLONE : REDCLONE);
    }

    // Returns x-coordinate
    int getX() {
        return x;
    }

    // Returns y-coordinate
    int getY() {
        return y;
    }

    // Returns figure type
    figureType getType() {
        return type;
    }

    // Returns alive status
    bool getStatus() {
        return isAlive;
    }

    // Marks figure as dead
    void Died() {
        this->isAlive = false;
    }

    // Returns collected coins
    int getCoins() {
        return coinsCollected;
    }

    // Adds coins to collection
    void addCoins(int v) {
        this->coinsCollected += v;
    }

    // Sets x-coordinate
    void setX(int x) {
        this->x = x;
    }

    // Sets y-coordinate
    void setY(int y) {
        this->y = y;
    }

    // Toggles between Normal and Attacking states
    void ChangeStyle() {
        string style = this->state->getName();
        delete this->state;
        // Switches state based on current style
        if (style == "NORMAL") {
            this->state = new Attacking(this);
        } else {
            this->state = new Normal(this);
        }
        cout << type << " CHANGED STYLE TO " << this->state->getName() << "\n";
    }

    // Delegates movement to current state
    Coords move(string direction) {
        return state->move(direction);
    }
};

// Represents a coin on the board
class Coin {
private:
    const int x, y, value;
public:
    // Initializes coin with position and value
    Coin(int x, int y, int value) : x(x), y(y), value(value) {}

    // Returns x-coordinate
    int getX() {
        return x;
    }

    // Returns y-coordinate
    int getY() {
        return y;
    }

    // Returns coin value
    int getValue() {
        return value;
    }
};

// Structure for board cell
typedef struct {
    Figure* figure = nullptr;
    Coin* coin = nullptr;
} Cell;

// Manages game board
class Board {
private:
    int size;
    vector<vector<Cell>> board;
    int greenTeamScore = 0;
    int redTeamScore = 0;
public:
    // Initializes board with given size
    Board(int size): size(size) {
        board.resize(size, vector<Cell>(size));
    }

    // Validates coordinates
    bool isValid(int x, int y) {
        return x >= 0 && x < size && y >= 0 && y < size;
    }

    // Places figure at coordinates
    bool placeFigure(int x, int y, Figure* figure) {
        if (!isValid(x, y)) { return false;}
        this->board[x][y].figure = figure;
        return true;
    }

    // Checks if figures are on the same team
    bool sameColor(int x, int y, Figure* figure) {
        figureType type = this->board[x][y].figure->getType();
        // Determines if figures belong to the same team
        return ((((type == RED) || (type == REDCLONE)) && ((figure->getType() == RED || figure->getType() == REDCLONE))) ||
        (((type == GREEN) || (type == GREENCLONE)) && ((figure->getType() == GREEN || figure->getType() == GREENCLONE))));
    }

    // Moves figure to new coordinates
    bool moveFigure(int x, int y, Figure* figure) {
        if (!isValid(x, y)) { return false;}
        // Handles destination interactions
        if (this->board[x][y].figure != nullptr) {
            if (sameColor(x, y, figure)) { return false;}
            this->board[x][y].figure->Died();
            figureType killedType = this->board[x][y].figure->getType();
            this->board[x][y].figure = nullptr;
            cout << figure->getType() << " MOVED TO " << x+1 << " " << y+1 << " AND KILLED " << killedType << "\n";
        } else if (this->board[x][y].coin != nullptr) {
            // Collects coin and updates team score
            figure->addCoins(this->board[x][y].coin->getValue());
            if (figure->getType() == RED || figure->getType() == REDCLONE) {
                redTeamScore += this->board[x][y].coin->getValue();
            } else { greenTeamScore += this->board[x][y].coin->getValue();}
            cout << figure->getType() << " MOVED TO " << x+1 << " " << y+1 << " AND COLLECTED " << this->board[x][y].coin->getValue() << "\n";
            this->board[x][y].coin = nullptr;
        } else {
            cout << figure->getType() << " MOVED TO " << x+1 << " " << y+1 << "\n";
        }
        this->board[figure->getX()][figure->getY()].figure = nullptr;
        this->board[x][y].figure = figure;
        figure->setX(x);
        figure->setY(y);
        return true;
    }

    // Places coin on board
    void placeCoin(Coin* coin) {
        if (!isValid(coin->getX(), coin->getY())) { return;}
        this->board[coin->getX()][coin->getY()].coin = coin;
    }

    // Checks if cell is empty
    bool isEmpty(int x, int y) {
        if (!isValid(x, y)) { return false;}
        return this->board[x][y].coin == nullptr && this->board[x][y].figure == nullptr;
    }

    // Returns red team score
    int getRedScore() {
        return redTeamScore;
    }

    // Returns green team score
    int getGreenScore() {
        return greenTeamScore;
    }
};

// Manages game logic (Facade pattern)
class Game {
private:
    Board* board;
    vector<Figure*> figures;
    vector<Coin*> coins;
public:
    // Initializes game with board, figures, and coins
    Game(int size, int X_g, int Y_g, int X_r, int Y_r, vector<Coin*> coins) {
        this->board = new Board(size);
        this->coins = coins;
        figures.push_back(new Figure(X_g, Y_g, GREEN));
        figures.push_back(new Figure(X_r, Y_r, RED));
        for(Coin* coin : coins) { board->placeCoin(coin);}
        for(Figure* figure : figures) { board->placeFigure(figure->getX(), figure->getY(), figure);}
    }

    // Finds alive figure by type
    Figure* findFigure(figureType type) {
        for (Figure* figure : figures) {
            if (figure->getStatus() && figure->getType() == type) {
                return figure;
            }
        }
        return nullptr;
    }

    // Moves figure in specified direction
    void move(figureType type, string direction) {
        Figure* figure = findFigure(type);
        if (figure != nullptr) {
            Coords newCoords = figure->move(direction);
            if (!board->moveFigure(newCoords.x + figure->getX(), newCoords.y + figure->getY(), figure)) {
                cout << "INVALID ACTION\n";
            }
        } else {
            cout << "INVALID ACTION\n";
        }
    }

    // Clones a figure
    void copy(figureType type) {
        Figure* figure = findFigure(type);
        // Validates cloning conditions
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

    // Changes figure's style
    void changeStyle(figureType type) {
        Figure* figure = findFigure(type);
        if (figure != nullptr) {
            figure->ChangeStyle();
        } else {
            cout << "INVALID ACTION\n";
        }
    }

    // Displays game outcome
    void finish() {
        int redTeamScore = board->getRedScore();
        int greenTeamScore = board->getGreenScore();
        // Determines winner based on scores
        if (redTeamScore > greenTeamScore) {
            cout << "RED TEAM WINS. SCORE " << greenTeamScore << " " << redTeamScore << "\n";
        } else if (redTeamScore < greenTeamScore) {
            cout << "GREEN TEAM WINS. SCORE " << greenTeamScore << " " << redTeamScore << "\n";
        } else {
            cout << "TIE. SCORE " << greenTeamScore << " " << redTeamScore << "\n";
        }
    }
};

// Converts string to figure type
figureType stringToFigureType(const string& figure) {
    // Maps string to figureType enum
    if (figure == "GREEN") return GREEN;
    if (figure == "RED") return RED;
    if (figure == "GREENCLONE") return GREENCLONE;
    if (figure == "REDCLONE") return REDCLONE;
    return (figureType) 404;
}

// Main function to run the game
int main() {
    int N, X_g, Y_g, X_r, Y_r, M, P;
    cin >> N >> X_g >> Y_g >> X_r >> Y_r >> M;

    // Creates coins from input
    vector<Coin*> coins;
    int x, y, v;
    for (int i = 0; i < M; i++) {
        cin >> x >> y >> v;
        coins.push_back(new Coin(x-1, y-1, v));
    }

    // Initializes game
    Game game(N, X_g-1, Y_g-1, X_r-1, Y_r-1, coins);

    // Processes actions
    cin >> P;
    for (int i = 0; i < P; i++) {
        string figure, action;
        cin >> figure >> action;
        // Handles action types
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