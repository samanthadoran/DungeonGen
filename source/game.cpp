#include "../include/game.h"

using namespace std;

Game::Game() {
    selectedTile = nullptr;
    floor = 0;
    d = Dungeon(78, 78);
    //TODO: Generate items and equipment
    //TODO: Generate NPCs

    music.openFromFile("music.ogg");
    music.setLoop(true);

    //Creating our window
    app.create(sf::VideoMode(1440, 900), "Dungeon Generator");

    //Load font for debugging
    font.loadFromFile("Minecraftia.ttf");

    text = sf::Text("", font);
    text.setCharacterSize(16);
    text.setColor(sf::Color::White);

    //Hacky way to get crisp bitmap glyphs
    const_cast<sf::Texture &>(font.getTexture(text.getCharacterSize())).setSmooth(false);

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
}

void Game::runEvents() {
    // Process events
    sf::Event event;
    while (app.pollEvent(event)) {
        if (event.type == sf::Event::Resized) {
            // update the view to the new size of the window
            sf::FloatRect newView(0, 0, event.size.width, event.size.height);
            newView.left = app.getView().getCenter().x - app.getView().getSize().x / 2.0;
            newView.top = app.getView().getCenter().y - app.getView().getSize().y / 2.0;
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
        if (event.type == sf::Event::KeyPressed) if (event.key.code == sf::Keyboard::Escape)
            app.close();
    }
}

//Returns a modified vector to obey collision models
sf::Vector2f Game::collisions(sf::Rect<float> test, sf::Vector2f movement) const {
    sf::Rect<float> future = test;
    future.left += movement.x * elapsed.asSeconds();

    for (auto row: d.getFloor(floor).getMap())
        for (auto tile: row)
            if (tile->getTileType() == TileType::WALL) if (tile->getGraphicalRepresentation().getGlobalBounds().intersects(future))
                movement.x = 0;

    future = test;
    future.top += movement.y * elapsed.asSeconds();

    for (auto row: d.getFloor(floor).getMap())
        for (auto tile: row)
            if (tile->getTileType() == TileType::WALL) if (tile->getGraphicalRepresentation().getGlobalBounds().intersects(future))
                movement.y = 0;

    return movement;
}

void Game::runTileEvent(Player *p) {
    Tile *t = d.getFloor(floor).getTileAtPos(worldToTileCoord(p->getPosition()));
    TileType type = t->getTileType();

    switch (type) {
        case TileType::STAIRS_UP:
            if (floor == 0) {
                cout << "You left the dungeon, exiting program..." << endl;
                app.close();
            }
            else {
                --floor;
                p->setPosition(tileToWorldCoord(d.getFloor(floor).getStairsDownSpawn()));
            }
            break;
        case TileType::STAIRS_DOWN:
            if (d.getFloors().size() == floor + 1)
                d.addFloor();

            ++floor;
            p->setPosition(tileToWorldCoord(d.getFloor(floor).getStairsUpSpawn()));
            break;
        default:
            break;
    }
}

void Game::loop() {
    bool pressed = false;

    Player player(tileToWorldCoord(d.getFloor(floor).getStairsUpSpawn()), 160, "link.png");
    NPC testnpc(tileToWorldCoord(d.getFloor(floor).getStairsUpSpawn()), "link.png");
    entities.push_back(&player);
    actors.push_back(&player);
    entities.push_back(&testnpc);
    actors.push_back(&testnpc);


    sf::Clock textclk;
    textclk.restart();

    //Start the game loop
    music.play();

    elapsed = clk.restart();

    while (app.isOpen()) {
        elapsed = clk.restart();

        // Clear screen
        app.clear();

        //Check for application events.
        runEvents();

        for (auto e: actors) {
            e->control();
            e->setVelocity(collisions(e->getCollisionAABB().getGlobalBounds(), e->getVelocity()));
            e->update(elapsed);
        }

        runTileEvent(&player);

        //Get & Update the camera
        sf::View currView = app.getView();
        currView.setCenter(player.getPosition());
        app.setView(currView);

        //Draw the map to the screen
        for (auto j: d.getFloor(floor).getMap())
            for (auto k: j)
                app.draw(k->getGraphicalRepresentation());

        //Experimental shadow stuff
        //drawShadows(&player);

        //Experimental minimap stuff
        drawMinimap();

        //We only want to detect this on press~
        if (!pressed && sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
            //We've pressed it!
            pressed = true;
            //Toggle debug status
            debug = !debug;
        }
        else if (pressed && !sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
            //We've released the key
            pressed = false;

        selectedTile = selectTile();

        if (selectedTile != nullptr) {
            sf::RectangleShape outline = sf::RectangleShape(sf::Vector2f(32.0, 32.0));

            //IMPORTANT: Tile accesses are reflected over y = x! so remember, swap x and y!
            sf::Vector2f outlinePos = sf::Vector2f(selectedTile->getPos().y * 32, selectedTile->getPos().x * 32);

            outline.setPosition(outlinePos);
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineColor(sf::Color::Red);
            outline.setOutlineThickness(1.0);

            app.draw(outline);

            if (getTileActors(selectedTile) != nullptr) {
                cout << "\n\n\n" << endl;
                cout << "Before interaction " << getTileActors(selectedTile)->toString() << endl;
                cout << "---------------------------------" << endl;
                cout << "Damage applied: " << player.attack(getTileActors(selectedTile)) << endl;
                cout << "After attack" << endl << getTileActors(selectedTile)->toString() << endl;
            }
            else {
                cout << "No Target!" << endl;
            }
        }

        //Save the current dungeon
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
            d.saveDungeon("default.txt");

        //Load a dungeon...
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F9)) {
            d = Dungeon("default.txt");
            floor = 0;
            player.setPosition(tileToWorldCoord(d.getFloor(floor).getStairsUpSpawn()));
            continue;
        }

        for (auto e: entities)
            app.draw(e->getSprite());

        //Update the debug info every second
        if (textclk.getElapsedTime().asSeconds() >= 1) {
            std::stringstream info;
            info << "Vsync: " << (vsync ? "Enabled" : "Disabled") << "\nTarget FPS: " << targetfps << "\nFPS: " << (int) ((1.f / elapsed.asSeconds()) + .5) << "\nSPF: " << elapsed.asSeconds();
            info << "\nFloor: " << floor + 1;
            text.setString(info.str());
            textclk.restart();
        }

        //Align the text to the viewport
        text.setPosition(currView.getCenter().x - (currView.getSize().x / 2), app.getView().getCenter().y - (currView.getSize().y / 2));
        app.draw(text);

        // Update the window
        app.display();
    }
}

