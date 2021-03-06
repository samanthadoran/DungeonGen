//
// Created by Doran on 7/10/2015.
//

#ifndef DUNGEONGEN_PLAYSTATE_H
#define DUNGEONGEN_PLAYSTATE_H

#include "state.h"

class Game;

class PlayState : public State {
public:
    PlayState(Game *, Dungeon *);

    void update(Game *) override;

    void render(Game *) override;

    void handleEvents(Game *) override;

    ~PlayState();

private:
    sf::Vector2f tileToWorldCoord(sf::Vector2i) const;

    sf::Vector2i worldToTileCoord(sf::Vector2f) const;

    sf::Vector2i worldToTileCoord(sf::Vector2i) const;

    vector<Actor *> getTileActors(Tile *);

    void initiateActors(int);

    void cleanupActors();

    void changeFloor(Game*, int);

    void checkActorLife();

    void dungeonChange(Game *);

    void pickupItems(Player *, const Tile *);

    const Tile *const selectTile(Game *);

    void drawShadows(Entity *, Game *);

    void drawMinimap(Game *, sf::Texture &);

    void drawTileOutline(Tile *t, Game *);

    void drawInventoryOfActor(Actor *, Game *);

    void showText(string, sf::Vector2f, Game *);

    string updateDebug();

    Tile *selectedTile;
    int floor;
    bool gameOver;

    Dungeon *d;
    Player *player;
    vector<Actor *> actors;

    sf::Vector2f collisions(sf::Rect<float>, sf::Vector2f) const;

    void runTileEvent(Game *, Player *);

    bool focus;
    bool debug;

    bool paused;

    bool vsync;
    unsigned int targetfps;

    int score;

    //Debug info
    string debugText;
    sf::Font font;
    sf::Clock clk;
    sf::Clock textclk;

    sf::Time elapsed;
};

#endif //DUNGEONGEN_PLAYSTATE_H
