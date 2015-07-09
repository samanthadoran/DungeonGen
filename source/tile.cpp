#include "../include/tile.h"

Tile::Tile(vector<sf::Texture *> *tex, int x, int y, TileType t) :
        type(t) {
    textures = tex;
    pos = sf::Vector2i(x, y);

    r.setSize(sf::Vector2f(32, 32));
    r.setPosition(sf::Vector2f(pos.y * 32, pos.x * 32));

    switch (type) {
        case TileType::BLANK:
            r.setFillColor(sf::Color::Black);
            break;
        case TileType::FLOOR:
            r.setTexture((*textures)[0]);
            break;
        case TileType::WALL:
            r.setTexture((*textures)[1]);
            break;
        case TileType::STAIRS_DOWN:
            r.setTexture((*textures)[2]);
            break;
        case TileType::STAIRS_UP:
            r.setTexture((*textures)[3]);
            break;
    }
}

TileType Tile::getTileType() const {
    return type;
}

TileType Tile::getTileTypeFromText(char t) {
    switch (t) {
        case 'B':
            return TileType::BLANK;
        case '_':
            return TileType::FLOOR;
        case '#':
            return TileType::WALL;
        case 'U':
            return TileType::STAIRS_UP;
        case 'D':
            return TileType::STAIRS_DOWN;
        default:
            return TileType::BLANK;
    }
}

char Tile::getTextualRepresentation() const {
    switch (type) {
        case TileType::BLANK:
            return 'B';
        case TileType::FLOOR:
            return '_';
        case TileType::WALL:
            return '#';
        case TileType::STAIRS_UP:
            return 'U';
        case TileType::STAIRS_DOWN:
            return 'D';
    }
}


sf::RectangleShape Tile::getGraphicalRepresentation() const {
    return r;
}

sf::Vector2i Tile::getPos() const {
    return pos;
}

void Tile::setPos(sf::Vector2i p) {
    this->pos = p;
}

void Tile::setTileType(TileType t) {
    r = sf::RectangleShape();
    r.setSize(sf::Vector2f(32, 32));
    r.setPosition(sf::Vector2f(pos.y * 32, pos.x * 32));
    type = t;

    switch (type) {
        case TileType::BLANK:
            r.setFillColor(sf::Color::Black);
            break;
        case TileType::FLOOR:
            r.setTexture((*textures)[0]);
            break;
        case TileType::WALL:
            r.setTexture((*textures)[1]);
            break;
        case TileType::STAIRS_DOWN:
            r.setTexture((*textures)[2]);
            break;
        case TileType::STAIRS_UP:
            r.setTexture((*textures)[3]);
            break;
    }
}

Tile::~Tile() {
    textures = nullptr;
    //dtor
}