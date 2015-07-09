#ifndef GAME_H
#define GAME_H


#include "player.h"
#include "item.h"
#include "equipment.h"
#include "npc.h"
#include "dungeon.h"
#include "map.h"
#include "state.h"

#include <sstream>
#include <stack>
#include <SFML/Audio.hpp>

class Game {
public:
    Game();

    void menu();

    virtual ~Game();

protected:
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
    stack<State> states;

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

#endif // GAME_H