/*------------------------------------------
PROYEK DPBO: GAME BILLIARD 2D (C++ & SFML)
--------------------------------------------
Tugas Minggu 3 (WBS 3.1): Menggambar meja, lubang, dan bola
Software: Dev-C++ 5.11
Tgl Pembuatan : 14 November 2026
Tgl Revisi terakhir: 14 November 2026
--------------------------------------------
Deskripsi:
Menampilkan meja biliar, enam lubang, dan enam belas bola
sesuai susunan rak 8-ball. Belum ada fisika dan aturan main.
-------------------------------------------*/

#include <SFML/Graphics.hpp>
#include "Meja.h"
#include "Bola.h"

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
        "Billiard 8-Ball - WBS 3.1 Menggambar Meja, Lubang, dan Bola",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    const float DIAMETER_BOLA = 32.0f;

    DataBola data[16] = {
        {  0, 270.0f, 270.0f },                 // bola putih
        {  1, 690.0f, 270.0f },                 // baris 1
        {  9, 717.7f, 254.0f }, {  2, 717.7f, 286.0f },             // baris 2
        { 10, 745.4f, 238.0f }, {  8, 745.4f, 270.0f }, {  3, 745.4f, 302.0f },   // baris 3
        { 11, 773.1f, 222.0f }, {  7, 773.1f, 254.0f }, { 14, 773.1f, 286.0f }, {  4, 773.1f, 318.0f }, // baris 4
        {  5, 800.8f, 206.0f }, { 13, 800.8f, 238.0f }, { 15, 800.8f, 270.0f }, {  6, 800.8f, 302.0f }, { 12, 800.8f, 334.0f } // baris 5
    };

    Bola* bola[16];
    for (int i = 0; i < 16; i++)
    {
        bola[i] = new Bola(data[i].nomor, sf::Vector2f(data[i].x, data[i].y), DIAMETER_BOLA);
    }

    Meja meja;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear(sf::Color(20, 20, 20));
        meja.gambar(window);
        for (int i = 0; i < 16; i++)
        {
            bola[i]->gambar(window);
        }
        window.display();
    }

    for (int i = 0; i < 16; i++)
    {
        delete bola[i];
    }

    return 0;
}
