#pragma once
#include "ObjekGame.h"
#include "Bola.h"
#include <SFML/Graphics.hpp>
#include <string>

class Meja : public ObjekGame
{
private:
	sf::FloatRect batas;
	sf::Vector2f lubang[6];
	float radiusTangkap[6];
	sf::Color warna;
	sf::Texture teksturMeja;
	sf::Sprite spriteMeja;
	sf::Texture teksturLubang[6];
	sf::Sprite spriteLubang[6];
	bool siapMeja;
	bool siapLubang[6];

public:
	Meja();
	void aturWarna(const std::string& namaWarna);
	void pantulkan(Bola& b);
	bool cekLubang(Bola& b);
	sf::Vector2f posisiLubang(int i) const;
	sf::FloatRect getBatas() const;
	void gambar(sf::RenderWindow& w);
};
