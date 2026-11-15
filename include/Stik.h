#pragma once
#include "ObjekGame.h"
#include "Bola.h"
#include <SFML/Graphics.hpp>

class Stik : public ObjekGame
{
private:
    float sudut;        
    float kekuatan;     
    sf::Texture tekstur;
    sf::Sprite sprite;
    sf::RectangleShape garisBidik;
    sf::RectangleShape latarIndikator;
    sf::RectangleShape isiIndikator;
    bool siap;

public:
    Stik();
    void bidik(sf::Vector2f arah);
    void aturKekuatan(float k);
    void ikuti(sf::Vector2f posisiBola);
    void pukul(Bola& bolaPutih);
    float getSudut() const;
    float getKekuatan() const;
    void gambar(sf::RenderWindow& w);
    void gambarIndikator(sf::RenderWindow& w);
};

