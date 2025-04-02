#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;


class Animal {
private:
    const string name;
    int daysLived;
public:
    Animal() = default;
    Animal(const string name, int daysLived) : name(name), daysLived(daysLived) {}
    Animal(Animal& other) : name(other.getName()), daysLived(other.getDaysLived()) {}
    virtual ~Animal() = default;

    int getDaysLived() {
        return this->daysLived;
    }

    string getName() {
        return this->name;
    }

    void sayName() {
        cout << "My name is " << this->name << ", days lived: " << this->daysLived << "\n";
    }

    virtual void attack(Animal& other) = 0;

    void setDaysLived(int NewValue) {
        this->daysLived = NewValue;
    }
};


class Fish : public virtual Animal {
public:
    Fish(const string name, int daysLived) : Animal(name, daysLived) {}

    void attack(Animal& other) override {
        cout << "Fish is attacking\n";
    }
};


class BetterFish : public Fish {
public:
    BetterFish(const string name, int daysLived) : Animal(name, daysLived), Fish(name, daysLived) {}
    BetterFish(Fish& other) : Animal(other), Fish(other) {}

    void attack(Animal& other) override {
        cout << "BetterFish is attacking\n";
    }
};


class Bird : public virtual Animal {
public:
    Bird(const string name, int daysLived) : Animal(name, daysLived) {}

    void attack(Animal& other) override {
        cout << "Bird is attacking\n";
    }
};


class BetterBird : public Bird {
public:
    BetterBird(const string name, int daysLived) : Animal(name, daysLived), Bird(name, daysLived) {}
    BetterBird(Bird& other) : Animal(other), Bird(other) {}

    void attack(Animal& other) override {
        cout << "BetterBird is attacking\n";
    }
};


class Mouse : public virtual Animal {
public:
    Mouse(const string name, int daysLived) : Animal(name, daysLived) {}

    void attack(Animal& other) override {
        cout << "Mouse is attacking\n";
    }
};


class BetterMouse : public Mouse {
public:
    BetterMouse(const string name, int daysLived) : Animal(name, daysLived), Mouse(name, daysLived) {}
    BetterMouse(Mouse& other) : Animal(other), Mouse(other) {}

    void attack(Animal& other) override {
        cout << "BetterMouse is attacking\n";
    }
};
    

class Monster : public virtual BetterFish, public virtual BetterBird, public virtual BetterMouse {
public:
    Monster(string name) : Animal(name, 1), BetterFish(name, 1), BetterBird(name, 1), BetterMouse(name, 1) {}
    Monster(Animal& other) : Monster(other.getName()) {}

    void attack(Animal& other) override {
        cout << "Monster is attacking\n";
    }
};


template <typename T>
class Freedom {
private:
    vector<T*> freedom;
public:
    using value_type = T;
    Freedom() {}
    ~Freedom() { this->kill_all();}

    void addAnimal(T* animal, int flag) {
        this->freedom.push_back(animal);
        if (flag) { animal->sayName();}
        this->sortFreedom();
    }

    void period() {
        auto i = freedom.begin();
        while (i != freedom.end()) {
            T* animal = *i;
            animal->setDaysLived(animal->getDaysLived() + 1);
            if (animal->getDaysLived() > 10 || dynamic_cast<Monster*>(animal)) {
                cout << animal->getName() << " has died of old days\n";
                delete animal;
                i = freedom.erase(i);
            } else {
                ++i;
            }
        }
        this->sortFreedom();
    }

    void print() {
        for (auto* animal : this->freedom) {
            animal->sayName();
        }
    }

    void talk(int index) {
        if (index < 0 || index >= this->freedom.size()) {
            cout << "Animal not found\n";
            return;
        }
        this->freedom[index]->sayName();
    }

    void kill_all() {
        this->freedom.clear();
    }

    void sortFreedom() {
        sort(this->freedom.begin(), this->freedom.end(), [](T* a, T* b) {
            if (a->getDaysLived() != b->getDaysLived()) {
                return a->getDaysLived() < b->getDaysLived();
            }
            return a->getName() < b->getName();
        });
    }
};


template <typename T>
class Cage {
private:
    vector<T*> cage;
public:
    using value_type = T;
    Cage() {}
    ~Cage() { this->kill_all();}

