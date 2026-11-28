#include "Menu.h"
#include "PengelolaSuara.h"
#include "PapanSkor.h"
#include <cstdio>
#include <iostream>

namespace
{
const float SLIDER_X = 78.0f;
const float SLIDER_W = 770.0f;
const float SLIDER_SFX_Y = 338.0f;
const float SLIDER_BGM_Y = 428.0f;
}

Menu::Menu(Pengaturan* cfg)
    : layar(LAYAR_MENU_UTAMA),
      layarSebelumPengaturan(LAYAR_MENU_UTAMA),
      fontSiap(false),
      namaPemain("Pemain 1"),
      tingkatKesulitan(1),
      lawanKomputer(true),
      inputNamaAktif(true),
      namaPemain2("Pemain 2"),
      inputNama2Aktif(false),
      waktuKursor(0.0f),
      pengaturan(cfg),
      draftWarna("hijau"),
      draftSfx(80),
      draftBgm(60),
      sliderAktif(-1),
      papanSkor(0),
      suara(0)
{
    fontSiap = font.loadFromFile("assets/fonts/LemonDays.ttf");
    if (!fontSiap)
    {
        fontSiap = font.loadFromFile("assets/fonts/arialbd.ttf");
    }

    if (texBgArcade.loadFromFile("assets/ui/bg_arcade.png"))
    {
        sprBgArcade.setTexture(texBgArcade);
    }
    if (texBgMainMenu.loadFromFile("assets/ui/bg_main_menu.png"))
    {
        sprBgMainMenu.setTexture(texBgMainMenu);
    }
    if (texPapanJeda.loadFromFile("assets/ui/wooden_board_pause.png"))
    {
        texPapanJeda.setSmooth(true);
        sprPapanJeda.setTexture(texPapanJeda);
        sprPapanJeda.setPosition(242.0f, 98.0f);
    }
    if (texInputBox.loadFromFile("assets/ui/input_box.png"))
    {
        texInputBox.setSmooth(true);
        sprInputBox.setTexture(texInputBox);
        sprInputBox.setPosition(76.0f, 162.0f);
    }
    if (texPensil.loadFromFile("assets/ui/icon_pencil.png"))
    {
        texPensil.setSmooth(true);
        sprPensil.setTexture(texPensil);
        sprPensil.setPosition(902.0f, 177.0f);
    }
    if (texGemCyan.loadFromFile("assets/ui/deco_gem_cyan.png"))
    {
        texGemCyan.setSmooth(true);
        sprGemCyan.setTexture(texGemCyan);
        sprGemCyan.setPosition(867.0f, 32.0f);
    }
    if (texGemPink.loadFromFile("assets/ui/deco_gem_pink.png"))
    {
        texGemPink.setSmooth(true);
        sprGemPink.setTexture(texGemPink);
        sprGemPink.setPosition(908.0f, 66.0f);
    }
    if (texStarCluster.loadFromFile("assets/ui/deco_star_cluster.png"))
    {
        texStarCluster.setSmooth(true);
        sprStarCluster.setTexture(texStarCluster);
        sprStarCluster.setPosition(70.0f, 30.0f);
    }
    if (texChalkBlue.loadFromFile("assets/ui/deco_chalk_blue.png"))
    {
        texChalkBlue.setSmooth(true);
        sprChalkBlue.setTexture(texChalkBlue);
        sprChalkBlue.setPosition(890.0f, 38.0f);
    }
    if (texCueDeco.loadFromFile("assets/ui/deco_cue_stick.png"))
    {
        texCueDeco.setSmooth(true);
        sprCueDeco.setTexture(texCueDeco);
        sprCueDeco.setPosition(870.0f, 480.0f);
    }
    if (texStarBadge.loadFromFile("assets/ui/icon_star_badge.png"))
    {
        texStarBadge.setSmooth(true);
        sprStarBadge.setTexture(texStarBadge);
    }

    // --- Menu Utama (posisi tombol persis Main-Title-Utama.png) ---
    btnMulaiMain.muat("assets/ui/btn_488x74_yellow_normal.png", "assets/ui/btn_488x74_yellow_hover.png",
                      font, "MULAI MAIN", 36, sf::Vector2f(264.0f, 258.0f));
    btnPengaturan.muat("assets/ui/btn_488x74_blue_normal.png", "assets/ui/btn_488x74_blue_hover.png",
                       font, "PENGATURAN", 36, sf::Vector2f(264.0f, 342.0f));
    btnPapanSkor.muat("assets/ui/btn_488x74_blue_normal.png", "assets/ui/btn_488x74_blue_hover.png",
                      font, "PAPAN SKOR", 36, sf::Vector2f(264.0f, 426.0f));
    btnKeluar.muat("assets/ui/btn_488x74_dark_normal.png", "assets/ui/btn_488x74_dark_hover.png",
                   font, "KELUAR", 36, sf::Vector2f(264.0f, 510.0f));

    // --- Pilih Mode ---
    aturTeksJudul(teksJudulMode, "PILIH MODE", 60, sf::Vector2f(512.0f, 72.0f));
    btnModeManusia.muat("assets/ui/btn_488x74_yellow_normal.png", "assets/ui/btn_488x74_yellow_hover.png",
                        font, "1 VS 1 MANUSIA", 36, sf::Vector2f(264.0f, 160.0f));
    aturTeksLabel(teksDescManusia, "Dua pemain bergantian memukul di satu komputer", 20, sf::Vector2f(0.0f, 0.0f));
    btnModeKomputer.muat("assets/ui/btn_488x74_blue_normal.png", "assets/ui/btn_488x74_blue_hover.png",
                         font, "LAWAN KOMPUTER", 36, sf::Vector2f(264.0f, 300.0f));
    aturTeksLabel(teksDescKomputer, "Pilih tingkat Mudah, Sedang, atau Sulit", 20, sf::Vector2f(0.0f, 0.0f));
    btnModeKembali.muat("assets/ui/btn_488x74_dark_normal.png", "assets/ui/btn_488x74_dark_hover.png",
                        font, "KEMBALI", 36, sf::Vector2f(264.0f, 480.0f));
    {
        sf::FloatRect b = teksDescManusia.getLocalBounds();
        teksDescManusia.setPosition(512.0f - b.width / 2.0f - b.left, 246.0f);
        b = teksDescKomputer.getLocalBounds();
        teksDescKomputer.setPosition(512.0f - b.width / 2.0f - b.left, 386.0f);
    }

    // --- Setup Pemain ---
    aturTeksJudul(teksJudulSetup, "PERSIAPAN PEMAIN", 58, sf::Vector2f(512.0f, 66.0f));
    aturTeksLabel(teksLabelNama, "Nama Pemain:", 24, sf::Vector2f(78.0f, 124.0f));
    aturTeksLabel(teksLabelKesulitan, "Pilih Tingkat Kesulitan Lawan Komputer (AI):", 24, sf::Vector2f(78.0f, 240.0f));

    teksInputNama.setFont(font);
    teksInputNama.setCharacterSize(26);
    teksInputNama.setFillColor(sf::Color::White);
    teksInputNama.setOutlineColor(sf::Color::Black);
    teksInputNama.setOutlineThickness(2.0f);
    teksInputNama.setPosition(100.0f, 176.0f);
    teksInputNama.setString(namaPemain);

    kursorInput.setSize(sf::Vector2f(3.0f, 30.0f));
    kursorInput.setFillColor(sf::Color(255, 210, 0));

    // Kolom nama kedua untuk mode 1 lawan 1 manusia, menempati tempat pilihan tingkat.
    aturTeksLabel(teksLabelNama2, "Nama Pemain 2:", 24, sf::Vector2f(78.0f, 240.0f));
    sprInputBox2 = sprInputBox;
    sprInputBox2.setPosition(76.0f, 278.0f);
    sprPensil2 = sprPensil;
    sprPensil2.setPosition(902.0f, 293.0f);
    teksInputNama2 = teksInputNama;
    teksInputNama2.setPosition(100.0f, 292.0f);
    teksInputNama2.setString(namaPemain2);
    kursorInput2 = kursorInput;

    const char* namaDiff[3] = { "mudah", "sedang", "sulit" };
    const char* labelDiff[3] = { "MUDAH", "SEDANG", "SULIT" };
    for (int i = 0; i < 3; i++)
    {
        std::string pNorm = std::string("assets/ui/card_diff_") + namaDiff[i] + "_normal.png";
        std::string pAkt = std::string("assets/ui/card_diff_") + namaDiff[i] + "_active.png";
        texDiffNormal[i].loadFromFile(pNorm);
        texDiffAktif[i].loadFromFile(pAkt);
        btnDiff[i].muat(pNorm, "", font, labelDiff[i], 28, sf::Vector2f(71.0f + i * 299.0f, 277.0f));
        btnDiff[i].aturPosisiTeksY(41.0f);
    }
    perbaruiTampilanKesulitan();

    btnSetupKembali.muat("assets/ui/btn_418x74_dark_normal.png", "assets/ui/btn_418x74_dark_hover.png",
                         font, "KEMBALI", 36, sf::Vector2f(74.0f, 523.0f));
    btnSetupMulai.muat("assets/ui/btn_418x74_yellow_normal.png", "assets/ui/btn_418x74_yellow_hover.png",
                       font, "MULAI MAIN", 36, sf::Vector2f(524.0f, 523.0f));

    // --- Pengaturan ---
    aturTeksJudul(teksJudulPengaturan, "PENGATURAN", 60, sf::Vector2f(512.0f, 68.0f));
    aturTeksLabel(teksLabelKain, "Pilih Warna Kain Meja Biliar:", 24, sf::Vector2f(78.0f, 128.0f));
    aturTeksLabel(teksLabelSfx, "Volume Efek Suara (SFX):", 24, sf::Vector2f(78.0f, 300.0f));
    aturTeksLabel(teksLabelBgm, "Volume Musik Latar (BGM):", 24, sf::Vector2f(78.0f, 390.0f));

    const char* namaKain[3] = { "hijau", "biru", "merah" };
    const char* labelKain[3] = { "HIJAU KLASIK", "BIRU ELEKTRIK", "MERAH MARUN" };
    for (int i = 0; i < 3; i++)
    {
        std::string pNorm = std::string("assets/ui/card_felt_") + namaKain[i] + "_normal.png";
        std::string pAkt = std::string("assets/ui/card_felt_") + namaKain[i] + "_active.png";
        texKainNormal[i].loadFromFile(pNorm);
        texKainAktif[i].loadFromFile(pAkt);
        btnKain[i].muat(pNorm, "", font, labelKain[i], 24, sf::Vector2f(71.0f + i * 298.0f, 161.0f));
    }

    if (texSliderTrack.loadFromFile("assets/ui/slider_track.png"))
    {
        sprTrackSfx.setTexture(texSliderTrack);
        sprTrackSfx.setPosition(SLIDER_X, SLIDER_SFX_Y);
        sprTrackBgm.setTexture(texSliderTrack);
        sprTrackBgm.setPosition(SLIDER_X, SLIDER_BGM_Y);
    }
    if (texSliderFill.loadFromFile("assets/ui/slider_fill.png"))
    {
        sprFillSfx.setTexture(texSliderFill);
        sprFillSfx.setPosition(SLIDER_X + 3.0f, SLIDER_SFX_Y + 3.0f);
        sprFillBgm.setTexture(texSliderFill);
        sprFillBgm.setPosition(SLIDER_X + 3.0f, SLIDER_BGM_Y + 3.0f);
    }
    if (texSliderThumb.loadFromFile("assets/ui/slider_thumb_8ball.png"))
    {
        texSliderThumb.setSmooth(true);
        sprThumbSfx.setTexture(texSliderThumb);
        sprThumbSfx.setOrigin(22.0f, 22.0f);
        sprThumbBgm.setTexture(texSliderThumb);
        sprThumbBgm.setOrigin(22.0f, 22.0f);
    }

    aturTeksLabel(teksPersenSfx, "80%", 24, sf::Vector2f(872.0f, 334.0f));
    teksPersenSfx.setFillColor(sf::Color(255, 225, 40));
    aturTeksLabel(teksPersenBgm, "60%", 24, sf::Vector2f(872.0f, 424.0f));
    teksPersenBgm.setFillColor(sf::Color(255, 225, 40));

    btnPengaturanKembali.muat("assets/ui/btn_388x74_dark_normal.png", "assets/ui/btn_388x74_dark_hover.png",
                              font, "KEMBALI", 36, sf::Vector2f(74.0f, 523.0f));
    btnPengaturanSimpan.muat("assets/ui/btn_388x74_yellow_normal.png", "assets/ui/btn_388x74_yellow_hover.png",
                             font, "SIMPAN", 36, sf::Vector2f(550.0f, 523.0f));

    // --- Menu Jeda ---
    overlayGelap.setSize(sf::Vector2f(1024.0f, 640.0f));
    overlayGelap.setFillColor(sf::Color(12, 18, 28, 195));
    aturTeksJudul(teksJudulJeda, "PERMAINAN DIJEDA", 38, sf::Vector2f(512.0f, 176.0f));

    btnJedaLanjut.muat("assets/ui/btn_390x68_yellow_normal.png", "assets/ui/btn_390x68_yellow_hover.png",
                       font, "LANJUTKAN PERMAINAN", 26, sf::Vector2f(313.0f, 238.0f));
    btnJedaPengaturan.muat("assets/ui/btn_390x68_blue_normal.png", "assets/ui/btn_390x68_blue_hover.png",
                           font, "PENGATURAN", 26, sf::Vector2f(313.0f, 324.0f));
    btnJedaMenuUtama.muat("assets/ui/btn_390x68_dark_normal.png", "assets/ui/btn_390x68_dark_hover.png",
                          font, "MENU UTAMA", 26, sf::Vector2f(313.0f, 410.0f));

    // --- Papan Skor ---
    aturTeksJudul(teksJudulSkor, "PAPAN SKOR", 60, sf::Vector2f(512.0f, 66.0f));
    if (texHeaderTabel.loadFromFile("assets/ui/table_header_bg.png"))
    {
        sprHeaderTabel.setTexture(texHeaderTabel);
        sprHeaderTabel.setPosition(76.0f, 120.0f);
    }
    texBarisTop.loadFromFile("assets/ui/table_row_top.png");
    texBarisNormal.loadFromFile("assets/ui/table_row_normal.png");
    texMedali[0].loadFromFile("assets/ui/medal_1_gold.png");
    texMedali[1].loadFromFile("assets/ui/medal_2_silver.png");
    texMedali[2].loadFromFile("assets/ui/medal_3_bronze.png");
    for (int i = 0; i < 5; i++)
    {
        char buf[64];
        std::sprintf(buf, "assets/ui/avatar_%d.png", i + 1);
        texAvatar[i].loadFromFile(buf);
    }
    btnSkorKembali.muat("assets/ui/btn_388x74_yellow_normal.png", "assets/ui/btn_388x74_yellow_hover.png",
                        font, "KEMBALI KE MENU", 32, sf::Vector2f(314.0f, 523.0f));

    if (pengaturan)
    {
        draftWarna = pengaturan->getWarnaMeja();
        draftSfx = pengaturan->getVolumeSfx();
        draftBgm = pengaturan->getVolumeBgm();
    }
    perbaruiTampilanKain();
    sinkronSliderVisual();
}

