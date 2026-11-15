#pragma once
#include <SFML/Graphics.hpp>

class ObjekGame
{
protected:
    sf::Vector2f posisi;

public:
    ObjekGame(sf::Vector2f posisiAwal) : posisi(posisiAwal) {}
    virtual ~ObjekGame() {}
    // Pembaruan bawaan setiap langkah waktu; objek yang tidak bergerak tidak perlu mengisinya.
    virtual void update(float) {}
    virtual void gambar(sf::RenderWindow& w) = 0;
    sf::Vector2f getPosisi() const { return posisi; }

};
