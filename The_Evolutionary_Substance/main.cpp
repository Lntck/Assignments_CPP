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


class BetterFish : public virtual Fish {
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


class BetterBird : public virtual Bird {
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


class BetterMouse : public virtual Mouse {
public:
    BetterMouse(const string name, int daysLived) : Animal(name, daysLived), Mouse(name, daysLived) {}
    BetterMouse(Mouse& other) : Animal(other), Mouse(other) {}

    void attack(Animal& other) override {
        cout << "BetterMouse is attacking\n";
    }
};
    

class Monster : public virtual BetterFish, public virtual BetterBird, public virtual BetterMouse {
public:
    Monster(string name) : Animal(name, 0), Fish(name, 0), Bird(name, 0), Mouse(name, 0), BetterFish(name, 0), BetterBird(name, 0), BetterMouse(name, 0) {}
    Monster(Animal& other) : Animal(other.getName(), 0), Fish(other.getName(), 0), Bird(other.getName(), 0), Mouse(other.getName(), 0), BetterFish(other.getName(), 0), BetterBird(other.getName(), 0), BetterMouse(other.getName(), 0) {}

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

    void addAnimal(T* animal, int flag) {
        this->freedom.push_back(animal);
        if (flag) { animal->sayName();}
        this->sortFreedom();
    }

