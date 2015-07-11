//
// Created by Doran on 7/10/2015.
//

#include "../include/playstate.h"

PlayState::PlayState(Dungeon *dungeon) {
    d = dungeon;

    gameOver = false;
    selectedTile = nullptr;
    floor = 0;

    //Music is broken until update to sfml 2.3
    //music.openFromFile("music.ogg");
    //music.setLoop(true);

    //Load font for debugging
    font.loadFromFile("Minecraftia.ttf");

    targetfps = 60;
    vsync = true;

    //to toggle debug output
    debug = true;
    focus = true;

    //Test actors
    actors.emplace_back(new Player(tileToWorldCoord(d->getFloor(floor)->getStairsUpSpawn()), 160, "link.png"));
    actors.emplace_back(new NPC(tileToWorldCoord(d->getFloor(floor)->getStairsUpSpawn()), "link.png"));

    player = dynamic_cast<Player *>(actors.front());
    dynamic_cast<NPC *>(actors.back())->setTarget(player);

    actors.back()->addItem(new Item(sf::Vector2f(-1, -1), 1, "link.png", "claws", -1, 200));
    player->addItem(new Item(sf::Vector2f(-1, -1), 10, "link.png", "sword", -1, 200));

    //music.setVolume(20);
    //music.play();
}

//Returns a modified vector to obey collision models
sf::Vector2f PlayState::collisions(sf::Rect<float> test, sf::Vector2f movement) const {
    sf::Rect<float> future = test;

    sf::Vector2f futurePos;

    future.left += movement.x * elapsed.asSeconds();
    futurePos = sf::Vector2f(future.left, future.top);

    //You can't walk where the dungeon isn't
    if (!d->getFloor(floor)->inBounds(futurePos))
        movement.x = 0;

    //Don't walk through walls
    for (auto row: *d->getFloor(floor)->getMap())
        for (auto tile: row)
            if (tile->getTileType() ==
                TileType::WALL) if (tile->getGraphicalRepresentation().getGlobalBounds().intersects(future))
                movement.x = 0;

    future = test;
    future.top += movement.y * elapsed.asSeconds();
    futurePos = sf::Vector2f(future.left, future.top);

    //You still can't walk where the dungeon isn't
    if (!d->getFloor(floor)->inBounds(futurePos))
        movement.y = 0;

    //You still can't walk through walls
    for (auto row: *d->getFloor(floor)->getMap())
        for (auto tile: row)
            if (tile->getTileType() ==
                TileType::WALL) if (tile->getGraphicalRepresentation().getGlobalBounds().intersects(future))
                movement.y = 0;

    return movement;
}

//TODO: Add more types of tile events.
void PlayState::runTileEvent(Player *p) {
    const Tile *t = d->getFloor(floor)->getTileAtPos(worldToTileCoord(p->getPosition()));
    TileType type = t->getTileType();

    switch (type) {
        case TileType::STAIRS_UP:
            if (floor == 0)
                //app.close();
                gameOver = true;
            else {
                --floor;
                p->setPosition(tileToWorldCoord(d->getFloor(floor)->getStairsDownSpawn()));
            }
            break;
        case TileType::STAIRS_DOWN:
            if (d->getFloors().size() == floor + 1)
                d->addFloor();

            ++floor;
            p->setPosition(tileToWorldCoord(d->getFloor(floor)->getStairsUpSpawn()));
            break;
        default:
            break;
    }
}

void PlayState::handleEvents(Game *game) {
    sf::Event event;
    while (game->getWindow()->pollEvent(event)) {
        if (event.type == sf::Event::Resized) {
            // update the view to the new size of the window
            sf::FloatRect newView(0, 0, event.size.width, event.size.height);
            newView.left = game->getWindow()->getView().getCenter().x - game->getWindow()->getView().getSize().x / 2.0f;
            newView.top = game->getWindow()->getView().getCenter().y - game->getWindow()->getView().getSize().y / 2.0f;
            game->getWindow()->setView(sf::View(newView));
        }

        //We can control it!
        if (event.type == sf::Event::GainedFocus)
            focus = true;

        //Don't send inputs!
        if (event.type == sf::Event::LostFocus)
            focus = false;

        // Close window: exit
        if (event.type == sf::Event::Closed) {
            gameOver = true;
            game->getWindow()->close();
        }

        //Escape key: exit
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Escape:
                    gameOver = true;
                    game->getWindow()->close();
                    break;
                case sf::Keyboard::F1:
                    debug = !debug;
                    break;
                case sf::Keyboard::F5:
                    d->saveDungeon("default.txt");
                    break;
                case sf::Keyboard::F9:
                    delete d;
                    d = new Dungeon("default.txt");
                    floor = 0;
                    if (player == nullptr) {
                        actors.emplace(actors.begin(),
                                       new Player(tileToWorldCoord(d->getFloor(floor)->getStairsUpSpawn()), 160,
                                                  "link.png"));
                        player = dynamic_cast<Player *>(actors.front());
                    }
                    player->setPosition(tileToWorldCoord(d->getFloor(floor)->getStairsUpSpawn()));
                    break;
                case sf::Keyboard::M:
                    /*if (music.getVolume() == 0)
                        music.setVolume(20);
                    else
                        music.setVolume(0);*/
                    break;
                default:
                    break;
            }
        }
    }
}