void Menu::aturTeksJudul(sf::Text& t, const std::string& isi, unsigned int ukuran, sf::Vector2f tengah)
{
    t.setFont(font);
    t.setString(isi);
    t.setCharacterSize(ukuran);
    t.setFillColor(sf::Color(245, 215, 0));
    t.setOutlineColor(sf::Color::Black);
    t.setOutlineThickness(4.0f);
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
    t.setPosition(tengah);
}

void Menu::aturTeksLabel(sf::Text& t, const std::string& isi, unsigned int ukuran, sf::Vector2f posisi)
{
    t.setFont(font);
    t.setString(isi);
    t.setCharacterSize(ukuran);
    t.setFillColor(sf::Color::White);
    t.setOutlineColor(sf::Color::Black);
    t.setOutlineThickness(2.0f);
    t.setPosition(posisi);
}

void Menu::perbaruiTampilanKesulitan()
{
    const char* namaDiff[3] = { "mudah", "sedang", "sulit" };
    const char* labelDiff[3] = { "MUDAH", "SEDANG", "SULIT" };
    for (int i = 0; i < 3; i++)
    {
        std::string p = std::string("assets/ui/card_diff_") + namaDiff[i] +
                        (i == tingkatKesulitan ? "_active.png" : "_normal.png");
        btnDiff[i].muat(p, "", font, labelDiff[i], 28, sf::Vector2f(71.0f + i * 299.0f, 277.0f));
        btnDiff[i].aturPosisiTeksY(41.0f);
        btnDiff[i].aturAktif(i == tingkatKesulitan);
    }
}

