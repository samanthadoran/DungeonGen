#ifndef TILE_H
#define TILE_H

#include <vector>
#include <SFML/Graphics.hpp>

using std::vector;

enum class TileType {
    WALL,
    FLOOR,
    STAIRS_UP,
    STAIRS_DOWN,
    BLANK
};

class Tile {
public:
    Tile(vector<sf::Texture *> *, int, int, TileType = TileType::BLANK);

    TileType getTileType() const;

    sf::Vector2i getPos() const;

    void setPos(sf::Vector2i);

    char getTextualRepresentation() const;

    sf::RectangleShape getGraphicalRepresentation() const;

    void setTileType(TileType);

    static TileType getTileTypeFromText(char);

    virtual ~Tile();

protected:
private:
    vector<sf::Texture *> *textures;
    sf::RectangleShape r;
    sf::Vector2i pos;
    TileType type;
};

#endif // TILE_H