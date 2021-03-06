#ifndef GAME_H
#define GAME_H


#include "player.h"
#include "weapon.h"
#include "item.h"
#include "equipment.h"
#include "npc.h"
#include "dungeon.h"
#include "map.h"

#include <sstream>
#include <stack>
#include <SFML/Audio.hpp>

class State;

class Game {
public:
    Game();

    void menu();

    void changeState(State *);

    void pushState(State *);

    void popState();

    Dungeon *getDungeon();

    void setDungeon(Dungeon *);

    sf::RenderWindow *getWindow();

    virtual ~Game();

protected:
private:
    Dungeon *d;

    stack<State *> states;

    unsigned int targetfps;
    bool vsync;

    sf::RenderWindow app;
};

#endif // GAME_H