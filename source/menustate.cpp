//
// Created by Doran on 7/9/2015.
//

#include "../include/menustate.h"

MenuState::MenuState() {
    selection = 0;
    font.loadFromFile("Minecraftia.ttf");

    //Consider making this dynamic
    choices.emplace_back("New...", font);
    choices.emplace_back("Load...", font);
    choices.emplace_back("Save...", font);
    choices.emplace_back("Resume...", font);
    choices.emplace_back("Quit...", font);

    //Apply specific settings to our options
    for (auto &c: choices) {
        c.setColor(sf::Color::Blue);
        c.setOrigin(c.getLocalBounds().width / 2.0f, c.getLocalBounds().height / 2.0f);
    }
}

void MenuState::handleEvents(Game *game) {
    //The user has made a selection
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
        select(game);

    sf::Event event;
    while (game->getWindow()->pollEvent(event)) {
        if (event.type == sf::Event::Resized) {
            // update the view to the new size of the window
            sf::FloatRect newView(0, 0, event.size.width, event.size.height);
            newView.left = game->getWindow()->getView().getCenter().x - game->getWindow()->getView().getSize().x / 2.0f;
            newView.top = game->getWindow()->getView().getCenter().y - game->getWindow()->getView().getSize().y / 2.0f;
            game->getWindow()->setView(sf::View(newView));
        }

        // Close window: exit
        if (event.type == sf::Event::Closed)
            game->getWindow()->close();

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Key::Down) if (selection != choices.size() - 1)
                selection++;
            else
                selection = 0;
            if (event.key.code == sf::Keyboard::Key::Up) if (selection != 0)
                selection--;
            else
                selection = choices.size() - 1;
        }
    }
}

void MenuState::select(Game *game) {
    Dungeon *d = nullptr;
    std::future<Dungeon*> f1;
    
    //Fancily display that we are generating a new dungeon
    sf::RectangleShape loading(sf::Vector2f(10,10));
    sf::Text message("Generating a dungeon. Sorry for the wait!", font);
    message.setOrigin(message.getLocalBounds().width / 2.0, message.getLocalBounds().height / 2.0);
    message.setPosition(game->getWindow()->getView().getCenter() + sf::Vector2f(0, 20));
    loading.setPosition(game->getWindow()->getView().getCenter());
    loading.setOrigin(5,5);
    loading.setFillColor(sf::Color::Green);
    
    switch (selection) {
        case 0:
            //Lambdas are useful for me for literally the first time
            f1 = std::async(std::launch::async, [](){
                return new Dungeon(78, 78);
            });
            
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
            d = f1.get();
            toChange = new PlayState(game, d);
            game->setDungeon(d);
            break;
        case 1:
            d = new Dungeon("default.txt");
            toChange = new PlayState(game, d);
            game->setDungeon(d);
            break;
        case 2:
            if (game->getDungeon() != nullptr)
                game->getDungeon()->saveDungeon("default.txt");
            break;
        case 3:
            if (game->getDungeon() != nullptr)
                toKill = true;
            break;
        case 4:
            game->getWindow()->close();
            break;
        default:
            break;
    }
}

void MenuState::update(Game *game) {
    //We can't save if the dungeon doesn't exist
    if (game->getDungeon() == nullptr) {
        choices[2].setString("Save...(Not available!)");
        choices[3].setString("Resume...(Not available!)");
    }
    else {
        choices[2].setString("Save...");
        choices[3].setString("Resume...");
    }

    //If we change the text size, we need to recenter it
    choices[2].setOrigin(choices[2].getLocalBounds().width / 2.0f,
                         choices[2].getLocalBounds().height / 2.0f);
    choices[3].setOrigin(choices[3].getLocalBounds().width / 2.0f,
                         choices[3].getLocalBounds().height / 2.0f);

    //Keep the positions in the middle of the screen
    for (int i = 0; i < choices.size(); ++i)
        choices[i].setPosition(game->getWindow()->getView().getCenter() + sf::Vector2f(0, 30 * i));
}

void MenuState::render(Game *game) {
    game->getWindow()->clear();

    //Make a rect to show the user their current selection
    sf::RectangleShape showSelect(
            sf::Vector2f(choices[selection].getLocalBounds().width, choices[selection].getLocalBounds().height));
    showSelect.setFillColor(sf::Color::Green);
    showSelect.setOrigin(showSelect.getSize() / 2.0f);
    showSelect.setPosition(choices[selection].getPosition() + sf::Vector2f(0, 3));

    //The selection must be drawn first so as not to overlap with the text
    game->getWindow()->draw(showSelect);

    for (auto c: choices)
        game->getWindow()->draw(c);

    game->getWindow()->display();
}
