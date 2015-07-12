//
// Created by Doran on 7/9/2015.
//
#ifndef DUNGEONGEN_STATE_H
#define DUNGEONGEN_STATE_H

#include "game.h"

//Abstract class to hold various game states

class State {
public:
    State();

    virtual void update(Game *) = 0;

    virtual void render(Game *) = 0;

    virtual void handleEvents(Game *) = 0;

    virtual ~State();

    State *toChange;

    bool toKill;
};

#endif //DUNGEONGEN_STATE_H
