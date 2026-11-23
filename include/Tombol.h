#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Komponen tombol UI berbasis sprite (normal/hover) dan teks LemonDays.
class Tombol
{
private:
    sf::Texture teksturNormal;
    sf::Texture teksturHover;
    sf::Sprite sprite;
    sf::Text teks;
    sf::FloatRect areaKlik;
    bool siapNormal;
    bool siapHover;
    bool sedangHover;
    bool aktif;

    void pusatkanTeks();

public:
    Tombol();
    bool muat(const std::string& pathNormal,
              const std::string& pathHover,
              const sf::Font& font,
              const std::string& label,
              unsigned int ukuranTeks,
              sf::Vector2f posisi);

    void aturPosisi(sf::Vector2f posisi);
    void aturPosisiTeksY(float offsetY);
    void aturLabel(const std::string& label);
    void aturAktif(bool status);
    bool apakahAktif() const;

    void perbaruiHover(sf::Vector2f posisiMouse);
    bool diklik(const sf::Event& e) const;
    void gambar(sf::RenderWindow& w) const;
};
