#ifndef GAME_H
#define GAME_H


#include "player.h"
#include "item.h"
#include "equipment.h"
#include "npc.h"
#include "dungeon.h"
#include "map.h"

#include <sstream>
#include <SFML/Audio.hpp>

class Game {
public:
    Game();

    void loop();

    virtual ~Game();

protected:
private:
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


    Dungeon d;
    Player player;
    vector<Entity *> entities;
    vector<Actor *> actors;

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
    sf::Font font;
    sf::Clock clk;

    sf::Time elapsed;
};

#endif // GAME_H
