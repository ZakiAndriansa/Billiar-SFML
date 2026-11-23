#pragma once
#include "Tombol.h"
#include "Permainan.h"
#include <SFML/Graphics.hpp>

enum AksiHUD
{
    HUD_TIDAK_ADA,
    HUD_BUKA_JEDA,
    HUD_BUKA_PENGATURAN,
    HUD_ULANG_PERTANDINGAN
};

// Mengelola tampilan HUD gameplay (WBS 5.3): strip atas (avatar, nama,
// jenis bola, deretan bola tersisa, kotak giliran, tombol cepat) dan
// strip bawah (bar kekuatan, banner status wasit, pintasan).
class HUD
{
private:
    sf::Font font;
    bool fontSiap;

    sf::Texture texTopStrip;
    sf::Sprite sprTopStrip;
    sf::Texture texBottomBar;
    sf::Sprite sprBottomBar;

    sf::Texture texAvatarP1Active;
    sf::Texture texAvatarP1Idle;
    sf::Sprite sprAvatarP1;
    sf::Texture texAvatarP2Active;
    sf::Texture texAvatarP2Idle;
    sf::Sprite sprAvatarP2;

    sf::Texture texModeBox;
    sf::Sprite sprModeBox;
    sf::Texture texPillPolos;
    sf::Sprite sprPillPolos;
    sf::Texture texPillBelang;
    sf::Sprite sprPillBelang;

    sf::Texture texBolaMini[16];
    sf::Sprite sprBolaMini[16];
    bool jenisSudahDitentukan;

    Tombol btnMenu;
    Tombol btnPengaturan;
    Tombol btnUlang;
    Tombol btnJeda;

    sf::Text teksNamaP1;
    sf::Text teksNamaP2;
    sf::Text teksPillP1;
    sf::Text teksPillP2;
    sf::Text teksModeAtas;
    sf::Text teksGiliranBawah;

    sf::Text teksLabelKekuatan;
    sf::RectangleShape barKekuatan;
    sf::Text teksPersenKekuatan;
    sf::Text teksStatusWasit;
    sf::Text teksPintasan;

    void aturTeks(sf::Text& t, const std::string& isi, unsigned int ukuran, sf::Color warna, float outline);

public:
    HUD();
    AksiHUD prosesEvent(const sf::Event& e);
    void perbarui(const Permainan& permainan);
    void gambar(sf::RenderWindow& w) const;
};
