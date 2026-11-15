#include "Meja.h"
#include <iostream>

Meja::Meja() : ObjekGame(sf::Vector2f(32.0f, 50.0f))
{
    batas.left   = posisi.x + 60.0f;
    batas.top    = posisi.y + 60.0f;
    batas.width  = 840.0f;
    batas.height = 420.0f;

    warna = sf::Color(20, 105, 50);

    const float lx[6] = { 60.0f, 480.0f, 900.0f,  60.0f, 480.0f, 900.0f };
    const float ly[6] = { 60.0f,  56.0f,  60.0f, 480.0f, 484.0f, 480.0f };

    const char* berkasLubang[6] = {
        "assets/textures/meja_green/pockets/pocket_1_top_left.png",
        "assets/textures/meja_green/pockets/pocket_2_top_middle.png",
        "assets/textures/meja_green/pockets/pocket_3_top_right.png",
        "assets/textures/meja_green/pockets/pocket_4_bottom_left.png",
        "assets/textures/meja_green/pockets/pocket_5_bottom_middle.png",
        "assets/textures/meja_green/pockets/pocket_6_bottom_right.png"
    };

    for (int i = 0; i < 6; i++)
    {
        lubang[i] = sf::Vector2f(posisi.x + lx[i], posisi.y + ly[i]);

        siapLubang[i] = teksturLubang[i].loadFromFile(berkasLubang[i]);
        if (siapLubang[i])
        {
            teksturLubang[i].setSmooth(true);
            spriteLubang[i].setTexture(teksturLubang[i]);
            spriteLubang[i].setOrigin(40.0f, 40.0f);  
            spriteLubang[i].setPosition(lubang[i]);
        }
        else
        {
            std::cerr << "[ERROR] Gagal memuat " << berkasLubang[i] << std::endl;
        }
    }

    siapMeja = teksturMeja.loadFromFile("assets/textures/meja_green/table_green_lubang_transparan.png");
    if (siapMeja)
    {
        teksturMeja.setSmooth(true);
        spriteMeja.setTexture(teksturMeja);
        spriteMeja.setPosition(posisi);
    }
    else
    {
        std::cerr << "[ERROR] Gagal memuat tekstur meja!" << std::endl;
    }
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
