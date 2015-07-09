//
// Created by Doran on 7/9/2015.
//

#ifndef DUNGEONGEN_MENUSTATE_H
#define DUNGEONGEN_MENUSTATE_H

#include "state.h"

class Game;

class MenuState : public State {
public:
    MenuState();

    void update(Game *);

    void render(Game *);

private:
    sf::Font font;
    vector<sf::Text> choices;
};

#endif //DUNGEONGEN_MENUSTATE_H
