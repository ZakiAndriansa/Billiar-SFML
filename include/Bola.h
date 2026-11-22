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
		sf::Texture tekstur;        // tampak atas, sisi yang memuat angka
		sf::Texture teksturBawah;   // tampak bawah, sisi tanpa angka
		sf::Sprite sprite;
		bool siap;
		bool siapBawah;
		sf::Vector2f kecepatan;
		bool masuk;
		float jarakTempuh;          // jarak gelinding, menentukan frame animasi

	public:
		Bola(int nomorAwal, sf::Vector2f posisiAwal, float diameter);
		int getNomor() const;
		JenisBola getJenis() const;
		float getJariJari() const;
		sf::Vector2f getKecepatan() const;
		void update(float dt);
		void beriKecepatan(sf::Vector2f v);
		void aturPosisi(sf::Vector2f p);
		void aturUlang(sf::Vector2f p);
		bool tumbuk(Bola& lain);
		void masukLubang();
		bool statusBola() const;
		bool berhenti() const;
		void gambar(sf::RenderWindow& w);
};