    void addAnimal(T* animal, int flag) {
        this->cage.push_back(animal);
        if (flag) { animal->sayName();}
        this->sortCage();
    }

    void period() {
        auto i = cage.begin();
        while (i != cage.end()) {
            T* animal = *i;
            animal->setDaysLived(animal->getDaysLived() + 1);
            if (animal->getDaysLived() > 10) {
                cout << animal->getName() << " has died of old days\n";
                delete animal;
                i = cage.erase(i);
            } else {
                ++i;
            }
        }
        this->sortCage();
    }

    void talk(int index) {
        if (index < 0 || index >= this->cage.size()) {
            cout << "Animal not found\n";
            return;
        }
        this->cage[index]->sayName();
    }

    void print() {
        for (auto* animal : this->cage) {
            animal->sayName();
        }
    }

    void attack(int index1, int index2) {
        if (index1 < 0 || index1 >= this->cage.size() || index2 < 0 || index2 >= this->cage.size()) {
            cout << "Animal not found\n";
            return;
        }
        this->cage[index1]->attack(*this->cage[index2]);
        delete this->cage[index2];
        this->cage.erase(this->cage.begin() + index2);
    }

    template <typename U>
    void applySubstance(int pos, Cage<U>& otherCage) {
        if (pos < 0 || pos >= this->cage.size()) {
            cout << "Animal not found\n";
            return;
        }

        T* animal = this->cage[pos];
        U* betterAnimal = new U(animal->getName(), ceil(animal->getDaysLived()/2.));
        otherCage.addAnimal(betterAnimal, 0);
        this->cage.erase(this->cage.begin() + pos);
        delete animal;
    }

    void applySubstance(int pos, Freedom<Animal>& freedom) {
        if (pos < 0 || pos >= this->cage.size()) {
            cout << "Animal not found\n";
            return;
        }

        T* animal = this->cage[pos];
        Monster* monster = new Monster(*animal);
        freedom.addAnimal(monster, 0);
        this->kill_all();
    }

    template <typename U>
    void removeSubstance(int pos, Cage<U>& otherCage) {
        if (pos < 0 || pos >= this->cage.size()) {
            cout << "Animal not found\n";
            return;
        }

        T* animal = this->cage[pos];
        U* normalAnimal = new U(animal->getName(), animal->getDaysLived()*2);
        otherCage.addAnimal(normalAnimal, 0);
        this->cage.erase(this->cage.begin() + pos);
        delete animal;
    }

    void kill_all() {
        // Deleting all animals in the cage
        for (auto* animal : this->cage) {
            delete animal;
        }
        this->cage.clear();
    }

    void sortCage() {
        sort(this->cage.begin(), this->cage.end(), [](T* a, T* b) {
            if (a->getDaysLived() != b->getDaysLived()) {
                return a->getDaysLived() < b->getDaysLived();
            }
            return a->getName() < b->getName();
        });
    }
};


template <typename T>
class Aquarium {
private:
    vector<T*> aquarium;
public:
    using value_type = T;
    Aquarium() {}
    ~Aquarium() { this->kill_all();}

    void addAnimal(T* animal, int flag) {
        this->aquarium.push_back(animal);
        if (flag) { animal->sayName();}
        this->sortAquarium();
    }

    void period() {
        auto i = aquarium.begin();
            while (i != aquarium.end()) {
                T* animal = *i;
                animal->setDaysLived(animal->getDaysLived() + 1);
                if (animal->getDaysLived() > 10) {
                    cout << animal->getName() << " has died of old days\n";
                    delete animal;
                    i = aquarium.erase(i);
                } else {
                    ++i;
                }
            }
        this->sortAquarium();
    }

    void talk(int index) {
        if (index < 0 || index >= this->aquarium.size()) {
            cout << "Animal not found\n";
            return;
        }
        this->aquarium[index]->sayName();
    }

    void print() {
        for (auto* animal : this->aquarium) {
            animal->sayName();
        }
    }

    void attack(int index1, int index2) {
        if (index1 < 0 || index1 >= this->aquarium.size() || index2 < 0 || index2 >= this->aquarium.size()) {
            cout << "Animal not found\n";
            return;
        }
        this->aquarium[index1]->attack(*this->aquarium[index2]);
        delete this->aquarium[index2];
        this->aquarium.erase(this->aquarium.begin() + index2);
    }

