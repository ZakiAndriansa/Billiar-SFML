#include "HUD.h"
#include <cstdio>

HUD::HUD() : fontSiap(false), jenisSudahDitentukan(false)
{
    fontSiap = font.loadFromFile("assets/fonts/LemonDays.ttf");
    if (!fontSiap)
    {
        fontSiap = font.loadFromFile("assets/fonts/arialbd.ttf");
    }

    if (texTopStrip.loadFromFile("assets/ui/hud_top_strip.png"))
    {
        sprTopStrip.setTexture(texTopStrip);
        sprTopStrip.setPosition(0.0f, 0.0f);
    }
    if (texBottomBar.loadFromFile("assets/ui/hud_bottom_bar.png"))
    {
        sprBottomBar.setTexture(texBottomBar);
        sprBottomBar.setPosition(0.0f, 594.0f);
    }
    texAvatarP1Active.loadFromFile("assets/ui/hud_avatar_p1_active.png");
    texAvatarP1Active.setSmooth(true);
    texAvatarP1Idle.loadFromFile("assets/ui/hud_avatar_p1_idle.png");
    texAvatarP1Idle.setSmooth(true);
    sprAvatarP1.setTexture(texAvatarP1Active);
    sprAvatarP1.setPosition(59.0f, 1.0f);

    texAvatarP2Active.loadFromFile("assets/ui/hud_avatar_ai_active.png");
    texAvatarP2Active.setSmooth(true);
    texAvatarP2Idle.loadFromFile("assets/ui/hud_avatar_ai_idle.png");
    texAvatarP2Idle.setSmooth(true);
    sprAvatarP2.setTexture(texAvatarP2Idle);
    sprAvatarP2.setPosition(911.0f, 1.0f);
    if (texModeBox.loadFromFile("assets/ui/hud_mode_box.png"))
    {
        texModeBox.setSmooth(true);
        sprModeBox.setTexture(texModeBox);
        sprModeBox.setPosition(436.0f, 2.0f);
    }
    if (texPillPolos.loadFromFile("assets/ui/hud_pill_polos.png"))
    {
        texPillPolos.setSmooth(true);
        sprPillPolos.setTexture(texPillPolos);
    }
    if (texPillBelang.loadFromFile("assets/ui/hud_pill_belang.png"))
    {
        texPillBelang.setSmooth(true);
        sprPillBelang.setTexture(texPillBelang);
    }

    for (int n = 1; n <= 15; n++)
    {
        if (n == 8) continue;
        char berkas[128];
        std::sprintf(berkas, "assets/textures/highres_bola/bola_%02d_atas_besar.png", n);
        if (texBolaMini[n].loadFromFile(berkas))
        {
            texBolaMini[n].setSmooth(true);
            sprBolaMini[n].setTexture(texBolaMini[n]);
            sf::Vector2u uk = texBolaMini[n].getSize();
            float skala = 21.0f / static_cast<float>(uk.x);
            sprBolaMini[n].setScale(skala, skala);
        }
    }

    btnMenu.muat("assets/ui/hud_btn_menu.png", "", font, "", 12, sf::Vector2f(10.0f, 5.0f));
    btnPengaturan.muat("assets/ui/hud_btn_settings.png", "", font, "", 12, sf::Vector2f(10.0f, 32.0f));
    btnUlang.muat("assets/ui/hud_btn_restart.png", "", font, "", 12, sf::Vector2f(972.0f, 5.0f));
    btnJeda.muat("assets/ui/hud_btn_jeda_sm.png", "", font, "JEDA", 11, sf::Vector2f(970.0f, 30.0f));

    aturTeks(teksNamaP1, "PEMAIN 1", 15, sf::Color::White, 2.0f);
    teksNamaP1.setPosition(120.0f, 6.0f);

    aturTeks(teksNamaP2, "KOMPUTER", 15, sf::Color(185, 202, 224), 2.0f);

    aturTeks(teksPillP1, "POLOS", 11, sf::Color::White, 1.0f);
    aturTeks(teksPillP2, "BELANG", 11, sf::Color::White, 1.0f);

    aturTeks(teksModeAtas, "8-BALL", 14, sf::Color::White, 2.0f);
    sf::FloatRect bm = teksModeAtas.getLocalBounds();
    teksModeAtas.setOrigin(bm.left + bm.width / 2.0f, bm.top + bm.height / 2.0f);
    teksModeAtas.setPosition(512.0f, 17.0f);

    aturTeks(teksGiliranBawah, "GILIRAN P1", 14, sf::Color(255, 210, 0), 2.0f);

    aturTeks(teksLabelKekuatan, "KEKUATAN", 14, sf::Color::White, 2.0f);
    teksLabelKekuatan.setPosition(26.0f, 608.0f);

    barKekuatan.setPosition(116.0f, 611.0f);
    barKekuatan.setSize(sf::Vector2f(0.0f, 12.0f));
    barKekuatan.setFillColor(sf::Color(255, 180, 0));

    aturTeks(teksPersenKekuatan, "0%", 15, sf::Color(255, 210, 0), 2.0f);
    teksPersenKekuatan.setPosition(338.0f, 607.0f);

    aturTeks(teksStatusWasit, "Tahan klik kiri untuk memukul", 13, sf::Color::White, 1.5f);

    aturTeks(teksPintasan, "ESC: JEDA   R: ULANG", 13, sf::Color(185, 202, 224), 1.5f);
    sf::FloatRect bp = teksPintasan.getLocalBounds();
    teksPintasan.setPosition(1010.0f - bp.width, 609.0f);
}

