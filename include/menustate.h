//
// Created by Doran on 7/9/2015.
//

#ifndef DUNGEONGEN_MENUSTATE_H
#define DUNGEONGEN_MENUSTATE_H

#include "state.h"
#include "playstate.h"
class Game;

class MenuState : public State {
public:
    MenuState();

    void update(Game *);

    void render(Game *);

    void handleEvents(Game *);

private:
    bool select(Game *);

    int selection;
    sf::Font font;
    vector<sf::Text> choices;
};

#endif //DUNGEONGEN_MENUSTATE_H