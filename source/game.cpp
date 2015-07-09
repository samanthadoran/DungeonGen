#include "../include/game.h"

using namespace std;

Game::Game() {
    //Creating our window
    app.create(sf::VideoMode(1440, 900), "Dungeon Generator");

    //Load font for debugging
    font.loadFromFile("Minecraftia.ttf");

    //adjust the view to center on map
    sf::View view = app.getView();
    view.setCenter(0.f, 0.f);
    view.zoom(1.0);
    app.setView(view);
    targetfps = 60;
    app.setFramerateLimit(targetfps);
    vsync = true;
    app.setVerticalSyncEnabled(vsync);

    //to toggle debug output
    debug = true;
    focus = true;

    d = nullptr;
}

// Process events thrown by sfml
void Game::runEvents() {
    sf::Event event;
    while (app.pollEvent(event)) {
        if (event.type == sf::Event::Resized) {
            // update the view to the new size of the window
            sf::FloatRect newView(0, 0, event.size.width, event.size.height);
            newView.left = app.getView().getCenter().x - app.getView().getSize().x / 2.0f;
            newView.top = app.getView().getCenter().y - app.getView().getSize().y / 2.0f;
            app.setView(sf::View(newView));
        }

        //We can control it!
        if (event.type == sf::Event::GainedFocus)
            focus = true;

        //Don't send inputs!
        if (event.type == sf::Event::LostFocus)
            focus = false;

        // Close window: exit
        if (event.type == sf::Event::Closed)
            app.close();

        //Escape key: exit
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Escape:
                    app.close();
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
                    if (music.getVolume() == 0)
                        music.setVolume(20);
                    else
                        music.setVolume(0);
                    break;
                default:
                    break;
            }
        }
    }
}

//Returns a modified vector to obey collision models
sf::Vector2f Game::collisions(sf::Rect<float> test, sf::Vector2f movement) const {
    sf::Rect<float> future = test;

    sf::Vector2f futurePos;

    future.left += movement.x * elapsed.asSeconds();
    futurePos = sf::Vector2f(future.left, future.top);
    if (!d->getFloor(floor)->inBounds(futurePos)) {
        movement.x = 0;
    }

    for (auto row: *d->getFloor(floor)->getMap())
        for (auto tile: row)
            if (tile->getTileType() ==
                TileType::WALL) if (tile->getGraphicalRepresentation().getGlobalBounds().intersects(future))
                movement.x = 0;

    future = test;
    future.top += movement.y * elapsed.asSeconds();
    futurePos = sf::Vector2f(future.left, future.top);
    if (!d->getFloor(floor)->inBounds(futurePos)) {
        movement.y = 0;
    }

    for (auto row: *d->getFloor(floor)->getMap())
        for (auto tile: row)
            if (tile->getTileType() ==
                TileType::WALL) if (tile->getGraphicalRepresentation().getGlobalBounds().intersects(future))
                movement.y = 0;

    return movement;
}

