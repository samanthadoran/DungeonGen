//
// Created by Doran on 7/9/2015.
//
#ifndef DUNGEONGEN_STATE_H
#define DUNGEONGEN_STATE_H

#include "game.h"

//Abstract class to hold various game states

class State {
public:
    virtual void update(Game *) = 0;

    virtual void render(Game *) = 0;

    void changeState(Game *, State *);
};

#endif //DUNGEONGEN_STATE_H