void Menu::perbaruiTampilanKain()
{
    const char* namaKain[3] = { "hijau", "biru", "merah" };
    const char* labelKain[3] = { "HIJAU KLASIK", "BIRU ELEKTRIK", "MERAH MARUN" };
    int aktifIdx = 0;
    if (draftWarna == "biru") aktifIdx = 1;
    else if (draftWarna == "merah") aktifIdx = 2;

    for (int i = 0; i < 3; i++)
    {
        std::string p = std::string("assets/ui/card_felt_") + namaKain[i] +
                        (i == aktifIdx ? "_active.png" : "_normal.png");
        btnKain[i].muat(p, "", font, labelKain[i], 24, sf::Vector2f(71.0f + i * 298.0f, 161.0f));
        btnKain[i].aturAktif(i == aktifIdx);
    }
}

void Menu::sinkronSliderVisual()
{
    int wSfx = static_cast<int>(764.0f * draftSfx / 100.0f);
    if (wSfx < 12) wSfx = 12;
    sprFillSfx.setTextureRect(sf::IntRect(0, 0, wSfx, 16));
    sprThumbSfx.setPosition(SLIDER_X + SLIDER_W * (draftSfx / 100.0f), SLIDER_SFX_Y + 11.0f);

    int wBgm = static_cast<int>(764.0f * draftBgm / 100.0f);
    if (wBgm < 12) wBgm = 12;
    sprFillBgm.setTextureRect(sf::IntRect(0, 0, wBgm, 16));
    sprThumbBgm.setPosition(SLIDER_X + SLIDER_W * (draftBgm / 100.0f), SLIDER_BGM_Y + 11.0f);

    char buf[16];
    std::sprintf(buf, "%d%%", draftSfx);
    teksPersenSfx.setString(buf);
    std::sprintf(buf, "%d%%", draftBgm);
    teksPersenBgm.setString(buf);
}

