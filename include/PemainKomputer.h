#pragma once
#include "Pemain.h"
#include "Meja.h"
#include <vector>

enum Tingkat { Mudah, Sedang, Sulit };

// Satu kemungkinan pukulan: bola sasaran dimasukkan ke salah satu lubang.
struct Pukulan
{
    const Bola* sasaran;
    int lubang;               // indeks lubang 0 sampai 5, -1 untuk pukulan cadangan
    sf::Vector2f titikBidik;  // posisi bola putih saat menyentuh sasaran (ghost ball)
    float sudutPotong;        // derajat; 0 berarti sasaran ditabrak lurus
    float jarak;              // bola putih ke titik bidik ditambah sasaran ke lubang
    float nilai;              // makin besar makin baik, cara menilainya bergantung pada tingkat
};

// Lawan komputer (WBS 4.1 dan 4.2). Setiap giliran, komputer mendata semua pasangan
// bola sasaran dan lubang, membuang yang terhalang, memilih pukulan sesuai tingkat,
// menambahkan galat acak, lalu memukul lewat Stik yang sama dengan pemain manusia.
class PemainKomputer : public Pemain
{
private:
    enum Tahap { BERPIKIR, MEMBIDIK, MENGISI };
    enum TahapTaruh { TARUH_BERPIKIR, TARUH_MENGGESER };

    Tingkat tingkat;
    const Meja& meja;
    Bola* const* bola;        // enam belas bola milik Permainan

    Tahap tahap;
    float waktu;
    float sudutAwal;
    float sudutTujuan;
    float kekuatanSaatIni;
    float kekuatanTujuan;
    bool tanpaJeda;           // untuk pengujian: langsung memukul tanpa animasi
    unsigned long benihAcak;
    Pukulan rencana;
    TahapTaruh tahapTaruh;
    sf::Vector2f asalTaruh;
    sf::Vector2f tujuanTaruh;

    bool sasaranSah(const Bola& b) const;
    bool sedangBreak() const;
    std::vector<Pukulan> cariKandidat(const Bola& bolaPutih) const;
    sf::Vector2f titikBidik(const Bola& sasaran, sf::Vector2f lubang) const;
    bool terhalang(sf::Vector2f dari, sf::Vector2f ke, const Bola* abaikan1, const Bola* abaikan2) const;
    float nilaiPukulan(const Pukulan& p) const;
    Pukulan pilihPukulan(std::vector<Pukulan>& kandidat) const;
    Pukulan pukulanCadangan(const Bola& bolaPutih) const;
    float hitungKekuatan(const Pukulan& p) const;
    float lajuSaatMenyentuh(const Pukulan& p, float kekuatan) const;
    float sudutBidik(const Pukulan& p, float kekuatan) const;
    float hitungGalat();
    float acak();             // bilangan acak antara -1 dan 1
    void rencanakan(const Bola& bolaPutih);
    bool posisiKosong(sf::Vector2f p) const;
    float nilaiPosisi(sf::Vector2f p);
    sf::Vector2f cariPosisiTaruh(sf::Vector2f asal, const sf::FloatRect& area);

public:
    PemainKomputer(const std::string& namaAwal, const Meja& meja, Bola* const* bola);
    bool ambilGiliran(Stik& stik, Bola& bolaPutih, float dt);
    bool taruhBolaPutih(sf::Vector2f& posisi, const sf::FloatRect& area, float dt);
    void batalkan();
    bool apakahKomputer() const;

    void aturTingkat(Tingkat t);
    Tingkat getTingkat() const;
    void aturTanpaJeda(bool status);
    void aturBenihAcak(unsigned long benih);
    const Pukulan& getRencana() const;
};