sf::Vector2f Game::tileToWorldCoord(sf::Vector2i toConv) const {
    float x = toConv.x * 32;
    float y = toConv.y * 32;

    return sf::Vector2f(x, y);
}

sf::Vector2i Game::worldToTileCoord(sf::Vector2f pos) const {
    sf::Vector2i newPos;
    newPos.x = pos.x / 32;
    newPos.y = pos.y / 32;
    return newPos;
}

sf::Vector2i Game::worldToTileCoord(sf::Vector2i pos) const {
    sf::Vector2i newPos;
    newPos.x = pos.x / 32;
    newPos.y = pos.y / 32;
    return newPos;
}

Actor *Game::getTileActors(Tile *t) {
    //There isn't a tile here.
    if (t == nullptr)
        return nullptr;

    for (auto a: actors) {
        sf::Vector2i testPos = worldToTileCoord(a->getPosition());
        if (t->getPos().y == testPos.x && t->getPos().x == testPos.y)
            return a;
    }

    //There's nothing in this tile.
    return nullptr;
}

Tile *Game::selectTile() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && focus) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(app);
        sf::Vector2f toConvert = app.mapPixelToCoords(mousePos);
        if (d.getFloor(floor).inBounds(toConvert)) {
            sf::Vector2i testPos = worldToTileCoord(toConvert);
            return d.getFloor(floor).getTileAtPos(testPos);
        }
    }
    return nullptr;
}

void Game::drawShadows(Entity *focus) {
    sf::RenderTexture darkness;
    darkness.create(app.getSize().x, app.getSize().y);
    darkness.clear(sf::Color(100, 100, 100));

    sf::CircleShape light(60);
    light.setOrigin(sf::Vector2f(light.getRadius() / 2.0, light.getRadius() / 2.0));
    light.setPosition(app.getSize().x / 2.0 - 20, app.getSize().y / 2.0 - 40);
    light.setFillColor(sf::Color(255, 255, 255, 200));
    darkness.draw(light, sf::BlendAdd);

    sf::Sprite helper(darkness.getTexture());
    helper.setOrigin(helper.getLocalBounds().width / 2.0, helper.getLocalBounds().height / 2.0);
    helper.setPosition(focus->getPosition());

    app.draw(helper, sf::BlendMultiply);
}

void Game::drawMinimap() {
    sf::View standard = app.getView();
    unsigned int size = 400; // The 'minimap' view will show a smaller picture of the map
    sf::View minimap(sf::FloatRect(standard.getCenter().x, standard.getCenter().y, static_cast<float>(size), static_cast<float>(app.getSize().y * size / app.getSize().x)));
    minimap.setViewport(sf::FloatRect(1.f - static_cast<float>(minimap.getSize().x) / app.getSize().x - 0.02f, 1.f - static_cast<float>(minimap.getSize().y) / app.getSize().y - 0.02f, static_cast<float>(minimap.getSize().x) / app.getSize().x, static_cast<float>(minimap.getSize().y) / app.getSize().y));
    minimap.zoom(8.f);
    app.setView(minimap);

    //Draw the minimap to the screen
    for (auto j: d.getFloor(floor).getMap())
        for (auto k: j)
            app.draw(k->getGraphicalRepresentation());
    app.setView(standard);
}

Game::~Game() {

}