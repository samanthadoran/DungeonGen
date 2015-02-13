#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "item.h"


class Equipment : public Item {
public:
    Equipment(sf::Vector2f, string, string);

    virtual ~Equipment();

protected:
private:
};

#endif // EQUIPMENT_H
