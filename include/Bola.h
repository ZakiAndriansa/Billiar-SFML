#pragma once
#include "ObjekGame.h"
#include <SFML/Graphics.hpp>

enum JenisBola {Putih, Polos, Belang, Hitam
};

class Bola : public ObjekGame{
	private:
		int nomor;
		JenisBola jenis;
		float jariJari;
		sf::Texture tekstur;
		sf::Sprite sprite;
		bool siap;
		
	public:
		Bola(int nomorAwal, sf::Vector2f posisiAwal, float diameter);
		int getNomor() const;
		void gambar(sf::RenderWindow& w);
};
