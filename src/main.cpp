/*------------------------------------------
PROYEK DPBO: GAME BILLIARD 2D (C++ & SFML)
--------------------------------------------
Tugas Minggu 3 (WBS 3.1): Menggambar meja, lubang, dan bola
Tugas Minggu 3 (WBS 3.3): Membuat garis bidik dan indikator kekuatan
PIC: User (Grafis dan Antarmuka)
Tugas Minggu 3 (WBS 3.2): Membuat gerak, gesekan, dan tumbukan bola
Tugas Minggu 4 (WBS 3.4): Membuat aturan 8-ball, giliran, dan pelanggaran
PIC: Zaki (Fisika dan Gameplay)
Software: Dev-C++ 5.11
Tgl Pembuatan : 14 November 2026
Tgl Revisi terakhir: 15 November 2026
--------------------------------------------
Deskripsi:
Meja, enam lubang, dan enam belas bola ditampilkan. Stik
mengikuti gerak mouse sebagai arah bidik. Menahan tombol kiri
mouse menambah kekuatan pukulan; indikator di bawah meja
menunjukkan nilainya. Saat tombol dilepas, bola putih dipukul,
lalu semua bola bergerak, melambat karena gesekan, saling
bertumbukan, memantul dari tepi, dan masuk lubang. Bola tampak
menggelinding. Wasit memeriksa setiap pukulan dengan aturan
8-ball untuk Pemain 1 dan Pemain 2; hasil, skor, dan giliran
dicetak di konsol sampai tampilan skor dibuat (WBS 5.3).
Tombol R memulai pertandingan baru.
-------------------------------------------*/

#include <SFML/Graphics.hpp>
#include "Permainan.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(1024, 640),
        "Billiard 8-Ball - WBS 3.1 sampai 3.4",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    Permainan permainan;
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
            else
            {
                permainan.prosesEvent(event);
            }
        }

        permainan.update(dt);

        window.clear(sf::Color(20, 20, 20));
        permainan.gambar(window);
        window.display();
    }

    return 0;
}
