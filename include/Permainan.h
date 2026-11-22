#pragma once
#include "Meja.h"
#include "Bola.h"
#include "Stik.h"
#include "Pemain.h"
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
    Pemain pemain1;
    Pemain pemain2;
    Wasit wasit;                  // harus dideklarasikan setelah kedua pemain
    bool selesai;
    sf::Vector2f posisiMouse;
    float kekuatan;
    bool menahan;
    bool bolaBergerak;
    float sisaWaktu;
    int sentuhanPertama;          // nomor bola pertama yang disentuh bola putih, -1 jika belum ada
    std::vector<int> bolaMasuk;   // nomor bola yang masuk lubang selama satu pukulan

    void susunBola();
    void langkahFisika(float h);
    bool semuaBerhenti() const;
    void selesaiPukulan();
    void kembalikanBolaPutih();

public:
    Permainan();
    ~Permainan();
    void mulaiPertandingan();
    void aturNamaPemain1(const std::string& nama);
    void aturWarnaMeja(const std::string& warna);
    const Pemain& getPemain1() const;
    const Pemain& getPemain2() const;
    const Wasit& getWasit() const;
    const Bola* getBola(int indeks) const;
    float getKekuatan() const;
    bool apakahSelesai() const;
    void prosesEvent(const sf::Event& e);
    void update(float dt);
    void gambar(sf::RenderWindow& w);
};
