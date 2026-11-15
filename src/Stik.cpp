#include "Stik.h"
#include <iostream>
#include <cmath>

namespace
{
const float PI = 3.14159265f;
const float PANJANG_STIK = 360.0f;   // panjang stik di layar (piksel)
const float JARAK_MIN = 22.0f;       // jarak ujung stik ke titik pusat bola putih
const float TARIKAN_MAKS = 35.0f;    // tambahan tarikan saat kekuatan 100
const float PANJANG_GARIS = 300.0f;  // panjang garis bidik
const float LAJU_MAKS = 1800.0f;     // laju bola putih saat kekuatan 100 (piksel/detik)
}

Stik::Stik() : ObjekGame(sf::Vector2f(270.0f, 270.0f))
{
    sudut = 0.0f;
    kekuatan = 0.0f;

    siap = tekstur.loadFromFile("assets/textures/cue_stick_flat.png");
    if (siap)
    {
        tekstur.setSmooth(true);
        sprite.setTexture(tekstur);

        sf::Vector2u ukuran = tekstur.getSize();
        // Ujung kiri gambar adalah ujung pemukul (berkapur biru), ujung kanan adalah
        // pegangan. Titik asal di ujung pemukul agar ujung itulah yang menempel ke bola.
        sprite.setOrigin(0.0f, ukuran.y / 2.0f);

        float skala = PANJANG_STIK / static_cast<float>(ukuran.x);
        sprite.setScale(skala, skala);
    }
    else
    {
        std::cerr << "[ERROR] Gagal memuat tekstur stik!" << std::endl;
    }

    garisBidik.setSize(sf::Vector2f(PANJANG_GARIS, 1.5f));
    garisBidik.setOrigin(0.0f, 0.75f);
    garisBidik.setFillColor(sf::Color(255, 255, 255, 150));

    latarIndikator.setPosition(32.0f, 604.0f);
    latarIndikator.setSize(sf::Vector2f(300.0f, 20.0f));
    latarIndikator.setFillColor(sf::Color(40, 40, 40));
    latarIndikator.setOutlineThickness(2.0f);
    latarIndikator.setOutlineColor(sf::Color(200, 200, 200));

    isiIndikator.setPosition(34.0f, 606.0f);
    isiIndikator.setSize(sf::Vector2f(0.0f, 16.0f));
    isiIndikator.setFillColor(sf::Color(60, 200, 90));
}

void Stik::bidik(sf::Vector2f arah)
{
    sudut = std::atan2(arah.y, arah.x) * 180.0f / PI;
    // Badan stik mengarah ke belakang bola, berlawanan dengan arah pukulan.
    sprite.setRotation(sudut + 180.0f);
    garisBidik.setRotation(sudut);
}

void Stik::aturKekuatan(float k)
{
    if (k < 0.0f)
    {
        kekuatan = 0.0f;
    }
    else if (k > 100.0f)
    {
        kekuatan = 100.0f;
    }
    else
    {
        kekuatan = k;
    }

    isiIndikator.setSize(sf::Vector2f(296.0f * kekuatan / 100.0f, 16.0f));

    sf::Uint8 merah = static_cast<sf::Uint8>(60 + 195 * kekuatan / 100.0f);
    sf::Uint8 hijau = static_cast<sf::Uint8>(200 - 160 * kekuatan / 100.0f);
    isiIndikator.setFillColor(sf::Color(merah, hijau, 60));
}

void Stik::ikuti(sf::Vector2f posisiBola)
{
    posisi = posisiBola;

    float radian = sudut * PI / 180.0f;
    sf::Vector2f arah(std::cos(radian), std::sin(radian));

    float tarik = JARAK_MIN + TARIKAN_MAKS * kekuatan / 100.0f;
    sprite.setPosition(posisi - arah * tarik);

    garisBidik.setPosition(posisi);
}

// Memukul bola putih searah garis bidik. Kekuatan 100 menghasilkan laju terbesar.
void Stik::pukul(Bola& bolaPutih)
{
    float radian = sudut * PI / 180.0f;
    sf::Vector2f arah(std::cos(radian), std::sin(radian));
    bolaPutih.beriKecepatan(arah * (LAJU_MAKS * kekuatan / 100.0f));
}

float Stik::getSudut() const
{
    return sudut;
}

float Stik::getKekuatan() const
{
    return kekuatan;
}

void Stik::gambar(sf::RenderWindow& w)
{
    if (siap)
    {
        w.draw(sprite);
    }
    w.draw(garisBidik);
}

void Stik::gambarIndikator(sf::RenderWindow& w)
{
    w.draw(latarIndikator);
    w.draw(isiIndikator);
}
