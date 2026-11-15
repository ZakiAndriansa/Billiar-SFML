#pragma once
#include <SFML/Graphics.hpp>

class ObjekGame
{
protected:
    sf::Vector2f posisi;

public:
    ObjekGame(sf::Vector2f posisiAwal) : posisi(posisiAwal) {}
    virtual ~ObjekGame() {}
    virtual void gambar(sf::RenderWindow& w) = 0;
    sf::Vector2f getPosisi() const { return posisi; }

};
