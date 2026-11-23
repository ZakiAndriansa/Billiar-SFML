#pragma once
#include "Pemain.h"

// Pemain yang memukul dengan mouse: arah stik mengikuti kursor, menahan klik kiri
// mengisi kekuatan, dan melepasnya memukul bola putih.
class PemainManusia : public Pemain
{
private:
    sf::Vector2f arahMouse;   // posisi kursor terakhir
    bool menekan;             // klik kiri sedang ditahan
    bool siapMemukul;         // klik sudah dilepas dengan kekuatan yang cukup
    float kekuatan;           // 0 sampai 100

public:
    PemainManusia(const std::string& namaAwal, sf::Vector2f arahAwal);
    bool ambilGiliran(Stik& stik, Bola& bolaPutih, float dt);
    bool taruhBolaPutih(sf::Vector2f& posisi, const sf::FloatRect& area, float dt);
    void bacaEvent(const sf::Event& e);
    void batalkan();
};
