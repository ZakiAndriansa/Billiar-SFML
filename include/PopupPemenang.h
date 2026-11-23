#pragma once
#include "Tombol.h"
#include "Permainan.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum AksiPopup
{
    POPUP_TIDAK_ADA,
    POPUP_MAIN_LAGI,
    POPUP_MENU_UTAMA
};

// Satu potong konfeti yang ditembakkan saat pertandingan dimenangkan.
struct Konfeti
{
    sf::Vector2f posisi;
    sf::Vector2f kecepatan;
    float sudut;
    float putaran;      // derajat per detik
    float fase;         // untuk gerak melambai dan efek berbalik
    sf::Vector2f ukuran;
    sf::Color warna;
};

// Pop up kemenangan yang muncul ketika pertandingan selesai: layar meredup,
// papan kayu meloncat masuk, sinar berputar di belakangnya, dan konfeti
// ditembakkan dari dua sudut bawah. Papan memuat nama dan avatar pemenang,
// skor akhir, alasan menang, serta tombol Main Lagi dan Menu Utama.
class PopupPemenang
{
private:
    sf::Font font;
    bool fontSiap;
    sf::Texture texPapan;
    sf::Texture texAvatarManusia;
    sf::Texture texAvatarKomputer;
    sf::Texture texMedali;
    sf::Texture texBintang;
    sf::Sprite sprPapan;
    sf::Sprite sprAvatar;
    sf::Sprite sprMedali;
    sf::Sprite sprBintangKiri;
    sf::Sprite sprBintangKanan;

    sf::Text teksJudul;
    sf::Text teksNama;
    sf::Text teksAlasan;
    sf::Text teksSkor;
    Tombol btnMainLagi;
    Tombol btnMenuUtama;

    bool aktif;
    bool menangKomputer;    // komputer mengalahkan pemain manusia
    float waktu;            // detik sejak pertandingan selesai
    float waktuKonfeti;     // sisa waktu hujan konfeti
    unsigned long benih;
    std::vector<Konfeti> konfeti;

    float acak(float bawah, float atas);
    void tembakKonfeti(sf::Vector2f asal, float arahX, int jumlah);
    void hujanKonfeti(int jumlah);
    void aturTeks(sf::Text& t, unsigned int ukuran, sf::Color isi, sf::Color garis, float tebal);
    void pusatkan(sf::Text& t, float x, float y, float lebarMaks);
    float skalaMuncul() const;
    void gambarSinar(sf::RenderWindow& w, sf::Vector2f pusat, float panjang, int jumlah,
                     sf::Color warna, float sudutAwal) const;

public:
    PopupPemenang();
    void tampilkan(const Permainan& permainan);
    void sembunyikan();
    bool apakahAktif() const;
    AksiPopup prosesEvent(const sf::Event& e);
    void update(float dt);
    void gambar(sf::RenderWindow& w);
};