    template <typename U>
    void applySubstance(int pos, Aquarium<U>& otherAquarium) {
        if (pos < 0 || pos >= this->aquarium.size()) {
            cout << "Animal not found\n";
            return;
        }

        T* animal = this->aquarium[pos];
        U* betterAnimal = new U(animal->getName(), ceil(animal->getDaysLived()/2.));
        otherAquarium.addAnimal(betterAnimal, 0);
        this->aquarium.erase(this->aquarium.begin() + pos);
        delete animal;
    }

    void applySubstance(int pos, Freedom<Animal>& freedom) {
        if (pos < 0 || pos >= this->aquarium.size()) {
            cout << "Animal not found\n";
            return;
        }

        T* animal = this->aquarium[pos];
        Monster* monster = new Monster(*animal);
        freedom.addAnimal(monster, 0);
        this->kill_all();
    }

    template <typename U>
    void removeSubstance(int pos, Aquarium<U>& otherAquarium) {
        if (pos < 0 || pos >= this->aquarium.size()) {
            cout << "Animal not found\n";
            return;
        }

        T* animal = this->aquarium[pos];
        U* normalAnimal = new U(animal->getName(), animal->getDaysLived()*2);
        otherAquarium.addAnimal(normalAnimal, 0);
        this->aquarium.erase(this->aquarium.begin() + pos);
        delete animal;
    }

    void kill_all() {
        // Deleting all animals in the aquarium
        for (auto* animal : this->aquarium) {
            delete animal;
        }
        this->aquarium.clear();
    }

    void sortAquarium() {
        sort(this->aquarium.begin(), this->aquarium.end(), [](T* a, T* b) {
            if (a->getDaysLived() != b->getDaysLived()) {
                return a->getDaysLived() < b->getDaysLived();
            }
            return a->getName() < b->getName();
        });
    }
};


template <>
class Cage<Fish> {
protected:
    vector<Fish*> cage;
public:
    Cage() = delete;
    Cage(const Cage<Fish>& other) = delete;
};


template <>
class Aquarium<Bird> {
protected:
    vector<Bird*> cage;
public:
    Aquarium() = delete;
    Aquarium(const Aquarium<Bird>& other) = delete;
};


class Simulation {
private:
    Cage<Bird>* cageBirds;
    Cage<BetterBird>* cageBetterBirds;
    Cage<Mouse>* cageMouses;
    Cage<BetterMouse>* cageBetterMouses;
    Aquarium<Fish>* aquariumFish;
    Aquarium<BetterFish>* aquariumBetterFish;
    Aquarium<Mouse>* aquariumMouses;
    Aquarium<BetterMouse>* aquariumBetterMouses;
    Freedom<Animal>* freedom;
public:
    Simulation() {
        cageBirds = new Cage<Bird>();
        cageBetterBirds = new Cage<BetterBird>();
        cageMouses = new Cage<Mouse>();
        cageBetterMouses = new Cage<BetterMouse>();
        aquariumFish = new Aquarium<Fish>();
        aquariumBetterFish = new Aquarium<BetterFish>();
        aquariumMouses = new Aquarium<Mouse>();
        aquariumBetterMouses = new Aquarium<BetterMouse>();
        freedom = new Freedom<Animal>();
    }
    ~Simulation() {
        delete cageBirds;
        delete cageBetterBirds;
        delete cageMouses;
        delete cageBetterMouses;
        delete aquariumFish;
        delete aquariumBetterFish;
        delete aquariumMouses;
        delete aquariumBetterMouses;
        delete freedom;
    }

