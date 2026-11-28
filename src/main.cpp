/*------------------------------------------
PROYEK DPBO: GAME BILLIARD 2D (C++ & SFML)
--------------------------------------------
Tugas Minggu 3 (WBS 3.1): Menggambar meja, lubang, dan bola
Tugas Minggu 3 (WBS 3.3): Membuat garis bidik dan indikator kekuatan
PIC: User (Grafis dan Antarmuka)
Tugas Minggu 3 (WBS 3.2): Membuat gerak, gesekan, dan tumbukan bola
Tugas Minggu 4 (WBS 3.4): Membuat aturan 8-ball, giliran, dan pelanggaran
Tugas Minggu 5 (WBS 4.1): Membuat pemilihan bola sasaran lawan komputer
Tugas Minggu 5 (WBS 4.2): Membuat tingkat Mudah, Sedang, dan Sulit
Tugas Minggu 5 (WBS 5.2): Memasang efek suara dan musik (PengelolaSuara)
Tugas Minggu 6 (WBS 5.4): Penyimpanan skor berkas lokal dan papan skor (PapanSkor)
Tugas Minggu 6 (WBS 5.5): Membuat penempatan bola putih
Tambahan: Pop up kemenangan dengan animasi dan konfeti
PIC: Zaki (Fisika & Gameplay), Fadlan (Audio, Papan Skor, Pengujian), Soleh (UI)
Software: Dev-C++ 5.11
Tgl Pembuatan : 14 November 2026
Tgl Revisi terakhir: 28 November 2026
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
Pemain 1 melawan komputer. Komputer memilih bola sasaran dan
lubang dengan metode ghost ball, membuang lintasan yang terhalang,
lalu memukul dengan galat sesuai tingkat yang dipilih di layar
persiapan pemain. Giliran pertama ditentukan secara acak.
Setelah pelanggaran, lawan mendapat bola di tangan: bola putih
mengikuti mouse dan ditaruh dengan klik kiri pada posisi yang
sah (lingkaran hijau). Pelanggaran saat break membatasinya di
belakang garis kepala. Komputer memilih posisi terbaik sendiri.
Tombol R memulai pertandingan baru.
-------------------------------------------*/

#include <SFML/Graphics.hpp>
#include "Permainan.h"
#include "Pengaturan.h"
#include "PengelolaSuara.h"
#include "PapanSkor.h"
#include "Menu.h"
#include "HUD.h"
#include "PopupPemenang.h"
#include <cstdlib>
#include <ctime>

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));
    sf::RenderWindow window(
        sf::VideoMode(1024, 640),
        "Billiard 8-Ball",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    Pengaturan pengaturan;
    PengelolaSuara suara;
    suara.muatSemua();
    suara.terapkanPengaturan(pengaturan);
    suara.mulaiMusik();

    PapanSkor papanSkor("papan_skor.txt");
    Menu menu(&pengaturan);
    menu.aturPengelolaSuara(&suara);
    menu.aturPapanSkor(&papanSkor);

    Permainan permainan;
    permainan.aturPengelolaSuara(&suara);
    HUD hud;
    PopupPemenang popup;
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
                // Selama pop up kemenangan tampil, hanya pop up yang menerima masukan.
                if (popup.apakahAktif())
                {
                    AksiPopup aksiPopup = popup.prosesEvent(event);
                    if (aksiPopup != POPUP_TIDAK_ADA)
                    {
                        suara.putarEfek(SFX_KLIK_MENU);
                    }
                    if (aksiPopup == POPUP_MAIN_LAGI)
                    {
                        popup.sembunyikan();
                        permainan.mulaiPertandingan();
                    }
                    else if (aksiPopup == POPUP_MENU_UTAMA)
                    {
                        popup.sembunyikan();
                        menu.aturLayar(LAYAR_MENU_UTAMA);
                    }
                    continue;
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                {
                    suara.putarEfek(SFX_KLIK_MENU);
                    menu.aturLayar(LAYAR_JEDA);
                    continue;
                }

                AksiHUD aksiHud = hud.prosesEvent(event);
                if (aksiHud != HUD_TIDAK_ADA)
                {
                    suara.putarEfek(SFX_KLIK_MENU);
                }
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
                    // Nama diambil dari layar Persiapan Pemain: dua nama pada mode
                    // manusia, atau satu nama dan tingkat pada mode komputer.
                    bool lawanKomputer = menu.apakahLawanKomputer();
                    permainan.aturNamaPemain1(menu.getNamaPemain());
                    if (!lawanKomputer)
                    {
                        permainan.aturNamaPemain2(menu.getNamaPemain2());
                    }
                    permainan.aturWarnaMeja(pengaturan.getWarnaMeja());
                    permainan.aturLawanKomputer(lawanKomputer);
                    if (lawanKomputer)
                    {
                        permainan.aturTingkat(static_cast<Tingkat>(menu.getTingkatKesulitan()));
                    }
                    permainan.mulaiPertandingan();
                }
                else if (aksi == AKSI_TERAPKAN_PENGATURAN)
                {
                    permainan.aturWarnaMeja(pengaturan.getWarnaMeja());
                    suara.terapkanPengaturan(pengaturan);
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

            // Pop up kemenangan muncul sekali ketika pertandingan selesai.
            if (permainan.apakahSelesai() && !popup.apakahAktif())
            {
                int poinSkor = 0;
                if (permainan.apakahLawanKomputer() &&
                    permainan.getWasit().pemenang() == &permainan.getPemain1())
                {
                    poinSkor = PapanSkor::hitungSkorAkhir(
                        permainan.getPemain1().getSkor(),
                        permainan.getPemain2().getSkor(),
                        permainan.getTingkat(),
                        permainan.getJumlahPelanggaranP1()
                    );
                    papanSkor.tambahSkor(
                        permainan.getPemain1().getNama(),
                        poinSkor,
                        PapanSkor::namaTingkat(permainan.getTingkat())
                    );
                }
                popup.tampilkan(permainan, poinSkor);
            }
            else if (!permainan.apakahSelesai() && popup.apakahAktif())
            {
                popup.sembunyikan();
            }
            popup.update(dt);
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
            popup.gambar(window);
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
