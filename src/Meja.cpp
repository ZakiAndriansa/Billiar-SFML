#include "Meja.h"
#include <iostream>

namespace
{
const float TANGKAP_SUDUT = 26.0f;   // lubang sudut berjari-jari 30 piksel pada gambar
const float TANGKAP_TENGAH = 22.0f;  // lubang tengah berjari-jari 26 piksel pada gambar
const float ZONA_MULUT = 14.0f;      // jarak tambahan dari lubang yang tidak memiliki tepi
const float PANTULAN_TEPI = 0.8f;    // sisa laju setelah memantul dari tepi
}

Meja::Meja() : ObjekGame(sf::Vector2f(32.0f, 50.0f))
{
    batas.left   = posisi.x + 60.0f;
    batas.top    = posisi.y + 60.0f;
    batas.width  = 840.0f;
    batas.height = 420.0f;

    const float lx[6] = { 60.0f, 480.0f, 900.0f,  60.0f, 480.0f, 900.0f };
    const float ly[6] = { 60.0f,  56.0f,  60.0f, 480.0f, 484.0f, 480.0f };

    for (int i = 0; i < 6; i++)
    {
        lubang[i] = sf::Vector2f(posisi.x + lx[i], posisi.y + ly[i]);
        radiusTangkap[i] = (i == 1 || i == 4) ? TANGKAP_TENGAH : TANGKAP_SUDUT;
        siapLubang[i] = false;
    }
    siapMeja = false;

    aturWarna("hijau");
}

void Meja::aturWarna(const std::string& namaWarna)
{
    std::string folder = "meja_green";
    std::string berkasMeja = "table_green_lubang_transparan.png";
    warna = sf::Color(20, 105, 50);

    if (namaWarna == "biru")
    {
        folder = "meja_blue";
        berkasMeja = "table_blue_lubang_transparan.png";
        warna = sf::Color(20, 85, 135);
    }
    else if (namaWarna == "merah")
    {
        folder = "meja_red";
        berkasMeja = "table_red_lubang_transparan.png";
        warna = sf::Color(120, 25, 35);
    }

    const char* namaLubang[6] = {
        "pocket_1_top_left.png",
        "pocket_2_top_middle.png",
        "pocket_3_top_right.png",
        "pocket_4_bottom_left.png",
        "pocket_5_bottom_middle.png",
        "pocket_6_bottom_right.png"
    };

    for (int i = 0; i < 6; i++)
    {
        std::string pathLubang = "assets/textures/" + folder + "/pockets/" + namaLubang[i];
        siapLubang[i] = teksturLubang[i].loadFromFile(pathLubang);
        if (siapLubang[i])
        {
            teksturLubang[i].setSmooth(true);
            spriteLubang[i].setTexture(teksturLubang[i], true);
            spriteLubang[i].setOrigin(40.0f, 40.0f);
            spriteLubang[i].setPosition(lubang[i]);
        }
        else
        {
            std::cerr << "[ERROR] Gagal memuat " << pathLubang << std::endl;
        }
    }

    std::string pathMeja = "assets/textures/" + folder + "/" + berkasMeja;
    siapMeja = teksturMeja.loadFromFile(pathMeja);
    if (siapMeja)
    {
        teksturMeja.setSmooth(true);
        spriteMeja.setTexture(teksturMeja, true);
        spriteMeja.setPosition(posisi);
    }
    else
    {
        std::cerr << "[ERROR] Gagal memuat tekstur meja: " << pathMeja << std::endl;
    }
}

// Memantulkan bola dari tepi meja. Di mulut lubang tidak ada tepi, sehingga bola
// yang dekat lubang dibiarkan lewat agar bisa masuk.
void Meja::pantulkan(Bola& b)
{
    sf::Vector2f p = b.getPosisi();
    sf::Vector2f v = b.getKecepatan();
    float r = b.getJariJari();

    for (int i = 0; i < 6; i++)
    {
        sf::Vector2f d = p - lubang[i];
        float zona = radiusTangkap[i] + ZONA_MULUT;
        if (d.x * d.x + d.y * d.y < zona * zona)
        {
            return;
        }
    }

    bool berubah = false;
    if (p.x - r < batas.left)
    {
        p.x = batas.left + r;
        if (v.x < 0.0f) v.x = -v.x * PANTULAN_TEPI;
        berubah = true;
    }
    else if (p.x + r > batas.left + batas.width)
    {
        p.x = batas.left + batas.width - r;
        if (v.x > 0.0f) v.x = -v.x * PANTULAN_TEPI;
        berubah = true;
    }

    if (p.y - r < batas.top)
    {
        p.y = batas.top + r;
        if (v.y < 0.0f) v.y = -v.y * PANTULAN_TEPI;
        berubah = true;
    }
    else if (p.y + r > batas.top + batas.height)
    {
        p.y = batas.top + batas.height - r;
        if (v.y > 0.0f) v.y = -v.y * PANTULAN_TEPI;
        berubah = true;
    }

    if (berubah)
    {
        b.aturPosisi(p);
        b.beriKecepatan(v);
    }
}

// Bola masuk jika titik pusatnya sudah berada di dalam lingkaran lubang.
bool Meja::cekLubang(Bola& b)
{
    if (b.statusBola())
    {
        return false;
    }

    sf::Vector2f p = b.getPosisi();
    for (int i = 0; i < 6; i++)
    {
        sf::Vector2f d = p - lubang[i];
        if (d.x * d.x + d.y * d.y < radiusTangkap[i] * radiusTangkap[i])
        {
            b.masukLubang();
            return true;
        }
    }
    return false;
}

sf::Vector2f Meja::posisiLubang(int i) const
{
    return lubang[i];
}

sf::FloatRect Meja::getBatas() const
{
    return batas;
}

void Meja::gambar(sf::RenderWindow& w)
{
    if (siapMeja)
    {
        w.draw(spriteMeja);
    }
    for (int i = 0; i < 6; i++)
    {
        if (siapLubang[i])
        {
            w.draw(spriteLubang[i]);
        }
    }
}
