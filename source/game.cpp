#include "../include/game.h"
#include "../include/state.h"
#include "../include/menustate.h"

using namespace std;

Game::Game() {
    //Creating our window
    app.create(sf::VideoMode(1440, 900), "Dungeon Generator");

    //adjust the view to center on map
    sf::View view = app.getView();
    view.setCenter(0.f, 0.f);
    view.zoom(1.0);
    app.setView(view);
    targetfps = 60;
    app.setFramerateLimit(targetfps);
    vsync = true;
    app.setVerticalSyncEnabled(vsync);

    d = nullptr;
}

sf::RenderWindow *Game::getWindow() {
    return &app;
}

Dungeon *Game::getDungeon() {
    return d;
}

void Game::setDungeon(Dungeon *dung) {
    d = dung;
}

void Game::changeState(State *state) {
    delete states.top();
    states.pop();

    states.push(state);
}

void Game::pushState(State *state) {
    states.push(state);
}

void Game::popState() {
    delete states.top();
    states.pop();
}

//TODO: Flesh out in a full menu class
void Game::menu() {
    //State version
    states.push(new MenuState());

    while (!states.empty() && app.isOpen()) {
        states.top()->handleEvents(this);
        states.top()->update(this);
        states.top()->render(this);

        if (states.top()->toChange != nullptr)
            changeState(states.top()->toChange);

        if (states.top()->toKill)
            popState();
    }
}

Game::~Game() {
    while (!states.empty()) {
        delete states.top();
        states.pop();
    }
    d = nullptr;
}