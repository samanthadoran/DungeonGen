//
// Created by Doran on 7/10/2015.
//

#include "../include/playstate.h"
#include "../include/menustate.h"

//Fix an annoying bug on MinGW until it is fixed.
#ifdef _WIN32
namespace fix {
    template<typename T>
    std::string to_string(const T &other) {
        std::stringstream s;
        s << other;
        return s.str();
    }
}
using namespace fix;
#endif

PlayState::PlayState(Game * game, Dungeon *dungeon) {
    score = 0;
    d = dungeon;

    gameOver = false;
    selectedTile = nullptr;
    floor = 0;

    paused = false;

    //Load font for debugging
    font.loadFromFile("Minecraftia.ttf");

    //to toggle debug output
    debug = true;
    focus = true;

    vsync = true;
    targetfps = 60;

    player = nullptr;

    changeFloor(game, 0);
}

void PlayState::cleanupActors() {
    for (int i = 0; i < actors.size(); ++i) {
        delete actors[i];
        actors[i] = nullptr;
    }
    actors.clear();
}

void PlayState::initiateActors(int floor) {
    if (player == nullptr) {
        actors.emplace_back(new Player(tileToWorldCoord(d->getFloor(floor)->getStairsUpSpawn()), 240, "player.png"));
        player = dynamic_cast<Player *>(actors.front());
        player->addItem(new Weapon(sf::Vector2f(-1, -1), 10, "crossbow.png", "crossbow", 500, 200));
    }
    else {
        actors.push_back(player);
        if(floor > this->floor)
            player->setPosition(tileToWorldCoord(d->getFloor(floor)->getStairsUpSpawn()));
        else
            player->setPosition(tileToWorldCoord(d->getFloor(floor)->getStairsDownSpawn()));
    }

    while (actors.size() < 15) {
        actors.emplace_back(new NPC(sf::Vector2f(0, 0), "monster.png"));
        actors.back()->addItem(new Weapon(sf::Vector2f(-1, -1), 1, "sword.png", "sword", -1, 200));
        dynamic_cast<NPC *>(actors.back())->setTarget(player);
    }

    //Randomize mob positions
    for (int i = 1; i < actors.size(); ++i) {
        sf::Vector2f spawnPos(rand() % (78 * 32), rand() % (78 * 32));


        while (d->getFloor(floor)->getTileAtPos(spawnPos)->getTileType() != TileType::FLOOR)
            spawnPos = sf::Vector2f(rand() % (78 * 32), rand() % (78 * 32));

        actors[i]->setPosition(spawnPos);
    }
}

void PlayState::changeFloor(Game * game, int floor) {
    //Fancily display that we are generating a new dungeon
    sf::RectangleShape loading(sf::Vector2f(10,10));
    sf::Text message("Adding floors. Sorry for the wait!", font);
    message.setOrigin(message.getLocalBounds().width / 2.0, message.getLocalBounds().height / 2.0);
    message.setPosition(game->getWindow()->getView().getCenter() + sf::Vector2f(0, 20));
    loading.setPosition(game->getWindow()->getView().getCenter());
    loading.setOrigin(5,5);
    loading.setFillColor(sf::Color::Green);

    std::future<void> f1 = std::async(std::launch::async, [&]{
        while(floor > d->getFloors().size() - 1)
            d->addFloor();
        return;
    });

    //Don't cleanup the player.
    if (!actors.empty())
        actors.front() = nullptr;

    cleanupActors();

    while(f1.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        loading.rotate(1);

        //If the user doesn't want to wait, let them cancel this
        sf::Event event;
        while(game->getWindow()->pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                game->getWindow()->close();
                return;
            }
        }

        game->getWindow()->clear();
        game->getWindow()->draw(loading);
        game->getWindow()->draw(message);
        game->getWindow()->display();
    }

    initiateActors(floor);

    this->floor = floor;
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

void PlayState::pickupItems(Player *p, const Tile *t) {
    vector<Actor *> actors = getTileActors((Tile *) t);

    for (auto a: actors)
        if (dynamic_cast<Item *>(a) != nullptr && a->isAlive())
            p->addItem(dynamic_cast<Item *>(a)->pickUp());
}