    void create() {
        int n;
        string type, name, in, container;
        cin >> type >> name >> in >> container >> n;
        // Mouse
        if (type == "M") {
            // Cage
            if (container[0] == 'C') {
                cageMouses->addAnimal(new Mouse(name, n), 1);
            }
            // Aquarium
            else if (container[0] == 'A') {
                aquariumMouses->addAnimal(new Mouse(name, n), 1);
            }
            // Freedom
            else if (container[0] == 'F') {
                freedom->addAnimal(new Mouse(name, n), 1);
            }
        }
        // Better Mouse
        else if (type == "BM") {
            // Cage
            if (container[0] == 'C') {
                cageBetterMouses->addAnimal(new BetterMouse(name, n), 1);
            }
            // Aquarium
            else if (container[0] == 'A') {
                aquariumBetterMouses->addAnimal(new BetterMouse(name, n), 1);
            }
            // Freedom
            else if (container[0] == 'F') {
                freedom->addAnimal(new BetterMouse(name, n), 1);
            }
        }
        // Fish
        else if (type == "F") {
            // Aquarium
            if (container[0] == 'A') {
                aquariumFish->addAnimal(new Fish(name, n), 1);
            }
            // Freedom
            else if (container[0] == 'F') {
                freedom->addAnimal(new Fish(name, n), 1);
            }
        }
        // Better Fish
        else if (type == "BF") {
            // Aquarium
            if (container[0] == 'A') {
                aquariumBetterFish->addAnimal(new BetterFish(name, n), 1);
            }
            // Freedom
            else if (container[0] == 'F') {
                freedom->addAnimal(new BetterFish(name, n), 1);
            }
        }
        // Bird
        else if (type == "B") {
            // Cage
            if (container[0] == 'C') {
                cageBirds->addAnimal(new Bird(name, n), 1);
            }
            // Freedom
            else if (container[0] == 'F') {
                freedom->addAnimal(new Bird(name, n), 1);
            }
        }
        // Better Bird
        else if (type == "BB") {
            // Cage
            if (container[0] == 'C') {
                cageBetterBirds->addAnimal(new BetterBird(name, n), 1);
            }
            // Freedom
            else if (container[0] == 'F') {
                freedom->addAnimal(new BetterBird(name, n), 1);
            }
        }
    }

    void applySubstance() {
        int pos;
        string type, container;
        cin >> container;

        if (container[0] == 'F') {
            cin >> pos;
            cout << "Substance cannot be applied in freedom\n";
            return;
        }
        cin >> type >> pos;
        // Mouse
        if (type == "M") {
            // Cage
            if (container[0] == 'C') {
                cageMouses->applySubstance(pos, *cageBetterMouses);
            }
            // Aquarium
            else if (container[0] == 'A') {
                aquariumMouses->applySubstance(pos, *aquariumBetterMouses);
            }
        }
        // Better Mouse
        else if (type == "BM") {
            // Cage
            if (container[0] == 'C') {
                cageBetterMouses->applySubstance(pos, *freedom);
            }
            // Aquarium
            else if (container[0] == 'A') {
                aquariumBetterMouses->applySubstance(pos, *freedom);
            }
        }
        // Fish
        else if (type == "F") {
            // Aquarium
            if (container[0] == 'A') {
                aquariumFish->applySubstance(pos, *aquariumBetterFish);
            }
        }
        // Better Fish
        else if (type == "BF") {
            // Aquarium
            if (container[0] == 'A') {
                aquariumBetterFish->applySubstance(pos, *freedom);
            }
        }
        // Bird
        else if (type == "B") {
            // Cage
            if (container[0] == 'C') {
                cageBirds->applySubstance(pos, *cageBetterBirds);
            }
        }
        // Better Bird
        else if (type == "BB") {
            // Cage
            if (container[0] == 'C') {
                cageBetterBirds->applySubstance(pos, *freedom);
            }
        }
    }

    void removeSubstance() {
        int pos;
        string type, container;
        cin >> container;

        if (container[0] == 'F') {
            cin >> pos;
            cout << "Substance cannot be removed in freedom\n";
            return;
        }
        cin >> type >> pos;
        // Mouse
        if (type == "M") {
            cout << "Invalid substance removal\n";
            return;
        }
        // Better Mouse
        else if (type == "BM") {
            // Cage
            if (container[0] == 'C') {
                cageBetterMouses->removeSubstance(pos, *cageMouses);
            }
            // Aquarium
            else if (container[0] == 'A') {
                aquariumBetterMouses->removeSubstance(pos, *aquariumMouses);
            }
        }
        // Fish
        else if (type == "F") {
            cout << "Invalid substance removal\n";
            return;
        }
        // Better Fish
        else if (type == "BF") {
            // Aquarium
            if (container[0] == 'A') {
                aquariumBetterFish->removeSubstance(pos, *aquariumFish);
            }
        }
        // Bird
        else if (type == "B") {
            cout << "Invalid substance removal\n";
            return;
        }
        // Better Bird
        else if (type == "BB") {
            // Cage
            if (container[0] == 'C') {
                cageBetterBirds->removeSubstance(pos, *cageBirds);
            }
        }
    }