void PlayState::update(Game *game) {
    if (gameOver) {
        player = nullptr;
        game->getWindow()->close();
        return;
    }

    elapsed = clk.restart();

    //Check for application events.
    handleEvents(game);

    for (auto i = actors.begin(); i != actors.end(); ++i) {
        if (!(*i)->isAlive()) {
            if (dynamic_cast<Player *>(*i)) {
                delete *i;
                *i = nullptr;
                gameOver = true;
                player = nullptr;
                cout << "Player died..." << endl;
            }
            delete *i;
            *i = nullptr;
            actors.erase(i);
            if (actors.empty())
                break;
            i = actors.begin();
        }
    }

    if (gameOver)
        return;

    for (auto a: actors) {
        a->control();
        a->setVelocity(collisions(a->getCollisionAABB().getGlobalBounds(), a->getVelocity()));
        a->update(elapsed);
    }

    runTileEvent(player);

    selectedTile = (Tile *) (selectTile(game));

    //Show the user where they are clicking and act on entities within that square
    if (selectedTile != nullptr) {
        if (getTileActors(selectedTile) != nullptr && player != nullptr)
            player->act(getTileActors(selectedTile));
    }
}

void PlayState::render(Game *game) {
    if (gameOver) {
        player = nullptr;
        return;
    }

    game->getWindow()->clear();

    //Get & Update the camera
    sf::View currView = game->getWindow()->getView();
    currView.setCenter(player->getPosition());
    game->getWindow()->setView(currView);
    sf::Vector2f debugPos(currView.getCenter().x - (currView.getSize().x / 2),
                          game->getWindow()->getView().getCenter().y - (currView.getSize().y / 2));

    //Draw the map to the screen
    for (auto j: *d->getFloor(floor)->getMap())
        for (auto k: j)
            game->getWindow()->draw(k->getGraphicalRepresentation());

    //Show the user where they are clicking and act on entities within that square
    if (selectedTile != nullptr) {
        drawTileOutline(selectedTile, game);
    }

    for (auto a: actors) {
        game->getWindow()->draw(a->getSprite());
        game->getWindow()->draw(a->getHealthBar());
    }

    //Update the debug info every second
    if (textclk.getElapsedTime().asSeconds() >= 1) {
        updateDebug();
        textclk.restart();
    }

    //Experimental shadow stuff
    //drawShadows(player, game);

    //Experimental minimap stuff
    drawMinimap(game);

    if (debug)
        showText(debugText, debugPos, game);

    //Update the window
    game->getWindow()->display();
}

//Utility function to switch coordinate systems
sf::Vector2f PlayState::tileToWorldCoord(sf::Vector2i toConv) const {
    float x = toConv.x * 32;
    float y = toConv.y * 32;

    //Remember, tile coordinates are (y, x)
    return sf::Vector2f(y, x);
}

//Utility function to switch coordinate systems
sf::Vector2i PlayState::worldToTileCoord(sf::Vector2f pos) const {
    sf::Vector2i newPos;
    newPos.x = int(pos.y / 32);
    newPos.y = int(pos.x / 32);
    return newPos;
}

//Utility function to switch coordinate systems
sf::Vector2i PlayState::worldToTileCoord(sf::Vector2i pos) const {
    sf::Vector2i newPos;
    newPos.x = pos.y / 32;
    newPos.y = pos.x / 32;
    return newPos;
}

//Return actors at the given tile
//TODO: Currently only returns one actor
Actor *PlayState::getTileActors(Tile *t) {
    //There isn't a tile here.
    if (t == nullptr)
        return nullptr;

    for (auto a: actors) {
        sf::Vector2i testPos = worldToTileCoord(a->getPosition());
        if (t->getPos() == testPos)
            return a;
    }

    //There's nothing in this tile.
    return nullptr;
}

