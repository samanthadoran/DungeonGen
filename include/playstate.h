//
// Created by Doran on 7/10/2015.
//

#ifndef DUNGEONGEN_PLAYSTATE_H
#define DUNGEONGEN_PLAYSTATE_H

#include "state.h"

class Game;

class PlayState : public State {
public:
    PlayState(Dungeon *);

    void update(Game *);

    void render(Game *);

    void handleEvents(Game *);

private:
    void init();

    void update();

    void render();

    void loop();

    sf::Vector2f tileToWorldCoord(sf::Vector2i) const;

    sf::Vector2i worldToTileCoord(sf::Vector2f) const;

    sf::Vector2i worldToTileCoord(sf::Vector2i) const;

    Actor *getTileActors(Tile *);

    const Tile *const selectTile();

    void drawShadows(Entity *);

    void drawMinimap();

    void drawTileOutline(Tile *t);

    void showText(string, sf::Vector2f);

    string updateDebug();

    Tile *selectedTile;
    int floor;
    bool gameOver;

    Dungeon *d;
    Player *player;
    vector<Actor *> actors;
    stack<State *> states;

    void runEvents();

    sf::Vector2f collisions(sf::Rect<float>, sf::Vector2f) const;

    void runTileEvent(Player *);

    unsigned int targetfps;
    bool vsync;
    bool focus;
    bool debug;

    sf::RenderWindow app;
    sf::Music music;

    //Debug info
    string debugText;
    sf::Font font;
    sf::Clock clk;
    sf::Clock textclk;

    sf::Time elapsed;
};

#endif //DUNGEONGEN_PLAYSTATE_H