void Menu::perbaruiSliderDariMouse(float mouseX)
{
    float rasio = (mouseX - SLIDER_X) / SLIDER_W;
    if (rasio < 0.0f) rasio = 0.0f;
    if (rasio > 1.0f) rasio = 1.0f;
    int nilai = static_cast<int>(rasio * 100.0f + 0.5f);

    if (sliderAktif == 0)
    {
        draftSfx = nilai;
    }
    else if (sliderAktif == 1)
    {
        draftBgm = nilai;
        if (suara)
        {
            suara->aturVolumeBgm(draftBgm);
        }
    }
    sinkronSliderVisual();
}

void Menu::putarKlik()
{
    if (suara)
    {
        suara->putarEfek(SFX_KLIK_MENU);
    }
}

void Menu::aturPengelolaSuara(PengelolaSuara* s)
{
    suara = s;
}

void Menu::aturPapanSkor(PapanSkor* p)
{
    papanSkor = p;
}

LayarAktif Menu::getLayar() const
{
    return layar;
}

void Menu::aturLayar(LayarAktif l)
{
    if (l == LAYAR_PENGATURAN && layar != LAYAR_PENGATURAN)
    {
        layarSebelumPengaturan = layar;
        if (pengaturan)
        {
            draftWarna = pengaturan->getWarnaMeja();
            draftSfx = pengaturan->getVolumeSfx();
            draftBgm = pengaturan->getVolumeBgm();
            perbaruiTampilanKain();
            sinkronSliderVisual();
        }
    }
    if (l == LAYAR_PAPAN_SKOR && papanSkor)
    {
        papanSkor->muat();
    }
    layar = l;
}