//TODO: Add more types of tile events.
void PlayState::runTileEvent(Game * game, Player *p) {
    const Tile *t = d->getFloor(floor)->getTileAtPos(worldToTileCoord(p->getPosition()));
    TileType type = t->getTileType();

    //When the user crosses over a tile, pickup all items in that tile
    pickupItems(p, t);

    //HandleNormal tile events
    switch (type) {
        case TileType::STAIRS_UP:
            paused = true;
            if (floor == 0)
                gameOver = true;
            else
                changeFloor(game, floor - 1);

            break;
        case TileType::STAIRS_DOWN:
            paused = true;

            changeFloor(game, floor + 1);

            break;
        default:
            break;
    }
}

void PlayState::handleEvents(Game *game) {
    sf::Event event;
    while (game->getWindow()->pollEvent(event)) {
        //Update the view to the new size of the window
        if (event.type == sf::Event::Resized) {
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

        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                //Go to menus
                case sf::Keyboard::Escape:
                    game->pushState(new MenuState());
                    paused = true;
                    break;
                    //Show the user helpful debug info
                case sf::Keyboard::F1:
                    debug = !debug;
                    break;
                    //Music is defunct as of now
                case sf::Keyboard::M:
                    break;
                default:
                    break;
            }
        }
    }
}

void PlayState::checkActorLife() {
    //Drop coins for dead actors!
    for (int i = 0; i < actors.size(); ++i)
        if (!actors[i]->isAlive() && actors[i]->getPoints() > 0)
            actors.emplace_back(
                    new Item(actors[i]->getPosition(), 0, "points.png", "money", actors[i]->getPoints(), 0));


    //Actually kill dead actors
    for (auto i = actors.begin(); i != actors.end(); ++i) {
        if (!(*i)->isAlive()) {
            if (i == actors.begin()) {
                if (dynamic_cast<Player *>(*i)) {
                    delete *i;
                    *i = nullptr;
                    gameOver = true;
                    player = nullptr;
                }
            }

            delete *i;
            *i = nullptr;
            actors.erase(i);

            if (actors.empty())
                break;

            i = actors.begin();
        }
    }
}

void PlayState::dungeonChange(Game *game) {
    for (int i = 0; i < actors.size(); ++i)
        delete actors[i];

    actors.clear();
    delete d;

    d = game->getDungeon();

    changeFloor(game, 0);

    score = 0;
}

void PlayState::update(Game *game) {
    //We were in a menu, we need another once around to clear the clocks
    if (paused) {
        clk.restart();
        paused = false;
        return;
    }

    //Time to close up shop!
    if (gameOver) {
        player = nullptr;
        game->getWindow()->close();
        return;
    }

    //The user must have loaded a new dungeon or generated one! Update accordingly
    if (game->getDungeon() != d)
        dungeonChange(game);

    elapsed = clk.restart();

    //Check for application events.
    handleEvents(game);

    //Make sure that actors are still alive
    checkActorLife();

    if (gameOver)
        return;

    for (auto a: actors) {
        a->control();
        a->setVelocity(collisions(a->getCollisionAABB().getGlobalBounds(), a->getVelocity()));
        a->update(elapsed);
    }

    runTileEvent(game, player);

    selectedTile = (Tile *) (selectTile(game));

    //Show the user where they are clicking and act on entities within that square
    if (player != nullptr)
        for (auto a: getTileActors(selectedTile))
            if (a != nullptr && a->isAlive())
                player->act(a);
}

