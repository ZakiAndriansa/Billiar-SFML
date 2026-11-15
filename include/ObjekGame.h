#pragma once
#include <SFML/Graphics.hpp>

class Objekgame
{
protected:
    sf::Vector2f posisi;

public:
    ObjekGame(sf::Vector2f posisiAwal) : posisi(posisiAwal) {}
    virtual ~ObjekGame() {}
    virtual void gambar(sf::RenderWindows& w) = 0;
};