//Return a pointer to the tile at the user's mouse
const Tile *const PlayState::selectTile(Game *game) {
    //Is the user trying to select a tile?
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && focus) {
        sf::Vector2f tilePos = game->getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*game->getWindow()));

        //If the mouse is inbounds on the selected floor, return the tile at its position
        if (d->getFloor(floor)->inBounds(tilePos))
            return d->getFloor(floor)->getTileAtPos(tilePos);
    }

    //The user wasn't trying to select a tile.
    return nullptr;
}

//TODO: Shadows don't draw well, work on it
void PlayState::drawShadows(Entity *focus, Game *game) {
    sf::RenderTexture darkness;
    darkness.create(game->getWindow()->getSize().x, game->getWindow()->getSize().y);
    darkness.clear(sf::Color(100, 100, 100));

    sf::CircleShape light(60);
    light.setOrigin(sf::Vector2f(light.getRadius(), light.getRadius()));
    light.setPosition(game->getWindow()->getSize().x / 2.0f, game->getWindow()->getSize().y / 2.0f);
    light.setFillColor(sf::Color(255, 255, 255, 200));
    darkness.draw(light, sf::BlendAdd);

    sf::Sprite helper(darkness.getTexture());
    helper.setOrigin(helper.getLocalBounds().width / 2.0f, helper.getLocalBounds().height / 2.0f);
    helper.setPosition(focus->getPosition());

    game->getWindow()->draw(helper, sf::BlendMultiply);
}

void PlayState::drawTileOutline(Tile *t, Game *game) {
    sf::RectangleShape outline = sf::RectangleShape(sf::Vector2f(32.0, 32.0));

    //IMPORTANT: Tile accesses are reflected over y = x! so remember, swap x and y!
    sf::Vector2f outlinePos = tileToWorldCoord(t->getPos());

    outline.setPosition(outlinePos);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineColor(sf::Color::Red);
    outline.setOutlineThickness(1.0);

    game->getWindow()->draw(outline);
}

void PlayState::drawMinimap(Game *game) {
    sf::View standard = game->getWindow()->getView();
    unsigned int size = 400; // The 'minimap' view will show a smaller picture of the map
    sf::View minimap(sf::FloatRect(standard.getCenter().x, standard.getCenter().y, static_cast<float>(size),
                                   static_cast<float>(game->getWindow()->getSize().y * size /
                                                      game->getWindow()->getSize().x)));
    //Set the viewport so we can draw a small map
    minimap.setViewport(sf::FloatRect(1.f - (minimap.getSize().x) / game->getWindow()->getSize().x - 0.02f,
                                      1.f - (minimap.getSize().y) / game->getWindow()->getSize().y - 0.02f,
                                      (minimap.getSize().x) / game->getWindow()->getSize().x,
                                      (minimap.getSize().y) / game->getWindow()->getSize().y));
    minimap.zoom(8.f);
    game->getWindow()->setView(minimap);

    //Draw the minimap to the screen
    for (auto row: *d->getFloor(floor)->getMap())
        for (auto tile: row)
            game->getWindow()->draw(tile->getGraphicalRepresentation());
    game->getWindow()->setView(standard);
}

//Show supplied text at supplied position
void PlayState::showText(string s, sf::Vector2f pos, Game *game) {
    sf::Text t;
    t = sf::Text("", font);
    t.setCharacterSize(16);
    t.setColor(sf::Color::White);
    t.setFont(font);

    t.setPosition(pos);
    t.setString(s);

    game->getWindow()->draw(t);
}

//Return a string with useful development information.
string PlayState::updateDebug() {
    stringstream info;
    info << "Vsync: " << (vsync ? "Enabled" : "Disabled") << "\nTarget FPS: " << targetfps << "\nFPS: " <<
    (int) ((1.f / elapsed.asSeconds()) + .5) << "\nSPF: " << elapsed.asSeconds();
    info << "\nFloor: " << floor + 1;

    if (player != nullptr)
        info << "\n" << player->toString();
    else
        info << "\n" << "Player is dead!";

    debugText = info.str();

    return info.str();
}

PlayState::~PlayState() {
    for (int i = 0; i < actors.size(); ++i) {
        delete actors[i];
        actors[i] = nullptr;
    }
    delete d;
}