const sf::Font& Menu::getFont() const
{
    return font;
}

std::string Menu::getNamaPemain() const
{
    return namaPemain.empty() ? "Pemain 1" : namaPemain;
}

std::string Menu::getNamaPemain2() const
{
    return namaPemain2.empty() ? "Pemain 2" : namaPemain2;
}

void Menu::ketikNama(std::string& nama, sf::Text& teks, sf::Uint32 ch)
{
    if (ch == 8) // Backspace
    {
        if (!nama.empty())
        {
            nama.erase(nama.size() - 1);
        }
    }
    else if (ch >= 32 && ch < 127 && nama.size() < 16)
    {
        nama += static_cast<char>(ch);
    }
    teks.setString(nama);
}

int Menu::getTingkatKesulitan() const
{
    return tingkatKesulitan;
}

bool Menu::apakahLawanKomputer() const
{
    return lawanKomputer;
}

AksiMenu Menu::prosesEvent(const sf::Event& e)
{
    if (e.type == sf::Event::MouseMoved)
    {
        sf::Vector2f m(static_cast<float>(e.mouseMove.x), static_cast<float>(e.mouseMove.y));
        if (layar == LAYAR_MENU_UTAMA)
        {
            btnMulaiMain.perbaruiHover(m);
            btnPengaturan.perbaruiHover(m);
            btnPapanSkor.perbaruiHover(m);
            btnKeluar.perbaruiHover(m);
        }
        else if (layar == LAYAR_PILIH_MODE)
        {
            btnModeManusia.perbaruiHover(m);
            btnModeKomputer.perbaruiHover(m);
            btnModeKembali.perbaruiHover(m);
        }
        else if (layar == LAYAR_SETUP_PEMAIN)
        {
            btnSetupKembali.perbaruiHover(m);
            btnSetupMulai.perbaruiHover(m);
        }
        else if (layar == LAYAR_PENGATURAN)
        {
            btnPengaturanKembali.perbaruiHover(m);
            btnPengaturanSimpan.perbaruiHover(m);
            if (sliderAktif >= 0)
            {
                perbaruiSliderDariMouse(m.x);
            }
        }
        else if (layar == LAYAR_JEDA)
        {
            btnJedaLanjut.perbaruiHover(m);
            btnJedaPengaturan.perbaruiHover(m);
            btnJedaMenuUtama.perbaruiHover(m);
        }
        else if (layar == LAYAR_PAPAN_SKOR)
        {
            btnSkorKembali.perbaruiHover(m);
        }
    }

    if (layar == LAYAR_MENU_UTAMA)
    {
        if (btnMulaiMain.diklik(e))
        {
            putarKlik();
            layar = LAYAR_PILIH_MODE;
        }
        else if (btnPengaturan.diklik(e))
        {
            putarKlik();
            aturLayar(LAYAR_PENGATURAN);
        }
        else if (btnPapanSkor.diklik(e))
        {
            putarKlik();
            aturLayar(LAYAR_PAPAN_SKOR);
        }
        else if (btnKeluar.diklik(e))
        {
            putarKlik();
            return AKSI_KELUAR;
        }
    }
    else if (layar == LAYAR_PILIH_MODE)
    {
        // Mode manusia mengisi dua nama; mode komputer mengisi nama dan tingkat.
        if (btnModeManusia.diklik(e))
        {
            putarKlik();
            lawanKomputer = false;
            teksLabelNama.setString("Nama Pemain 1:");
            inputNamaAktif = true;
            inputNama2Aktif = false;
            layar = LAYAR_SETUP_PEMAIN;
        }
        else if (btnModeKomputer.diklik(e))
        {
            putarKlik();
            lawanKomputer = true;
            teksLabelNama.setString("Nama Pemain:");
            inputNamaAktif = true;
            inputNama2Aktif = false;
            layar = LAYAR_SETUP_PEMAIN;
        }
        else if (btnModeKembali.diklik(e))
        {
            putarKlik();
            layar = LAYAR_MENU_UTAMA;
        }
    }
    else if (layar == LAYAR_SETUP_PEMAIN)
    {
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f p(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y));
            inputNamaAktif = sprInputBox.getGlobalBounds().contains(p);
            inputNama2Aktif = !lawanKomputer && sprInputBox2.getGlobalBounds().contains(p);
        }
        if (e.type == sf::Event::TextEntered)
        {
            sf::Uint32 ch = e.text.unicode;
            if (ch == 9 && !lawanKomputer) // Tab berpindah kolom nama
            {
                bool keKedua = !inputNama2Aktif;
                inputNama2Aktif = keKedua;
                inputNamaAktif = !keKedua;
            }
            else if (inputNamaAktif)
            {
                ketikNama(namaPemain, teksInputNama, ch);
            }
            else if (inputNama2Aktif)
            {
                ketikNama(namaPemain2, teksInputNama2, ch);
            }
        }
        for (int i = 0; i < 3 && lawanKomputer; i++)
        {
            if (btnDiff[i].diklik(e))
            {
                putarKlik();
                tingkatKesulitan = i;
                perbaruiTampilanKesulitan();
            }
        }
        if (btnSetupKembali.diklik(e))
        {
            putarKlik();
            layar = LAYAR_PILIH_MODE;
        }
        else if (btnSetupMulai.diklik(e))
        {
            putarKlik();
            layar = LAYAR_BERMAIN;
            return AKSI_MULAI_PERTANDINGAN;
        }
    }
    else if (layar == LAYAR_PENGATURAN)
    {
        const char* namaKain[3] = { "hijau", "biru", "merah" };
        for (int i = 0; i < 3; i++)
        {
            if (btnKain[i].diklik(e))
            {
                putarKlik();
                draftWarna = namaKain[i];
                perbaruiTampilanKain();
            }
        }
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
        {
            float mx = static_cast<float>(e.mouseButton.x);
            float my = static_cast<float>(e.mouseButton.y);
            if (mx >= SLIDER_X - 15.0f && mx <= SLIDER_X + SLIDER_W + 15.0f)
            {
                if (my >= SLIDER_SFX_Y - 14.0f && my <= SLIDER_SFX_Y + 36.0f)
                {
                    sliderAktif = 0;
                    perbaruiSliderDariMouse(mx);
                }
                else if (my >= SLIDER_BGM_Y - 14.0f && my <= SLIDER_BGM_Y + 36.0f)
                {
                    sliderAktif = 1;
                    perbaruiSliderDariMouse(mx);
                }
            }
        }
        else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left)
        {
            if (sliderAktif == 0 && suara)
            {
                suara->aturVolumeSfx(draftSfx);
                suara->putarEfek(SFX_KLIK_MENU);
            }
            sliderAktif = -1;
        }

        if (btnPengaturanKembali.diklik(e))
        {
            if (suara && pengaturan)
            {
                suara->terapkanPengaturan(*pengaturan);
            }
            putarKlik();
            layar = layarSebelumPengaturan;
        }
        else if (btnPengaturanSimpan.diklik(e))
        {
            if (pengaturan)
            {
                pengaturan->aturWarnaMeja(draftWarna);
                pengaturan->aturVolumeSfx(draftSfx);
                pengaturan->aturVolumeBgm(draftBgm);
            }
            if (suara && pengaturan)
            {
                suara->terapkanPengaturan(*pengaturan);
            }
            putarKlik();
            layar = layarSebelumPengaturan;
            return AKSI_TERAPKAN_PENGATURAN;
        }
    }
    else if (layar == LAYAR_JEDA)
    {
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
        {
            putarKlik();
            layar = LAYAR_BERMAIN;
            return AKSI_LANJUTKAN;
        }
        if (btnJedaLanjut.diklik(e))
        {
            putarKlik();
            layar = LAYAR_BERMAIN;
            return AKSI_LANJUTKAN;
        }
        else if (btnJedaPengaturan.diklik(e))
        {
            putarKlik();
            aturLayar(LAYAR_PENGATURAN);
        }
        else if (btnJedaMenuUtama.diklik(e))
        {
            putarKlik();
            layar = LAYAR_MENU_UTAMA;
        }
    }
    else if (layar == LAYAR_PAPAN_SKOR)
    {
        if (btnSkorKembali.diklik(e))
        {
            putarKlik();
            layar = LAYAR_MENU_UTAMA;
        }
    }

    return AKSI_TIDAK_ADA;
}

