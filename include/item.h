#ifndef ITEM_H
#define ITEM_H


#include <string>

using std::string;

class Item {
public:
    Item(string, int);

    //void use(Entity *);

    virtual ~Item();

protected:
private:
    string name;
    int uses;
};

#endif // ITEM_H