void HUD::aturTeks(sf::Text& t, const std::string& isi, unsigned int ukuran, sf::Color warna, float outline)
{
    t.setFont(font);
    t.setString(isi);
    t.setCharacterSize(ukuran);
    t.setFillColor(warna);
    t.setOutlineColor(sf::Color::Black);
    t.setOutlineThickness(outline);
}

AksiHUD HUD::prosesEvent(const sf::Event& e)
{
    if (btnMenu.diklik(e) || btnJeda.diklik(e))
    {
        return HUD_BUKA_JEDA;
    }
    if (btnPengaturan.diklik(e))
    {
        return HUD_BUKA_PENGATURAN;
    }
    if (btnUlang.diklik(e))
    {
        return HUD_ULANG_PERTANDINGAN;
    }
    return HUD_TIDAK_ADA;
}

void HUD::perbarui(const Permainan& permainan)
{
    const Pemain& p1 = permainan.getPemain1();
    const Pemain& p2 = permainan.getPemain2();
    const Wasit& wasit = permainan.getWasit();

    teksNamaP1.setString(p1.getNama());
    teksNamaP2.setString(p2.getNama());

    bool giliranP1 = (wasit.getGiliran() == &p1);
    sprAvatarP1.setTexture(giliranP1 ? texAvatarP1Active : texAvatarP1Idle);
    // Avatar komputer hanya untuk lawan komputer; pada mode 1 lawan 1 manusia,
    // Pemain 2 memakai avatar manusia yang sama dengan Pemain 1.
    if (p2.apakahKomputer())
    {
        sprAvatarP2.setTexture(giliranP1 ? texAvatarP2Idle : texAvatarP2Active);
    }
    else
    {
        sprAvatarP2.setTexture(giliranP1 ? texAvatarP1Idle : texAvatarP1Active);
    }
    teksNamaP1.setFillColor(giliranP1 ? sf::Color::White : sf::Color(185, 202, 224));
    teksNamaP2.setFillColor(!giliranP1 ? sf::Color::White : sf::Color(185, 202, 224));

    sf::FloatRect b2 = teksNamaP2.getLocalBounds();
    teksNamaP2.setPosition(902.0f - b2.width, 6.0f);

    // Pill jenis bola P1 & P2 (posisi dinamis di sebelah kanan/kiri nama)
    JenisBola j1 = p1.getJenisBola();
    JenisBola j2 = p2.getJenisBola();
    sf::FloatRect b1 = teksNamaP1.getLocalBounds();
    float pill1X = 120.0f + b1.width + 12.0f;
    float pill2X = 902.0f - b2.width - 86.0f;

    if (j1 == Polos)
    {
        sprPillPolos.setPosition(pill1X, 7.0f);
        teksPillP1.setString("POLOS");
        teksPillP1.setFillColor(sf::Color::White);
        teksPillP1.setPosition(pill1X + 14.0f, 9.0f);
    }
    else if (j1 == Belang)
    {
        sprPillBelang.setPosition(pill1X, 7.0f);
        teksPillP1.setString("BELANG");
        teksPillP1.setFillColor(sf::Color::White);
        teksPillP1.setPosition(pill1X + 12.0f, 9.0f);
    }
    else
    {
        teksPillP1.setString("");
    }

    if (j2 == Polos)
    {
        sprPillPolos.setPosition(pill2X, 7.0f);
        teksPillP2.setString("POLOS");
        teksPillP2.setFillColor(sf::Color::White);
        teksPillP2.setPosition(pill2X + 14.0f, 9.0f);
    }
    else if (j2 == Belang)
    {
        sprPillBelang.setPosition(pill2X, 7.0f);
        teksPillP2.setString("BELANG");
        teksPillP2.setFillColor(sf::Color::White);
        teksPillP2.setPosition(pill2X + 12.0f, 9.0f);
    }
    else
    {
        teksPillP2.setString("");
    }

    // Posisi deretan bola mini hanya tampil setelah jenis bola ditentukan (bukan Putih)
    jenisSudahDitentukan = (j1 == Polos || j1 == Belang);
    if (jenisSudahDitentukan)
    {
        bool p1Belang = (j1 == Belang);
        int idxKiri = 0;
        int idxKanan = 0;
        for (int i = 1; i < 16; i++)
        {
            const Bola* b = permainan.getBola(i);
            if (!b) continue;
            int num = b->getNomor();
            if (num == 8) continue;

            bool masuk = b->statusBola();
            sprBolaMini[num].setColor(masuk ? sf::Color(255, 255, 255, 70) : sf::Color::White);

            bool keKiri = p1Belang ? (num > 8) : (num < 8);
            if (keKiri)
            {
                sprBolaMini[num].setPosition(120.0f + idxKiri * 25.0f, 30.0f);
                idxKiri++;
            }
            else
            {
                sprBolaMini[num].setPosition(728.0f + idxKanan * 25.0f, 30.0f);
                idxKanan++;
            }
        }
    }
    else
    {
        // Meja masih terbuka: list bola dan badge jenis bola 100% kosong
        teksPillP1.setString("");
        teksPillP2.setString("");
    }

    // Kotak tengah giliran / pemenang
    if (permainan.apakahSelesai() && wasit.pemenang())
    {
        teksModeAtas.setString("MENANG");
        teksGiliranBawah.setString(wasit.pemenang()->getNama());
    }
    else
    {
        teksModeAtas.setString("8-BALL");
        teksGiliranBawah.setString(giliranP1 ? "GILIRAN P1" : "GILIRAN P2");
    }
    sf::FloatRect bm = teksModeAtas.getLocalBounds();
    teksModeAtas.setOrigin(bm.left + bm.width / 2.0f, bm.top + bm.height / 2.0f);
    teksModeAtas.setPosition(512.0f, 17.0f);

    sf::FloatRect bg = teksGiliranBawah.getLocalBounds();
    teksGiliranBawah.setOrigin(bg.left + bg.width / 2.0f, bg.top + bg.height / 2.0f);
    teksGiliranBawah.setPosition(512.0f, 37.0f);

    // Bar kekuatan
    float k = permainan.getKekuatan();
    barKekuatan.setSize(sf::Vector2f(208.0f * (k / 100.0f), 12.0f));
    sf::Uint8 r = static_cast<sf::Uint8>(255);
    sf::Uint8 g = static_cast<sf::Uint8>(225 - 130 * (k / 100.0f));
    barKekuatan.setFillColor(sf::Color(r, g, 0));

    char pbuf[16];
    std::sprintf(pbuf, "%d%%", static_cast<int>(k + 0.5f));
    teksPersenKekuatan.setString(pbuf);

    // Banner status wasit di strip bawah
    std::string ket = wasit.getKeterangan();
    if (permainan.apakahSelesai() && wasit.pemenang())
    {
        ket = "Pemenang: " + wasit.pemenang()->getNama() + " - Tekan R untuk main lagi";
    }
    else if (permainan.apakahMenaruhBola())
    {
        // Bola di tangan: pesan pelanggaran tetap terlihat, ditambah petunjuk menaruh bola.
        std::string nama = wasit.getGiliran()->getNama();
        std::string petunjuk = wasit.getGiliran()->apakahKomputer()
            ? nama + " menaruh bola putih..."
            : "Bola di tangan, klik kiri untuk menaruh.";
        ket = ket.empty() ? petunjuk : ket + " " + petunjuk;
    }
    else if (ket.empty())
    {
        // Petunjuk mouse hanya untuk pemain manusia; saat giliran komputer cukup
        // ditampilkan bahwa komputer sedang membidik.
        if (wasit.getGiliran()->apakahKomputer())
        {
            ket = "Giliran " + wasit.getGiliran()->getNama() + " - sedang membidik...";
        }
        else
        {
            ket = "Giliran " + wasit.getGiliran()->getNama() + " - Tahan klik kiri untuk memukul";
        }
    }
    teksStatusWasit.setString(ket);
    sf::FloatRect bs = teksStatusWasit.getLocalBounds();
    // Pesan yang panjang diperkecil agar tetap muat di dalam kotak status.
    const float LEBAR_MAKS = 425.0f;
    float skala = (bs.width > LEBAR_MAKS) ? LEBAR_MAKS / bs.width : 1.0f;
    teksStatusWasit.setScale(skala, skala);
    teksStatusWasit.setOrigin(bs.left + bs.width / 2.0f, bs.top + bs.height / 2.0f);
    teksStatusWasit.setPosition(623.0f, 616.0f);
}