//TODO: Add more types of tile events.
void Game::runTileEvent(Player *p) {
    const Tile *t = d->getFloor(floor)->getTileAtPos(worldToTileCoord(p->getPosition()));
    TileType type = t->getTileType();

    switch (type) {
        case TileType::STAIRS_UP:
            if (floor == 0)
                app.close();
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

//Setup variables for this dungeon instance
void Game::init() {
    gameOver = false;
    selectedTile = nullptr;
    floor = 0;

    //TODO: Generate items and equipment
    d = new Dungeon(78, 78);

    music.openFromFile("music.ogg");
    music.setLoop(true);

    actors.emplace_back(new Player(tileToWorldCoord(d->getFloor(floor)->getStairsUpSpawn()), 160, "link.png"));
    actors.emplace_back(new NPC(tileToWorldCoord(d->getFloor(floor)->getStairsUpSpawn()), "link.png"));

    player = dynamic_cast<Player *>(actors.front());
    dynamic_cast<NPC *>(actors.back())->setTarget(player);

    actors.back()->addItem(new Item(sf::Vector2f(-1, -1), 1, "link.png", "claws", -1, 200));
    player->addItem(new Item(sf::Vector2f(-1, -1), 10, "link.png", "sword", -1, 200));

    //Start the game loop
    music.setVolume(20);
    music.play();
}

//TODO: Flesh out in a full menu class
void Game::menu() {
    init();
    loop();
}

void Game::update() {
    elapsed = clk.restart();

    //Check for application events.
    runEvents();

    for (auto i = actors.begin(); i != actors.end(); ++i) {
        if (!(*i)->isAlive()) {
            if (dynamic_cast<Player *>(*i)) {
                *i = nullptr;
                gameOver = true;
                player = nullptr;
            }

            delete *i;
            *i = nullptr;
            actors.erase(i);
            if (actors.empty())
                break;
            i = actors.begin();
        }
    }

    for (auto a: actors) {
        a->control();
        a->setVelocity(collisions(a->getCollisionAABB().getGlobalBounds(), a->getVelocity()));
        a->update(elapsed);
    }

    runTileEvent(player);

    selectedTile = (Tile *) (selectTile());

    //Show the user where they are clicking and act on entities within that square
    if (selectedTile != nullptr) {
        if (getTileActors(selectedTile) != nullptr && player != nullptr)
            player->act(getTileActors(selectedTile));
    }
}

void Game::render() {
    app.clear();

    //Get & Update the camera
    sf::View currView = app.getView();
    currView.setCenter(player->getPosition());
    app.setView(currView);
    sf::Vector2f debugPos(currView.getCenter().x - (currView.getSize().x / 2),
                          app.getView().getCenter().y - (currView.getSize().y / 2));

    //Draw the map to the screen
    for (auto j: *d->getFloor(floor)->getMap())
        for (auto k: j)
            app.draw(k->getGraphicalRepresentation());

    //Experimental shadow stuff
    //drawShadows(&player);

    //Experimental minimap stuff
    drawMinimap();

    selectedTile = (Tile *) (selectTile());

    //Show the user where they are clicking and act on entities within that square
    if (selectedTile != nullptr) {
        drawTileOutline(selectedTile);
    }

    for (auto a: actors)
        app.draw(a->getSprite());

    for (auto a: actors)
        app.draw(a->getHealthBar());

    //Update the debug info every second
    if (textclk.getElapsedTime().asSeconds() >= 1) {
        updateDebug();
        textclk.restart();
    }

    if (debug)
        showText(debugText, debugPos);

    //Update the window
    app.display();
}

void Game::loop() {
    textclk.restart();

    while (app.isOpen()) {
        update();
        render();
    }

    //Clean up!
    delete d;
}

//Utility function to switch coordinate systems
sf::Vector2f Game::tileToWorldCoord(sf::Vector2i toConv) const {
    float x = toConv.x * 32;
    float y = toConv.y * 32;

    //Remember, tile coordinates are (y, x)
    return sf::Vector2f(y, x);
}

//Utility function to switch coordinate systems
sf::Vector2i Game::worldToTileCoord(sf::Vector2f pos) const {
    sf::Vector2i newPos;
    newPos.x = int(pos.y / 32);
    newPos.y = int(pos.x / 32);
    return newPos;
}

//Utility function to switch coordinate systems
sf::Vector2i Game::worldToTileCoord(sf::Vector2i pos) const {
    sf::Vector2i newPos;
    newPos.x = pos.y / 32;
    newPos.y = pos.x / 32;
    return newPos;
}

//Return actors at the given tile
//TODO: Currently only returns one actor
Actor *Game::getTileActors(Tile *t) {
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
const Tile *const Game::selectTile() {
    //Is the user trying to select a tile?
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && focus) {
        sf::Vector2f tilePos = app.mapPixelToCoords(sf::Mouse::getPosition(app));

        //If the mouse is inbounds on the selected floor, return the tile at its position
        if (d->getFloor(floor)->inBounds(tilePos))
            return d->getFloor(floor)->getTileAtPos(tilePos);
    }

    //The user wasn't trying to select a tile.
    return nullptr;
}

//TODO: Shadows don't draw well, work on it
void Game::drawShadows(Entity *focus) {
    sf::RenderTexture darkness;
    darkness.create(app.getSize().x, app.getSize().y);
    darkness.clear(sf::Color(100, 100, 100));

    sf::CircleShape light(60);
    light.setOrigin(sf::Vector2f(light.getRadius() / 2.0f, light.getRadius() / 2.0f));
    light.setPosition(app.getSize().x / 2.0f - 20, app.getSize().y / 2.0f - 40);
    light.setFillColor(sf::Color(255, 255, 255, 200));
    darkness.draw(light, sf::BlendAdd);

    sf::Sprite helper(darkness.getTexture());
    helper.setOrigin(helper.getLocalBounds().width / 2.0f, helper.getLocalBounds().height / 2.0f);
    helper.setPosition(focus->getPosition());

    app.draw(helper, sf::BlendMultiply);
}

void Game::drawTileOutline(Tile *t) {
    sf::RectangleShape outline = sf::RectangleShape(sf::Vector2f(32.0, 32.0));

    //IMPORTANT: Tile accesses are reflected over y = x! so remember, swap x and y!
    sf::Vector2f outlinePos = tileToWorldCoord(t->getPos());

    outline.setPosition(outlinePos);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineColor(sf::Color::Red);
    outline.setOutlineThickness(1.0);

    app.draw(outline);
}

void Game::drawMinimap() {
    sf::View standard = app.getView();
    unsigned int size = 400; // The 'minimap' view will show a smaller picture of the map
    sf::View minimap(sf::FloatRect(standard.getCenter().x, standard.getCenter().y, static_cast<float>(size),
                                   static_cast<float>(app.getSize().y * size / app.getSize().x)));
    //Set the viewport so we can draw a small map
    minimap.setViewport(sf::FloatRect(1.f - (minimap.getSize().x) / app.getSize().x - 0.02f,
                                      1.f - (minimap.getSize().y) / app.getSize().y - 0.02f,
                                      (minimap.getSize().x) / app.getSize().x,
                                      (minimap.getSize().y) / app.getSize().y));
    minimap.zoom(8.f);
    app.setView(minimap);

    //Draw the minimap to the screen
    for (auto row: *d->getFloor(floor)->getMap())
        for (auto tile: row)
            app.draw(tile->getGraphicalRepresentation());
    app.setView(standard);
}

//Show supplied text at supplied position
void Game::showText(string s, sf::Vector2f pos) {
    sf::Text t;
    t = sf::Text("", font);
    t.setCharacterSize(16);
    t.setColor(sf::Color::White);
    t.setFont(font);

    t.setPosition(pos);
    t.setString(s);

    app.draw(t);
}

//Return a string with useful development information.
string Game::updateDebug() {
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

Game::~Game() {
    for (int i = 0; i < actors.size(); ++i) {
        delete actors[i];
        actors[i] = nullptr;
    }
    delete d;
    delete player;
}