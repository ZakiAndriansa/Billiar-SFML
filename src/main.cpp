/*------------------------------------------
PROYEK DPBO: GAME BILLIARD 2D (C++ & SFML)
--------------------------------------------
Tugas Minggu 3 (WBS 3.1): Menggambar meja, lubang, dan bola
Tugas Minggu 3 (WBS 3.3): Membuat garis bidik dan indikator kekuatan
PIC: User (Grafis dan Antarmuka)
Software: Dev-C++ 5.11
Tgl Pembuatan : 14 November 2026
Tgl Revisi terakhir: 15 November 2026
--------------------------------------------
Deskripsi:
Meja, enam lubang, dan enam belas bola ditampilkan. Stik
mengikuti gerak mouse sebagai arah bidik. Menahan tombol kiri
mouse menambah kekuatan pukulan; indikator di bawah meja
menunjukkan nilainya. Fisika bola belum ada (WBS 3.2).
-------------------------------------------*/

#include <SFML/Graphics.hpp>
#include "Meja.h"
#include "Bola.h"
#include "Stik.h"
#include <iostream>

struct DataBola
{
    int nomor;
    float x;
    float y;
};

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(1024, 640),
        "Billiard 8-Ball - WBS 3.1 dan 3.3 Menggambar Objek, Garis Bidik, dan Kekuatan",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    const float DIAMETER_BOLA = 32.0f;
    const float KEKUATAN_PER_DETIK = 70.0f;

    // Posisi 16 bola: 1 bola putih di garis kepala, 15 bola di rak segitiga.
    DataBola data[16] = {
        {  0, 302.0f, 320.0f },
        {  1, 722.0f, 320.0f },
        {  9, 749.7f, 304.0f }, {  2, 749.7f, 336.0f },
        { 10, 777.4f, 288.0f }, {  8, 777.4f, 320.0f }, {  3, 777.4f, 352.0f },
        { 11, 805.1f, 272.0f }, {  7, 805.1f, 304.0f }, { 14, 805.1f, 336.0f }, {  4, 805.1f, 368.0f },
        {  5, 832.8f, 256.0f }, { 13, 832.8f, 288.0f }, { 15, 832.8f, 320.0f }, {  6, 832.8f, 352.0f }, { 12, 832.8f, 384.0f }
    };

    Bola* bola[16];
    for (int i = 0; i < 16; i++)
    {
        bola[i] = new Bola(data[i].nomor, sf::Vector2f(data[i].x, data[i].y), DIAMETER_BOLA);
    }

    Meja meja;
    Stik stik;

    float kekuatan = 0.0f;
    bool menahan = false;

    sf::Clock jam;

    while (window.isOpen())
    {
        float dt = jam.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed &&
                     event.mouseButton.button == sf::Mouse::Left)
            {
                menahan = true;
            }
            else if (event.type == sf::Event::MouseButtonReleased &&
                     event.mouseButton.button == sf::Mouse::Left)
            {
                menahan = false;
                // Fisika belum ada (WBS 3.2), jadi hasilnya baru dilaporkan di konsol
                std::cout << "Pukulan: sudut " << stik.getSudut()
                          << " derajat, kekuatan " << stik.getKekuatan() << std::endl;
                kekuatan = 0.0f;
            }
        }

        sf::Vector2i pikselMouse = sf::Mouse::getPosition(window);
        sf::Vector2f posisiMouse(static_cast<float>(pikselMouse.x),
                                 static_cast<float>(pikselMouse.y));
        sf::Vector2f arahBidik = posisiMouse - bola[0]->getPosisi();

        if (menahan)
        {
            kekuatan += KEKUATAN_PER_DETIK * dt;
            if (kekuatan > 100.0f)
            {
                kekuatan = 100.0f;
            }
        }

        stik.bidik(arahBidik);
        stik.aturKekuatan(kekuatan);
        stik.ikuti(bola[0]->getPosisi());

        window.clear(sf::Color(20, 20, 20));
        meja.gambar(window);
        for (int i = 0; i < 16; i++)
        {
            bola[i]->gambar(window);
        }
        stik.gambar(window);
        stik.gambarIndikator(window);
        window.display();
    }

    for (int i = 0; i < 16; i++)
    {
        delete bola[i];
    }

    return 0;
}
