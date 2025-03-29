#include <iostream>
#include <string>

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
        cout << this->name << "\n";
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
    BetterFish(const string name, int daysLived) : Fish(name, daysLived) {}
    BetterFish(Fish& other) : Fish(other) {}

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
    BetterBird(const string name, int daysLived) : Bird(name, daysLived) {}
    BetterBird(Bird& other) : Bird(other) {}

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
    BetterMouse(const string name, int daysLived) : Mouse(name, daysLived) {}
    BetterMouse(Mouse& other) : Mouse(other) {}

    void attack(Animal& other) override {
        cout << "BetterMouse is attacking\n";
    }
};
    

class Monster : public virtual BetterFish, public virtual BetterBird, public virtual BetterMouse{
public:
    Monster(string name) : Animal(name, 0), Fish(name, 0), Bird(name, 0), Mouse(name, 0), BetterFish(name, 0), BetterBird(name, 0), BetterMouse(name, 0) {}
    Monster(Animal& other) : Animal(other), Fish(other.getName(), other.getDaysLived()), Bird(other.getName(), other.getDaysLived()), Mouse(other.getName(), other.getDaysLived()), BetterFish(other.getName(), other.getDaysLived()), BetterBird(other.getName(), other.getDaysLived()), BetterMouse(other.getName(), other.getDaysLived()) {}

    void attack(Animal& other) override {
        cout << "Monster is attacking\n";
    }
};


int main() {
    Fish fish("Nemo", 0);
    return 0;
}