void Menu::update(float dt)
{
    waktuKursor += dt;
    if (waktuKursor >= 1.0f)
    {
        waktuKursor = 0.0f;
    }
    sf::FloatRect b = teksInputNama.getGlobalBounds();
    float kx = namaPemain.empty() ? 102.0f : (b.left + b.width + 6.0f);
    kursorInput.setPosition(kx, 178.0f);
    b = teksInputNama2.getGlobalBounds();
    kx = namaPemain2.empty() ? 102.0f : (b.left + b.width + 6.0f);
    kursorInput2.setPosition(kx, 294.0f);
}

void Menu::gambar(sf::RenderWindow& w) const
{
    if (layar == LAYAR_MENU_UTAMA)
    {
        w.draw(sprBgMainMenu);
        btnMulaiMain.gambar(w);
        btnPengaturan.gambar(w);
        btnPapanSkor.gambar(w);
        btnKeluar.gambar(w);
    }
    else if (layar == LAYAR_PILIH_MODE)
    {
        w.draw(sprBgArcade);
        w.draw(sprGemCyan);
        w.draw(sprGemPink);
        w.draw(teksJudulMode);
        btnModeManusia.gambar(w);
        w.draw(teksDescManusia);
        btnModeKomputer.gambar(w);
        w.draw(teksDescKomputer);
        btnModeKembali.gambar(w);
    }
    else if (layar == LAYAR_SETUP_PEMAIN)
    {
        w.draw(sprBgArcade);
        w.draw(sprGemCyan);
        w.draw(sprGemPink);
        w.draw(teksJudulSetup);
        w.draw(teksLabelNama);
        w.draw(sprInputBox);
        w.draw(teksInputNama);
        if (inputNamaAktif && waktuKursor < 0.55f)
        {
            w.draw(kursorInput);
        }
        w.draw(sprPensil);
        if (lawanKomputer)
        {
            w.draw(teksLabelKesulitan);
            for (int i = 0; i < 3; i++)
            {
                btnDiff[i].gambar(w);
            }
            sf::Sprite bintang = sprStarBadge;
            bintang.setPosition(78.0f + tingkatKesulitan * 299.0f + 236.0f, 274.0f);
            w.draw(bintang);
        }
        else
        {
            w.draw(teksLabelNama2);
            w.draw(sprInputBox2);
            w.draw(teksInputNama2);
            if (inputNama2Aktif && waktuKursor < 0.55f)
            {
                w.draw(kursorInput2);
            }
            w.draw(sprPensil2);
        }

        btnSetupKembali.gambar(w);
        btnSetupMulai.gambar(w);
    }
    else if (layar == LAYAR_PENGATURAN)
    {
        w.draw(sprBgArcade);
        w.draw(sprGemCyan);
        w.draw(sprGemPink);
        w.draw(teksJudulPengaturan);
        w.draw(teksLabelKain);
        for (int i = 0; i < 3; i++)
        {
            btnKain[i].gambar(w);
        }
        int aktifIdx = 0;
        if (draftWarna == "biru") aktifIdx = 1;
        else if (draftWarna == "merah") aktifIdx = 2;
        sf::Sprite bintang = sprStarBadge;
        bintang.setPosition(78.0f + aktifIdx * 298.0f + 236.0f, 158.0f);
        w.draw(bintang);

        w.draw(teksLabelSfx);
        w.draw(sprTrackSfx);
        w.draw(sprFillSfx);
        w.draw(sprThumbSfx);
        w.draw(teksPersenSfx);

        w.draw(teksLabelBgm);
        w.draw(sprTrackBgm);
        w.draw(sprFillBgm);
        w.draw(sprThumbBgm);
        w.draw(teksPersenBgm);

        btnPengaturanKembali.gambar(w);
        btnPengaturanSimpan.gambar(w);
    }
    else if (layar == LAYAR_JEDA)
    {
        w.draw(overlayGelap);
        w.draw(sprPapanJeda);
        w.draw(teksJudulJeda);
        btnJedaLanjut.gambar(w);
        btnJedaPengaturan.gambar(w);
        btnJedaMenuUtama.gambar(w);
    }
    else if (layar == LAYAR_PAPAN_SKOR)
    {
        w.draw(sprBgArcade);
        w.draw(sprStarCluster);
        w.draw(sprChalkBlue);
        w.draw(teksJudulSkor);
        w.draw(sprHeaderTabel);

        const float colX[5] = { 145.0f, 335.0f, 545.0f, 705.0f, 855.0f };
        const char* hdr[5] = { "Peringkat", "Nama Pemain", "Skor Akhir", "Tingkat", "Tanggal" };
        for (int c = 0; c < 5; c++)
        {
            sf::Text th;
            th.setFont(font);
            th.setString(hdr[c]);
            th.setCharacterSize(18);
            th.setFillColor(sf::Color(255, 220, 40));
            th.setOutlineColor(sf::Color::Black);
            th.setOutlineThickness(2.0f);
            sf::FloatRect b = th.getLocalBounds();
            th.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
            th.setPosition(colX[c], 144.0f);
            w.draw(th);
        }

        const std::vector<EntriSkor>* daftar = papanSkor ? &papanSkor->getDaftar() : 0;
        if (daftar == 0 || daftar->empty())
        {
            sf::Text teksKosong;
            teksKosong.setFont(font);
            teksKosong.setString("Belum ada rekor skor.");
            teksKosong.setCharacterSize(28);
            teksKosong.setFillColor(sf::Color(255, 225, 40));
            teksKosong.setOutlineColor(sf::Color::Black);
            teksKosong.setOutlineThickness(2.5f);
            sf::FloatRect bk = teksKosong.getLocalBounds();
            teksKosong.setOrigin(bk.left + bk.width / 2.0f, bk.top + bk.height / 2.0f);
            teksKosong.setPosition(512.0f, 305.0f);
            w.draw(teksKosong);

            sf::Text teksPetunjuk;
            teksPetunjuk.setFont(font);
            teksPetunjuk.setString("Menangkan pertandingan melawan Komputer untuk mencatat skor!");
            teksPetunjuk.setCharacterSize(19);
            teksPetunjuk.setFillColor(sf::Color(210, 224, 240));
            teksPetunjuk.setOutlineColor(sf::Color::Black);
            teksPetunjuk.setOutlineThickness(2.0f);
            sf::FloatRect bp = teksPetunjuk.getLocalBounds();
            teksPetunjuk.setOrigin(bp.left + bp.width / 2.0f, bp.top + bp.height / 2.0f);
            teksPetunjuk.setPosition(512.0f, 342.0f);
            w.draw(teksPetunjuk);
        }
        else
        {
            size_t maksBaris = (daftar->size() < 5) ? daftar->size() : 5;
            for (size_t r = 0; r < maksBaris; r++)
            {
                const EntriSkor& entri = (*daftar)[r];
                float ry = 178.0f + static_cast<float>(r) * 64.0f;
                sf::Sprite rowSpr(r == 0 ? texBarisTop : texBarisNormal);
                rowSpr.setPosition(76.0f, ry);
                w.draw(rowSpr);

                if (r < 3)
                {
                    sf::Sprite med(texMedali[r]);
                    med.setPosition(127.0f, ry + 9.0f);
                    w.draw(med);
                }
                else
                {
                    sf::Text rk;
                    rk.setFont(font);
                    char rbuf[8];
                    std::sprintf(rbuf, "%u", static_cast<unsigned int>(r + 1));
                    rk.setString(rbuf);
                    rk.setCharacterSize(20);
                    rk.setFillColor(sf::Color(185, 202, 224));
                    rk.setOutlineColor(sf::Color::Black);
                    rk.setOutlineThickness(2.0f);
                    sf::FloatRect b = rk.getLocalBounds();
                    rk.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
                    rk.setPosition(145.0f, ry + 29.0f);
                    w.draw(rk);
                }

                sf::Sprite av(texAvatar[r]);
                av.setPosition(247.0f, ry + 12.0f);
                w.draw(av);

                sf::Text nm;
                nm.setFont(font);
                nm.setString(entri.nama);
                nm.setCharacterSize(20);
                nm.setFillColor(sf::Color::White);
                nm.setOutlineColor(sf::Color::Black);
                nm.setOutlineThickness(2.0f);
                nm.setPosition(295.0f, ry + 16.0f);
                w.draw(nm);

                char sbuf[16];
                std::sprintf(sbuf, "%d", entri.skor);
                sf::Text sk;
                sk.setFont(font);
                sk.setString(sbuf);
                sk.setCharacterSize(20);
                sk.setFillColor(sf::Color(255, 225, 40));
                sk.setOutlineColor(sf::Color::Black);
                sk.setOutlineThickness(2.0f);
                sf::FloatRect bs = sk.getLocalBounds();
                sk.setOrigin(bs.left + bs.width / 2.0f, bs.top + bs.height / 2.0f);
                sk.setPosition(545.0f, ry + 29.0f);
                w.draw(sk);

                sf::Text tg;
                tg.setFont(font);
                tg.setString(entri.tingkat);
                tg.setCharacterSize(20);
                if (entri.tingkat == "Mudah") tg.setFillColor(sf::Color(110, 235, 130));
                else if (entri.tingkat == "Sedang") tg.setFillColor(sf::Color(90, 195, 255));
                else tg.setFillColor(sf::Color(255, 115, 115));
                tg.setOutlineColor(sf::Color::Black);
                tg.setOutlineThickness(2.0f);
                sf::FloatRect bt = tg.getLocalBounds();
                tg.setOrigin(bt.left + bt.width / 2.0f, bt.top + bt.height / 2.0f);
                tg.setPosition(705.0f, ry + 29.0f);
                w.draw(tg);

                sf::Text dt;
                dt.setFont(font);
                dt.setString(entri.tanggal);
                dt.setCharacterSize(17);
                dt.setFillColor(sf::Color(185, 202, 224));
                dt.setOutlineColor(sf::Color::Black);
                dt.setOutlineThickness(2.0f);
                sf::FloatRect bd = dt.getLocalBounds();
                dt.setOrigin(bd.left + bd.width / 2.0f, bd.top + bd.height / 2.0f);
                dt.setPosition(855.0f, ry + 29.0f);
                w.draw(dt);
            }
        }

        w.draw(sprCueDeco);
        btnSkorKembali.gambar(w);
    }
}
