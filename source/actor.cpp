#include "../include/actor.h"

Actor::Actor(sf::Vector2f pos, int damage, string filename, string name) : Entity(pos, filename) {
    walkingAnimationUp = new Animation();
    walkingAnimationUp->setSpriteSheet(*getSprite().getTexture());
    walkingAnimationUp->addFrame(sf::IntRect(24, 0, 24, 32));
    walkingAnimationUp->addFrame(sf::IntRect(48, 0, 24, 32));
    walkingAnimationUp->addFrame(sf::IntRect(24, 0, 24, 32));
    walkingAnimationUp->addFrame(sf::IntRect(0, 0, 24, 32));

    walkingAnimationRight = new Animation();
    walkingAnimationRight->setSpriteSheet(*getSprite().getTexture());
    walkingAnimationRight->addFrame(sf::IntRect(24, 32, 24, 32));
    walkingAnimationRight->addFrame(sf::IntRect(48, 32, 24, 32));
    walkingAnimationRight->addFrame(sf::IntRect(24, 32, 24, 32));
    walkingAnimationRight->addFrame(sf::IntRect(0, 32, 24, 32));

    walkingAnimationDown = new Animation();
    walkingAnimationDown->setSpriteSheet(*getSprite().getTexture());
    walkingAnimationDown->addFrame(sf::IntRect(24, 64, 24, 32));
    walkingAnimationDown->addFrame(sf::IntRect(48, 64, 24, 32));
    walkingAnimationDown->addFrame(sf::IntRect(24, 64, 24, 32));
    walkingAnimationDown->addFrame(sf::IntRect(0, 64, 24, 32));

    walkingAnimationLeft = new Animation();
    walkingAnimationLeft->setSpriteSheet(*getSprite().getTexture());
    walkingAnimationLeft->addFrame(sf::IntRect(24, 96, 24, 32));
    walkingAnimationLeft->addFrame(sf::IntRect(48, 96, 24, 32));
    walkingAnimationLeft->addFrame(sf::IntRect(24, 96, 24, 32));
    walkingAnimationLeft->addFrame(sf::IntRect(0, 96, 24, 32));

    animatedSprite = new AnimatedSprite(sf::seconds(0.2), true, true);
    animatedSprite->setAnimation(*walkingAnimationDown);

    healthBar = sf::RectangleShape(sf::Vector2f(24, 4));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setOrigin(healthBar.getSize() / 2.0f);
    healthBar.setPosition(getPosition());
    this->damage = damage;
    this->name = name;
    hp = 100;
}

void Actor::update(sf::Time elapsed) {
    Animation *currentAnimation = (Animation *) animatedSprite->getAnimation();

    if (getVelocity().y < 0)
        currentAnimation = walkingAnimationUp;
    else if (getVelocity().y > 0)
        currentAnimation = walkingAnimationDown;

    if (fabs(getVelocity().x) > fabs(getVelocity().y)) if (getVelocity().x < 0)
        currentAnimation = walkingAnimationLeft;
    else if (getVelocity().x > 0)
        currentAnimation = walkingAnimationRight;

    animatedSprite->play(*currentAnimation);

    if (getVelocity().x == 0 && getVelocity().y == 0)
        animatedSprite->stop();

    animatedSprite->update(elapsed);

    Entity::update(elapsed);

    animatedSprite->setOrigin(animatedSprite->getLocalBounds().width / 2.0f,
                              animatedSprite->getLocalBounds().height / 2.0f);
    animatedSprite->setPosition(getPosition());

    for (auto i: items) {
        i->setPosition(getPosition());
        i->update(elapsed);
    }

    if (hp < 0)
        kill();

    healthBar.setPosition(getPosition() - sf::Vector2f(0, 13));
}

vector<Actor *> Actor::getItems() const {
    return items;
}

void Actor::addItem(Actor *a) {
    items.push_back(a);
}

string Actor::getName() const {
    return name;
}

double Actor::getHP() const {
    return hp;
}

sf::RectangleShape Actor::getHealthBar() const {
    return healthBar;
}

void Actor::setHP(double hp) {
    this->hp = hp;
    healthBar.setScale(sf::Vector2f(getHP() / 100.0f, 1));
}

double Actor::getDamage() const {
    return damage;
}

void Actor::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(*animatedSprite, states);
    target.draw(getHealthBar(), states);

    if (!items.empty())
        target.draw(items[0]->getSprite(), states);
}

Actor::~Actor() {
    for (int i = 0; i < items.size(); ++i) {
        delete items[i];
        items[i] = nullptr;
    }

    //Ugly cleanup
    delete walkingAnimationDown;
    delete walkingAnimationUp;
    delete walkingAnimationLeft;
    delete walkingAnimationRight;
    delete animatedSprite;

}