void HUD::gambar(sf::RenderWindow& w) const
{
    w.draw(sprTopStrip);
    btnMenu.gambar(w);
    btnPengaturan.gambar(w);
    btnUlang.gambar(w);
    btnJeda.gambar(w);

    w.draw(sprAvatarP1);
    w.draw(sprAvatarP2);
    w.draw(sprModeBox);

    w.draw(teksNamaP1);
    w.draw(teksNamaP2);

    if (jenisSudahDitentukan)
    {
        if (!teksPillP1.getString().isEmpty())
        {
            w.draw(teksPillP1.getString() == "POLOS" ? sprPillPolos : sprPillBelang);
            w.draw(teksPillP1);
        }
        if (!teksPillP2.getString().isEmpty())
        {
            w.draw(teksPillP2.getString() == "POLOS" ? sprPillPolos : sprPillBelang);
            w.draw(teksPillP2);
        }

        for (int n = 1; n <= 15; n++)
        {
            if (n == 8) continue;
            w.draw(sprBolaMini[n]);
        }
    }

    w.draw(teksModeAtas);
    w.draw(teksGiliranBawah);

    w.draw(sprBottomBar);
    w.draw(teksLabelKekuatan);
    w.draw(barKekuatan);
    w.draw(teksPersenKekuatan);
    w.draw(teksStatusWasit);
    w.draw(teksPintasan);
}
