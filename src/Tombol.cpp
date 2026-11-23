#include "Tombol.h"
#include <iostream>

Tombol::Tombol()
    : siapNormal(false), siapHover(false), sedangHover(false), aktif(false)
{
}

bool Tombol::muat(const std::string& pathNormal,
                  const std::string& pathHover,
                  const sf::Font& font,
                  const std::string& label,
                  unsigned int ukuranTeks,
                  sf::Vector2f posisi)
{
    siapNormal = teksturNormal.loadFromFile(pathNormal);
    if (!siapNormal)
    {
        std::cerr << "[ERROR] Gagal memuat tombol: " << pathNormal << std::endl;
        return false;
    }
    teksturNormal.setSmooth(true);

    if (!pathHover.empty())
    {
        siapHover = teksturHover.loadFromFile(pathHover);
        if (siapHover)
        {
            teksturHover.setSmooth(true);
        }
    }

    sprite.setTexture(teksturNormal);
    sprite.setPosition(posisi);
    areaKlik = sprite.getGlobalBounds();

    teks.setFont(font);
    teks.setString(label);
    teks.setCharacterSize(ukuranTeks);
    teks.setFillColor(sf::Color::White);
    teks.setOutlineColor(sf::Color::Black);
    teks.setOutlineThickness(2.0f);

    pusatkanTeks();
    return true;
}

void Tombol::pusatkanTeks()
{
    sf::FloatRect b = teks.getLocalBounds();
    teks.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
    teks.setPosition(
        areaKlik.left + areaKlik.width / 2.0f,
        areaKlik.top + areaKlik.height / 2.0f - 2.0f
    );
}

void Tombol::aturPosisiTeksY(float offsetY)
{
    sf::FloatRect b = teks.getLocalBounds();
    teks.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
    teks.setPosition(
        areaKlik.left + areaKlik.width / 2.0f,
        areaKlik.top + offsetY
    );
}

void Tombol::aturPosisi(sf::Vector2f posisi)
{
    sprite.setPosition(posisi);
    areaKlik = sprite.getGlobalBounds();
    pusatkanTeks();
}

void Tombol::aturLabel(const std::string& label)
{
    teks.setString(label);
    pusatkanTeks();
}

void Tombol::aturAktif(bool status)
{
    aktif = status;
}

bool Tombol::apakahAktif() const
{
    return aktif;
}

void Tombol::perbaruiHover(sf::Vector2f posisiMouse)
{
    bool baru = areaKlik.contains(posisiMouse);
    if (baru != sedangHover)
    {
        sedangHover = baru;
        if (sedangHover && siapHover)
        {
            sprite.setTexture(teksturHover);
        }
        else if (siapNormal)
        {
            sprite.setTexture(teksturNormal);
        }
    }
}

bool Tombol::diklik(const sf::Event& e) const
{
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f p(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y));
        return areaKlik.contains(p);
    }
    return false;
}

void Tombol::gambar(sf::RenderWindow& w) const
{
    if (siapNormal)
    {
        w.draw(sprite);
    }
    if (!teks.getString().isEmpty())
    {
        w.draw(teks);
    }
}
