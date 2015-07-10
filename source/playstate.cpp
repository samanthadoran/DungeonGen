//
// Created by Doran on 7/10/2015.
//

#include "../include/playstate.h"

PlayState::PlayState(Dungeon *dungeon) {
    d = dungeon;
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

        // Close window: exit
        if (event.type == sf::Event::Closed)
            game->getWindow()->close();
    }
}

void PlayState::update(Game *game) {

}

void PlayState::render(Game *game) {

}