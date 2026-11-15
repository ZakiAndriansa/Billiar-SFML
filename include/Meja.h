#pragma once
#include "ObjekGame.h"
#include <SFML/Graphics.hpp>

class Meja : public ObjekGame
{
private:
	sf::FloatRect batas;
	sf::Vector2f lubang[6];
	sf::Color warna;
	sf::Texture teksturMeja;
	sf::Sprite spriteMeja;
	sf::Texture teksturLubang[6];
	sf::Sprite spriteLubang[6];
	bool siapMeja;
	bool siapLubang[6];

public:
	Meja();
	void gambar(sf::RenderWindow& w);
};