    void attack() {
        int pos1, pos2;
        string type, container;
        cin >> container;

        if (container[0] == 'F') {
            cin >> pos1 >> pos2;
            cout << "Animals cannot attack in Freedom\n";
            return;
        }
        cin >> type >> pos1 >> pos2;
        // Mouse
        if (type == "M") {
            // Cage
            if (container[0] == 'C') {
                cageMouses->attack(pos1, pos2);
            }
            // Aquarium
            else if (container[0] == 'A') {
                aquariumMouses->attack(pos1, pos2);
            }
        }
        // Better Mouse
        else if (type == "BM") {
            // Cage
            if (container[0] == 'C') {
                cageBetterMouses->attack(pos1, pos2);
            }
            // Aquarium
            else if (container[0] == 'A') {
                aquariumBetterMouses->attack(pos1, pos2);
            }
        }
        // Fish
        else if (type == "F") {
            // Aquarium
            if (container[0] == 'A') {
                aquariumFish->attack(pos1, pos2);
            }
        }
        // Better Fish
        else if (type == "BF") {
            // Aquarium
            if (container[0] == 'A') {
                aquariumBetterFish->attack(pos1, pos2);
            }
        }
        // Bird
        else if (type == "B") {
            // Cage
            if (container[0] == 'C') {
                cageBirds->attack(pos1, pos2);
            }
        }
        // Better Bird
        else if (type == "BB") {
            // Cage
            if (container[0] == 'C') {
                cageBetterBirds->attack(pos1, pos2);
            }
        }
    }

    void talk() {
        int pos;
        string type, container;
        cin >> container;
        // Freedom
        if (container[0] == 'F') {
            cin >> pos;
            freedom->talk(pos);
            return;
        }
        cin >> type >> pos;
        
        // Mouse
        if (type == "M") {
            // Cage
            if (container[0] == 'C') {
                cageMouses->talk(pos);
            }
            // Aquarium
            else if (container[0] == 'A') {
                aquariumMouses->talk(pos);
            }
        }
        // Better Mouse
        else if (type == "BM") {
            // Cage
            if (container[0] == 'C') {
                cageBetterMouses->talk(pos);
            }
            // Aquarium
            else if (container[0] == 'A') {
                aquariumBetterMouses->talk(pos);
            }
        }
        // Fish
        else if (type == "F") {
            // Aquarium
            if (container[0] == 'A') {
                aquariumFish->talk(pos);
            }
        }
        // Better Fish
        else if (type == "BF") {
            // Aquarium
            if (container[0] == 'A') {
                aquariumBetterFish->talk(pos);
            }
        }
        // Bird
        else if (type == "B") {
            // Cage
            if (container[0] == 'C') {
                cageBirds->talk(pos);
            }
        }
        // Better Bird
        else if (type == "BB") {
            // Cage
            if (container[0] == 'C') {
                cageBetterBirds->talk(pos);
            }
        }
    }

    void period() {
        // Adding +1 day to all animals in the Containers and sorting by days lived
        cageBirds->period();
        cageBetterBirds->period();
        cageMouses->period();
        cageBetterMouses->period();
        aquariumFish->period();
        aquariumBetterFish->period();
        aquariumMouses->period();
        aquariumBetterMouses->period();
        freedom->period();
    }
};


int main() {
    int numberOfOperations;
    cin >> numberOfOperations;

    Simulation simulation;

    for (int i = 0; i < numberOfOperations; i++) {
        string operation;
        cin >> operation;
        if (operation == "CREATE") { simulation.create();}
        else if (operation == "APPLY_SUBSTANCE") { simulation.applySubstance();}
        else if (operation == "REMOVE_SUBSTANCE") { simulation.removeSubstance();}
        else if (operation == "ATTACK") { simulation.attack();}
        else if (operation == "TALK") { simulation.talk();}
        else if (operation == "PERIOD") { simulation.period();}
    }
    return 0;
}