    void period() {
        vector<int> deleteList;
        
        for (int i = 0; i < this->freedom.size(); i++) {
            this->freedom[i]->setDaysLived(this->freedom[i]->getDaysLived() + 1);
            if (this->freedom[i]->getDaysLived() > 10) {
                deleteList.push_back(i);
            }
            if (dynamic_cast<Monster*>(this->freedom[i]) && this->freedom[i]->getDaysLived() > 1) {
                cout << "WORKS!\n";
                deleteList.push_back(i);
            }
        }

        for (int i = deleteList.size() - 1; i >= 0; i--) {
            cout << this->freedom[deleteList[i]]->getName() << " has died of old days\n";
            delete this->freedom[deleteList[i]];
            this->freedom.erase(this->freedom.begin() + deleteList[i]);
        }
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
            return a->getDaysLived() < b->getDaysLived();
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

    void addAnimal(T* animal, int flag) {
        this->cage.push_back(animal);
        if (flag) { animal->sayName();}
        this->sortCage();
    }

    void period() {
        vector<int> deleteList;

        for (int i = 0; i < this->cage.size(); i++) {
            this->cage[i]->setDaysLived(this->cage[i]->getDaysLived() + 1);
            if (this->cage[i]->getDaysLived() > 10) {
                deleteList.push_back(i);
            }
        }

        for (int i = deleteList.size() - 1; i >= 0; i--) {
            cout << this->cage[deleteList[i]]->getName() << " has died of old days\n";
            delete this->cage[deleteList[i]];
            this->cage.erase(this->cage.begin() + deleteList[i]);
        }
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
        this->cage.erase(this->cage.begin() + index2);
        delete this->cage[index2];
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

    void kill_all() {
        // Deleting all animals in the cage
        for (auto* animal : this->cage) {
            delete animal;
        }
        this->cage.clear();
    }

    void sortCage() {
        sort(this->cage.begin(), this->cage.end(), [](T* a, T* b) {
            return a->getDaysLived() < b->getDaysLived();
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

    void addAnimal(T* animal, int flag) {
        this->aquarium.push_back(animal);
        if (flag) { animal->sayName();}
        this->sortAquarium();
    }

    void period() {
        vector<int> deleteList;
        
        for (int i = 0; i < this->aquarium.size(); i++) {
            this->aquarium[i]->setDaysLived(this->aquarium[i]->getDaysLived() + 1);
            if (this->aquarium[i]->getDaysLived() > 10) {
                deleteList.push_back(i);
            }
        }

        for (int i = deleteList.size() - 1; i >= 0; i--) {
            cout << this->aquarium[deleteList[i]]->getName() << " has died of old days\n";
            delete this->aquarium[deleteList[i]];
            this->aquarium.erase(this->aquarium.begin() + deleteList[i]);
        }
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
        this->aquarium.erase(this->aquarium.begin() + index2);
        delete this->aquarium[index2];
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

    void kill_all() {
        // Deleting all animals in the aquarium
        for (auto* animal : this->aquarium) {
            delete animal;
        }
        this->aquarium.clear();
    }

    void sortAquarium() {
        sort(this->aquarium.begin(), this->aquarium.end(), [](T* a, T* b) {
            return a->getDaysLived() < b->getDaysLived();
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


int main() {
    int numberOfOperations;
    cin >> numberOfOperations;

    Cage<Bird> cageBirds;
    Cage<BetterBird> cageBetterBirds;
    Cage<Mouse> cageMouses;
    Cage<BetterMouse> cageBetterMouses;
    Aquarium<Fish> aquariumFish;
    Aquarium<BetterFish> aquariumBetterFish;
    Aquarium<Mouse> aquariumMouses;
    Aquarium<BetterMouse> aquariumBetterMouses;
    Freedom<Animal> freedom;

    for (int i = 0; i < numberOfOperations; i++) {
        string operation;
        cin >> operation;
        switch(operation[2]) {
            // CREATE
            case 'E':
                {   
                    int n;
                    string type, name, in, container;
                    cin >> type >> name >> in >> container >> n;
                    // Mouse
                    if (type == "M") {
                        // Cage
                        if (container[0] == 'C') {
                            cageMouses.addAnimal(new Mouse(name, n), 1);
                        }
                        // Aquarium
                        else if (container[0] == 'A') {
                            aquariumMouses.addAnimal(new Mouse(name, n), 1);
                        }
                        // Freedom
                        else if (container[0] == 'F') {
                            freedom.addAnimal(new Mouse(name, n), 1);
                        }
                    }
                    // Better Mouse
                    else if (type == "BM") {
                        // Cage
                        if (container[0] == 'C') {
                            cageBetterMouses.addAnimal(new BetterMouse(name, n), 1);
                        }
                        // Aquarium
                        else if (container[0] == 'A') {
                            aquariumBetterMouses.addAnimal(new BetterMouse(name, n), 1);
                        }
                        // Freedom
                        else if (container[0] == 'F') {
                            freedom.addAnimal(new BetterMouse(name, n), 1);
                        }
                    }
                    // Fish
                    else if (type == "F") {
                        // Aquarium
                        if (container[0] == 'A') {
                            aquariumFish.addAnimal(new Fish(name, n), 1);
                        }
                        // Freedom
                        else if (container[0] == 'F') {
                            freedom.addAnimal(new Fish(name, n), 1);
                        }
                    }
                    // Better Fish
                    else if (type == "BF") {
                        // Aquarium
                        if (container[0] == 'A') {
                            aquariumBetterFish.addAnimal(new BetterFish(name, n), 1);
                        }
                        // Freedom
                        else if (container[0] == 'F') {
                            freedom.addAnimal(new BetterFish(name, n), 1);
                        }
                    }
                    // Bird
                    else if (type == "B") {
                        // Cage
                        if (container[0] == 'C') {
                            cageBirds.addAnimal(new Bird(name, n), 1);
                        }
                        // Freedom
                        else if (container[0] == 'F') {
                            freedom.addAnimal(new Bird(name, n), 1);
                        }
                    }
                    // Better Bird
                    else if (type == "BB") {
                        // Cage
                        if (container[0] == 'C') {
                            cageBetterBirds.addAnimal(new BetterBird(name, n), 1);
                        }
                        // Freedom
                        else if (container[0] == 'F') {
                            freedom.addAnimal(new BetterBird(name, n), 1);
                        }
                    }
                    else { return 0; }
                    break;
                }
            // APPLY_SUBSTANCE
            case 'P':
                {
                    int pos;
                    string type, container;
                    cin >> container >> type >> pos;

                    if (container[0] == 'F') {
                        cout << "Substance cannot be applied in freedom\n";
                        break;
                    }
                    // Mouse
                    if (type == "M") {
                        // Cage
                        if (container[0] == 'C') {
                            cageMouses.applySubstance(pos, cageBetterMouses);
                        }
                        // Aquarium
                        else if (container[0] == 'A') {
                            aquariumMouses.applySubstance(pos, aquariumBetterMouses);
                        }
                    }
                    // Better Mouse
                    else if (type == "BM") {
                        // Cage
                        if (container[0] == 'C') {
                            cageBetterMouses.applySubstance(pos, freedom);
                        }
                        // Aquarium
                        else if (container[0] == 'A') {
                            aquariumBetterMouses.applySubstance(pos, freedom);
                        }
                    }
                    // Fish
                    else if (type == "F") {
                        // Aquarium
                        if (container[0] == 'A') {
                            aquariumFish.applySubstance(pos, aquariumBetterFish);
                        }
                    }
                    // Better Fish
                    else if (type == "BF") {
                        // Aquarium
                        if (container[0] == 'A') {
                            aquariumBetterFish.applySubstance(pos, freedom);
                        }
                    }
                    // Bird
                    else if (type == "B") {
                        // Cage
                        if (container[0] == 'C') {
                            cageBirds.applySubstance(pos, cageBetterBirds);
                        }
                    }
                    // Better Bird
                    else if (type == "BB") {
                        // Cage
                        if (container[0] == 'C') {
                            cageBetterBirds.applySubstance(pos, freedom);
                        }
                    }
                    else { return 0; }
                    break;


                    break;
                }
            // REMOVE_SUBSTANCE
            case 'M':
                {
                    int pos;
                    string type, container;
                    cin >> container >> type >> pos;

                    if (container[0] == 'F') {
                        cout << "Animals cannot attack in Freedom\n";
                        break;
                    }

                    break;
                }
            // ATTACK
            case 'T':
                {
                    int pos1, pos2;
                    string type, container;
                    cin >> container >> type >> pos1 >> pos2;

                    if (container[0] == 'F') {
                        cout << "Animals cannot attack in Freedom\n";
                        break;
                    }
                    // Mouse
                    if (type == "M") {
                        // Cage
                        if (container[0] == 'C') {
                            cageMouses.attack(pos1, pos2);
                        }
                        // Aquarium
                        else if (container[0] == 'A') {
                            aquariumMouses.attack(pos1, pos2);
                        }
                    }
                    // Better Mouse
                    else if (type == "BM") {
                        // Cage
                        if (container[0] == 'C') {
                            cageBetterMouses.attack(pos1, pos2);
                        }
                        // Aquarium
                        else if (container[0] == 'A') {
                            aquariumBetterMouses.attack(pos1, pos2);
                        }
                    }
                    // Fish
                    else if (type == "F") {
                        // Aquarium
                        if (container[0] == 'A') {
                            aquariumFish.attack(pos1, pos2);
                        }
                    }
                    // Better Fish
                    else if (type == "BF") {
                        // Aquarium
                        if (container[0] == 'A') {
                            aquariumBetterFish.attack(pos1, pos2);
                        }
                    }
                    // Bird
                    else if (type == "B") {
                        // Cage
                        if (container[0] == 'C') {
                            cageBirds.attack(pos1, pos2);
                        }
                    }
                    // Better Bird
                    else if (type == "BB") {
                        // Cage
                        if (container[0] == 'C') {
                            cageBetterBirds.attack(pos1, pos2);
                        }
                    }
                    else { return 0; }
                    break;
                }
            // TALK
            case 'L':
                {
                    int pos;
                    string type, container;
                    cin >> container >> type >> pos;
                    
                    // Mouse
                    if (type == "M") {
                        // Cage
                        if (container[0] == 'C') {
                            cageMouses.talk(pos);
                        }
                        // Aquarium
                        else if (container[0] == 'A') {
                            aquariumMouses.talk(pos);
                        }
                        // Freedom
                        else if (container[0] == 'F') {
                            freedom.talk(pos);
                        }
                    }
                    // Better Mouse
                    else if (type == "BM") {
                        // Cage
                        if (container[0] == 'C') {
                            cageBetterMouses.talk(pos);
                        }
                        // Aquarium
                        else if (container[0] == 'A') {
                            aquariumBetterMouses.talk(pos);
                        }
                        // Freedom
                        else if (container[0] == 'F') {
                            freedom.talk(pos);
                        }
                    }
                    // Fish
                    else if (type == "F") {
                        // Aquarium
                        if (container[0] == 'A') {
                            aquariumFish.talk(pos);
                        }
                        // Freedom
                        else if (container[0] == 'F') {
                            freedom.talk(pos);
                        }
                    }
                    // Better Fish
                    else if (type == "BF") {
                        // Aquarium
                        if (container[0] == 'A') {
                            aquariumBetterFish.talk(pos);
                        }
                        // Freedom
                        else if (container[0] == 'F') {
                            freedom.talk(pos);
                        }
                    }
                    // Bird
                    else if (type == "B") {
                        // Cage
                        if (container[0] == 'C') {
                            cageBirds.talk(pos);
                        }
                        // Freedom
                        else if (container[0] == 'F') {
                            freedom.talk(pos);
                        }
                    }
                    // Better Bird
                    else if (type == "BB") {
                        // Cage
                        if (container[0] == 'C') {
                            cageBetterBirds.talk(pos);
                        }
                        // Freedom
                        else if (container[0] == 'F') {
                            freedom.talk(pos);
                        }
                    }
                    else { return 0; }
                    break;
                }
            // PERIOD
            case 'R':
                {
                    // Adding +1 day to all animals in the Containers and sorting by days lived
                    cageBirds.period();
                    cageBirds.sortCage();
                    cageBetterBirds.period();
                    cageBetterBirds.sortCage();
                    cageMouses.period();
                    cageMouses.sortCage();
                    cageBetterMouses.period();
                    cageBetterMouses.sortCage();
                    aquariumFish.period();
                    aquariumFish.sortAquarium();
                    aquariumBetterFish.period();
                    aquariumBetterFish.sortAquarium();
                    aquariumMouses.period();
                    aquariumMouses.sortAquarium();
                    aquariumBetterMouses.period();
                    aquariumBetterMouses.sortAquarium();
                    freedom.period();
                    freedom.sortFreedom();
                    break;
                }
            // PRINT
            case 'I':
                {
                    cout << "Mouses\n";
                    cageMouses.print();
                    cout << "Better Mouses\n";
                    cageBetterMouses.print();
                    cout << "Fish\n";
                    aquariumFish.print();
                    cout << "Better Fish\n";
                    aquariumBetterFish.print();
                    cout << "Birds\n";
                    cageBirds.print();
                    cout << "Better Birds\n";
                    cageBetterBirds.print();
                    cout << "Freedom\n";
                    freedom.print();
                    break;
                }
        }
    }

    return 0;
}