#ifndef ITEM_H
#define ITEM_H

#include "entity.h"

#include <string>

using std::string;

class Item : public Entity {
public:
    Item(sf::Vector2f, string, string, int);

    virtual ~Item();

protected:
private:
    string name;
    int uses;
};

#endif // ITEM_H
