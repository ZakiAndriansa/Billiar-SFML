#pragma once
#include "Meja.h"
#include "Bola.h"
#include "Stik.h"
#include "PemainManusia.h"
#include "PemainKomputer.h"
#include "Wasit.h"
#include <SFML/Graphics.hpp>
#include <vector>

// Menyatukan semua objek di meja dan menjalankan satu pertandingan.
class Permainan
{
private:
    Meja meja;
    Stik stik;
    Bola* bola[16];
    PemainManusia pemain1;
    PemainManusia pemain2;        // lawan saat mode dua pemain
    PemainKomputer komputer;      // lawan saat mode melawan komputer
    Pemain* lawan;                // menunjuk pemain2 atau komputer
    Wasit wasit;                  // harus dideklarasikan setelah semua pemain
    bool selesai;
    bool bolaBergerak;
    bool menaruhBola;             // bola di tangan: pemain sedang menaruh bola putih
    bool posisiTaruhSah;          // posisi bola putih saat ini boleh ditempati
    float sisaWaktu;
    int sentuhanPertama;          // nomor bola pertama yang disentuh bola putih, -1 jika belum ada
    std::vector<int> bolaMasuk;   // nomor bola yang masuk lubang selama satu pukulan

    void susunBola();
    void langkahFisika(float h);
    bool semuaBerhenti() const;
    void selesaiPukulan();
    void kembalikanBolaPutih();
    sf::FloatRect areaTaruh() const;
    bool posisiSah(sf::Vector2f p) const;
    void prosesTaruhBola(float dt);

public:
    Permainan();
    ~Permainan();
    void mulaiPertandingan();
    void batalkanBidikan();
    void aturNamaPemain1(const std::string& nama);
    void aturNamaPemain2(const std::string& nama);
    void aturLawanKomputer(bool status);
    bool apakahLawanKomputer() const;
    void aturTingkat(Tingkat t);
    void aturWarnaMeja(const std::string& warna);
    const Pemain& getPemain1() const;
    const Pemain& getPemain2() const;
    const Wasit& getWasit() const;
    const Bola* getBola(int indeks) const;
    float getKekuatan() const;
    bool apakahSelesai() const;
    bool apakahMenaruhBola() const;
    void prosesEvent(const sf::Event& e);
    void update(float dt);
    void gambar(sf::RenderWindow& w);
};
