#include "Bola.h"
#include <cstdio>
#include <iostream>

Bola::Bola(int nomorAwal, sf::Vector2f posisiAwal, float diameter)
    : ObjekGame(posisiAwal), nomor(nomorAwal), jariJari(diameter / 2.0f)
{
    if (nomor == 0)
    {
        jenis = Putih;
    }
    else if (nomor == 8)
    {
        jenis = Hitam;
    }
    else if (nomor < 8)
    {
        jenis = Polos;
    }
    else
    {
        jenis = Belang;
    }

    char berkas[128];
    std::sprintf(berkas, "assets/textures/highres_bola/bola_%02d_atas_besar.png", nomor);

    siap = tekstur.loadFromFile(berkas);
    if (!siap)
    {
        std::cerr << "[ERROR] Gagal memuat " << berkas << std::endl;
        return;
    }

    tekstur.setSmooth(true);
    sprite.setTexture(tekstur);

    sf::Vector2u ukuran = tekstur.getSize();
    sprite.setOrigin(ukuran.x / 2.0f, ukuran.y / 2.0f);

    float skala = (jariJari * 2.0f) / static_cast<float>(ukuran.x);
    sprite.setScale(skala, skala);
    sprite.setPosition(posisi);
}

int Bola::getNomor() const
{
    return nomor;
}

void Bola::gambar(sf::RenderWindow& w)
{
    if (siap)
    {
        w.draw(sprite);
    }
}
