//
// Created by Sammi on 7/12/2015.
//
#include "item.h"

#ifndef DUNGEONGEN_WEAPON_H
#define DUNGEONGEN_WEAPON_H

class Weapon : public Item {
public:
    Weapon(sf::Vector2f, int, string, string, int, int);
};

#endif //DUNGEONGEN_WEAPON_H