void PlayState::render(Game *game) {
    if (gameOver) {
        player = nullptr;
        return;
    }

    game->getWindow()->clear();

    //Get & Update the camera
    sf::View currView = game->getWindow()->getView();
    //sf::View currView = game->getWindow()->getDefaultView();
    currView.setCenter(player->getPosition());
    //currView.
    sf::View zoomed = currView;
    zoomed.zoom(.25);
    game->getWindow()->setView(zoomed);


    //Get the position to place the debug text at
    sf::Vector2f debugPos(zoomed.getCenter().x - (zoomed.getSize().x * 2),
                          zoomed.getCenter().y - (zoomed.getSize().y * 2));



    //Draw the map to the screen
    game->getWindow()->draw(*d->getFloor(floor));

    sf::Texture window;
    window.setSmooth(true);
    window.create(game->getWindow()->getSize().x, game->getWindow()->getSize().y);
    window.update(*game->getWindow());

    //Show the user where they are clicking and act on entities within that square
    if (selectedTile != nullptr)
        drawTileOutline(selectedTile, game);

    for (auto a: actors)
        if (a != nullptr && a->isAlive())
            game->getWindow()->draw(*a);

    //Update the debug info every second
    if (textclk.getElapsedTime().asSeconds() >= 1) {
        updateDebug();
        textclk.restart();
    }

    //Experimental shadow stuff
    //drawShadows(player, game);

    //Experimental minimap stuff
    drawMinimap(game, window);


    game->getWindow()->setView(currView);
    if (debug)
        showText(debugText, debugPos, game);

    showText("Score: " + to_string(player->getPoints()),
             debugPos + sf::Vector2f(game->getWindow()->getView().getSize().x / 2.0f, 0), game);

    drawInventoryOfActor(player, game);
    game->getWindow()->setView(zoomed);

    //Update the window
    game->getWindow()->display();
    game->getWindow()->setView(currView);
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
//TODO: Make more rigid. A player can be in two tiles at once, account for this.
vector<Actor *>PlayState::getTileActors(Tile *t) {
    vector<Actor *> toRet;
    //There isn't a tile here.
    if (t == nullptr)
        return toRet;

    for (auto a: actors)
        if (t->getPos() == worldToTileCoord(a->getPosition()))
            toRet.push_back(a);

    //There's nothing in this tile.
    return toRet;
}

//Return a pointer to the tile at the user's mouse
const Tile *const PlayState::selectTile(Game *game) {
    //Is the user trying to select a tile?
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && focus) {
        sf::View zoomedView = game->getWindow()->getView();
        zoomedView.zoom(.25);
        sf::Vector2f tilePos = game->getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*game->getWindow()),
                                                                   zoomedView);

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

void PlayState::drawMinimap(Game *game, sf::Texture &f) {
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

    game->getWindow()->setView(minimap);
    sf::RectangleShape d(minimap.getSize());
    d.setOrigin(d.getLocalBounds().width / 2.f, d.getLocalBounds().height / 2.f);
    d.setTexture(&f);
    d.setPosition(minimap.getCenter());
    game->getWindow()->draw(d);

    game->getWindow()->setView(standard);
}

void PlayState::drawInventoryOfActor(Actor *a, Game *game) {
    sf::Vector2f placePosition = game->getWindow()->getView().getCenter();
    placePosition -= sf::Vector2f(game->getWindow()->getSize().x / 2.0f, 0);
    placePosition += sf::Vector2f(0, game->getWindow()->getSize().y / 2.0f - 32);

    for (int i = 0; i < a->getItems().size(); ++i) {
        //Show the user which item they are using
        if (i == a->getSelectedItemIndex()) {
            sf::RectangleShape outline(sf::Vector2f(32, 32));
            outline.setOutlineThickness(3);
            outline.setOutlineColor(sf::Color::White);
            outline.setFillColor(sf::Color::Transparent);
            outline.setPosition(placePosition + sf::Vector2f(32 * i, 0));
            game->getWindow()->draw(outline);
        }
        sf::Sprite item;
        item.setTexture(*a->getItems()[i]->getSprite().getTexture());
        item.setPosition(placePosition + sf::Vector2f(32 * i, 0));
        game->getWindow()->draw(item);

        showText(to_string(i + 1), placePosition + sf::Vector2f(32 * i, 0), game);
    }
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
            (int) ((1000.f / elapsed.asMilliseconds())) << "\nSPF: " << elapsed.asSeconds();
    info << "\nFloor: " << floor + 1;

    if (player != nullptr) {
        info << "\n" << player->toString();
        info << "\nTileType: " << d->getFloor(floor)->getTileAtPos(player->getPosition())->getTextualRepresentation();
    }
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