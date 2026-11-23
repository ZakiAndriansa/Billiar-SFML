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
#include "Pengaturan.h"
#include "Menu.h"
#include "HUD.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(1024, 640),
        "Billiard 8-Ball",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    Pengaturan pengaturan;
    Menu menu(&pengaturan);
    Permainan permainan;
    HUD hud;
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
                continue;
            }

            if (menu.getLayar() == LAYAR_BERMAIN)
            {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                {
                    menu.aturLayar(LAYAR_JEDA);
                    continue;
                }

                AksiHUD aksiHud = hud.prosesEvent(event);
                if (aksiHud == HUD_BUKA_JEDA)
                {
                    menu.aturLayar(LAYAR_JEDA);
                    continue;
                }
                else if (aksiHud == HUD_BUKA_PENGATURAN)
                {
                    menu.aturLayar(LAYAR_PENGATURAN);
                    continue;
                }
                else if (aksiHud == HUD_ULANG_PERTANDINGAN)
                {
                    permainan.mulaiPertandingan();
                    continue;
                }

                permainan.prosesEvent(event);
            }
            else
            {
                AksiMenu aksi = menu.prosesEvent(event);
                if (aksi == AKSI_KELUAR)
                {
                    window.close();
                }
                else if (aksi == AKSI_MULAI_PERTANDINGAN)
                {
                    permainan.aturNamaPemain1(menu.getNamaPemain());
                    permainan.aturWarnaMeja(pengaturan.getWarnaMeja());
                    permainan.mulaiPertandingan();
                }
                else if (aksi == AKSI_TERAPKAN_PENGATURAN)
                {
                    permainan.aturWarnaMeja(pengaturan.getWarnaMeja());
                }
            }
        }

        // Bidikan yang sedang diisi dibatalkan saat layar lain (jeda, pengaturan,
        // menu) terbuka, supaya pukulan tidak terlepas ketika permainan dilanjutkan.
        if (menu.getLayar() != LAYAR_BERMAIN)
        {
            permainan.batalkanBidikan();
        }

        if (menu.getLayar() == LAYAR_BERMAIN)
        {
            permainan.update(dt);
            hud.perbarui(permainan);
        }
        else
        {
            menu.update(dt);
        }

        window.clear(sf::Color(30, 43, 60));
        if (menu.getLayar() == LAYAR_BERMAIN)
        {
            permainan.gambar(window);
            hud.gambar(window);
        }
        else if (menu.getLayar() == LAYAR_JEDA)
        {
            permainan.gambar(window);
            hud.gambar(window);
            menu.gambar(window);
        }
        else
        {
            menu.gambar(window);
        }
        window.display();
    }

    return 0;
}
