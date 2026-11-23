#pragma once
#include "Bola.h"
#include "Stik.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <string>

// Kelas abstrak untuk satu pemain. Turunannya adalah PemainManusia, yang memukul
// lewat mouse, dan PemainKomputer, yang memilih pukulan sendiri (WBS 4.1 dan 4.2).
class Pemain
{
protected:
    std::string nama;
    JenisBola jenisBola;   // Putih berarti jenis bola belum ditentukan
    int skor;

public:
    Pemain(const std::string& namaAwal);
    virtual ~Pemain() {}

    // Dipanggil setiap frame selama giliran pemain ini dan semua bola diam.
    // Pemain mengarahkan stik, lalu mengembalikan true pada frame ketika stik
    // memukul bola putih.
    virtual bool ambilGiliran(Stik& stik, Bola& bolaPutih, float dt) = 0;

    // Bola di tangan (UC-04): dipanggil setiap frame selama pemain menaruh bola
    // putih setelah lawan melanggar. posisi berisi posisi bola putih sekarang dan
    // diisi posisi yang diinginkan; area adalah daerah yang boleh ditempati pusat
    // bola. Mengembalikan true pada frame ketika pemain memilih posisi itu.
    virtual bool taruhBolaPutih(sf::Vector2f& posisi, const sf::FloatRect& area, float dt) = 0;

    // Masukan mouse. Hanya pemain manusia yang memakainya.
    virtual void bacaEvent(const sf::Event& e) { (void)e; }

    // Membatalkan bidikan yang sedang berjalan, misalnya saat permainan dijeda.
    virtual void batalkan() {}

    virtual bool apakahKomputer() const { return false; }

    void aturNama(const std::string& namaBaru);
    std::string getNama() const;
    JenisBola getJenisBola() const;
    void aturJenisBola(JenisBola j);
    int getSkor() const;
    void tambahSkor(int n);
    void aturUlang();
};
