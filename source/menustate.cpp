//
// Created by Doran on 7/9/2015.
//

#include "../include/menustate.h"

MenuState::MenuState() {
    selection = 0;
    font.loadFromFile("Minecraftia.ttf");

    choices.emplace_back("New...", font);
    choices.emplace_back("Load...", font);
    choices.emplace_back("Save...", font);
    choices.emplace_back("Exit...", font);

    for (auto &c: choices) {
        c.setColor(sf::Color::Blue);
        c.setOrigin(c.getLocalBounds().width / 2.0f, c.getLocalBounds().height / 2.0f);
    }
}

void MenuState::handleEvents(Game *game) {

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

bool MenuState::select(Game *game) {
    switch (selection) {
        case 0:
            //game->setDungeon(new Dungeon(78, 78));
            toChange = new PlayState(new Dungeon(78, 78));
            break;
        case 1:
            toChange = new PlayState(new Dungeon("default.txt"));
            break;
        case 2:
            //game->getWindow()->close();
            break;
        case 3:
            game->getWindow()->close();
            break;
    }
}

void MenuState::update(Game *game) {
    if (game->getDungeon() == nullptr)
        choices[2].setString("Save...(Not available!)");
    else
        choices[2].setString("Save...");

    choices[2].setOrigin(choices[2].getLocalBounds().width / 2.0f,
                         choices[2].getLocalBounds().height / 2.0f);

    for (int i = 0; i < choices.size(); ++i)
        choices[i].setPosition(game->getWindow()->getView().getCenter() + sf::Vector2f(0, 30 * i));
}

void MenuState::render(Game *game) {
    game->getWindow()->clear();

    sf::RectangleShape showSelect(
            sf::Vector2f(choices[selection].getLocalBounds().width, choices[selection].getLocalBounds().height));
    showSelect.setFillColor(sf::Color::Green);
    showSelect.setOrigin(showSelect.getSize() / 2.0f);
    showSelect.setPosition(choices[selection].getPosition());

    game->getWindow()->draw(showSelect);

    for (auto c: choices)
        game->getWindow()->draw(c);

    game->getWindow()->display();
}
