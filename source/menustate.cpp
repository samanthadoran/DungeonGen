//
// Created by Doran on 7/9/2015.
//

#include "../include/menustate.h"

MenuState::MenuState() {
    font.loadFromFile("Minecraftia.ttf");

    choices.emplace_back("New...", font);
    choices.emplace_back("Load...", font);
    choices.emplace_back("Save...", font);

    for (auto &c: choices) {
        c.setColor(sf::Color::Blue);
        c.setOrigin(c.getLocalBounds().width / 2.0f, c.getLocalBounds().height / 2.0f);
    }
}

void MenuState::update(Game *game) {
    if (game->getDungeon() == nullptr)
        choices.back().setString("Save...(Not available!)");
    else
        choices.back().setString("Save...");

    choices.back().setOrigin(choices.back().getLocalBounds().width / 2.0f,
                             choices.back().getLocalBounds().height / 2.0f);

    for (int i = 0; i < choices.size(); ++i)
        choices[i].setPosition(game->getWindow()->getView().getCenter() + sf::Vector2f(0, 30 * i));
}

void MenuState::render(Game *game) {
    game->getWindow()->clear();

    for (auto c: choices)
        game->getWindow()->draw(c);

    game->getWindow()->display();
}
