#include "Bola.h"
#include <cstdio>
#include <cmath>
#include <iostream>

namespace
{
const float PI = 3.14159265f;
const float GESEKAN = 120.0f;       // perlambatan tetap oleh kain meja (piksel/detik^2)
const float REDAMAN = 0.3f;         // perlambatan sebanding kecepatan (per detik)
const float PANTULAN_BOLA = 0.95f;  // koefisien restitusi antarbola

float panjang(sf::Vector2f v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}
}

Bola::Bola(int nomorAwal, sf::Vector2f posisiAwal, float diameter)
    : ObjekGame(posisiAwal), nomor(nomorAwal), jariJari(diameter / 2.0f),
      siapBawah(false), kecepatan(0.0f, 0.0f), masuk(false), jarakTempuh(0.0f)
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

    std::sprintf(berkas, "assets/textures/highres_bola/bola_%02d_bawah_besar.png", nomor);
    siapBawah = teksturBawah.loadFromFile(berkas);
    if (siapBawah)
    {
        teksturBawah.setSmooth(true);
    }
    else
    {
        std::cerr << "[ERROR] Gagal memuat " << berkas << std::endl;
    }

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

JenisBola Bola::getJenis() const
{
    return jenis;
}

float Bola::getJariJari() const
{
    return jariJari;
}

sf::Vector2f Bola::getKecepatan() const
{
    return kecepatan;
}

void Bola::update(float dt)
{
    if (masuk)
    {
        return;
    }

    float laju = panjang(kecepatan);
    if (laju <= 0.0f)
    {
        return;
    }

    posisi += kecepatan * dt;
    jarakTempuh += laju * dt;

    // Gesekan kain mengurangi laju sampai bola berhenti.
    float lajuBaru = laju - GESEKAN * dt - REDAMAN * laju * dt;
    if (lajuBaru <= 0.0f)
    {
        kecepatan = sf::Vector2f(0.0f, 0.0f);
    }
    else
    {
        kecepatan *= lajuBaru / laju;
    }
}

void Bola::beriKecepatan(sf::Vector2f v)
{
    kecepatan = v;
}

void Bola::aturPosisi(sf::Vector2f p)
{
    posisi = p;
}

void Bola::aturUlang(sf::Vector2f p)
{
    posisi = p;
    kecepatan = sf::Vector2f(0.0f, 0.0f);
    masuk = false;
    jarakTempuh = 0.0f;
}

// Tumbukan lenting antara dua bola bermassa sama. Mengembalikan true jika kedua
// bola bersentuhan, dipakai wasit untuk mengetahui bola yang pertama kali tersentuh.
bool Bola::tumbuk(Bola& lain)
{
    if (masuk || lain.masuk)
    {
        return false;
    }

    sf::Vector2f d = lain.posisi - posisi;
    float jarakMin = jariJari + lain.jariJari;
    float jarak2 = d.x * d.x + d.y * d.y;
    if (jarak2 >= jarakMin * jarakMin || jarak2 <= 0.0f)
    {
        return false;
    }

    float jarak = std::sqrt(jarak2);
    sf::Vector2f n = d / jarak;

    // Memisahkan bola yang saling menembus.
    float tembus = jarakMin - jarak;
    posisi -= n * (tembus / 2.0f);
    lain.posisi += n * (tembus / 2.0f);

    float relatif = (kecepatan.x - lain.kecepatan.x) * n.x + (kecepatan.y - lain.kecepatan.y) * n.y;
    if (relatif > 0.0f)
    {
        float impuls = (1.0f + PANTULAN_BOLA) / 2.0f * relatif;
        kecepatan -= n * impuls;
        lain.kecepatan += n * impuls;
    }
    return true;
}

void Bola::masukLubang()
{
    masuk = true;
    kecepatan = sf::Vector2f(0.0f, 0.0f);
}

bool Bola::statusBola() const
{
    return masuk;
}

bool Bola::berhenti() const
{
    return masuk || (kecepatan.x == 0.0f && kecepatan.y == 0.0f);
}

// Animasi frame: tampak atas dan tampak bawah ditampilkan bergantian. Setiap
// setengah putaran bola (jarak pi x jari-jari) frame berganti, sehingga makin
// cepat bola bergerak, makin cepat pula gambarnya berganti.
void Bola::gambar(sf::RenderWindow& w)
{
    if (!siap || masuk)
    {
        return;
    }

    int frame = static_cast<int>(jarakTempuh / (PI * jariJari)) % 2;
    if (frame == 1 && siapBawah)
    {
        sprite.setTexture(teksturBawah);
    }
    else
    {
        sprite.setTexture(tekstur);
    }
    sprite.setPosition(posisi);
    w.draw(sprite);
}
